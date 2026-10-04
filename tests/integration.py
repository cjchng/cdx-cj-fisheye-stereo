"""No third-party Python packages required. Uses temporary files and loopback only."""
import json
import math
from pathlib import Path
import socket
import subprocess
import sys
import tempfile
import time
import urllib.error
import urllib.request

cli, server, synthetic = map(str, map(Path, sys.argv[1:]))

def run(*args):
    return subprocess.run(args, check=True, capture_output=True, text=True)

with tempfile.TemporaryDirectory(prefix="fisheye-stereo-test-") as temp:
    root = Path(temp)
    request = root / "request.json"
    run(synthetic, str(request))
    output = root / "result.json"
    run(cli, "calibrate", str(request), str(output))
    result = json.loads(output.read_text())
    assert abs(result["baseline"] - math.sqrt(14425)) < 0.02
    assert subprocess.run([cli, "calibrate", str(request), str(output)], capture_output=True).returncode == 2

    # Detector integration: a clean front-facing board. Identical frames are
    # deliberately used ONLY for extraction; they are unsuitable for calibration.
    width, height = 900, 700
    pixels = bytearray([255]) * (width * height)
    for row in range(7):
        for col in range(10):
            if (row + col) % 2 == 0:
                for y in range(100 + row * 60, 100 + (row + 1) * 60):
                    start = y * width + 120 + col * 60
                    pixels[start:start + 60] = bytes(60)
    board = root / "board.pgm"
    board.write_bytes(f"P5\n{width} {height}\n255\n".encode() + pixels)
    config = {"schema_version": 1, "board_cols": 9, "board_rows": 6,
              "square_size": 30.0, "unit": "mm", "pairs": [
                  {"left": "board.pgm", "right": "board.pgm", "reverse_left": 0,
                   "reverse_right": 0} for _ in range(6)]}
    manifest = root / "pairs.json"
    manifest.write_text(json.dumps(config))
    observations = root / "corners.json"
    run(cli, "detect", str(manifest), str(observations), str(root / "previews"))
    detected = json.loads(observations.read_text())
    assert len(detected["views"]) == 6
    assert len(detected["views"][0]["left"]) == 54
    assert len(list((root / "previews").glob("*.png"))) == 12

    with socket.socket() as sock:
        sock.bind(("127.0.0.1", 0))
        port = sock.getsockname()[1]
    process = subprocess.Popen([server, str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    url = f"http://127.0.0.1:{port}"

    def http(path, data=None, content_type="application/json"):
        req = urllib.request.Request(url + path, data=data,
                                     headers={"Content-Type": content_type})
        try:
            with urllib.request.urlopen(req, timeout=180) as response:
                return response.status, json.loads(response.read())
        except urllib.error.HTTPError as exc:
            return exc.code, json.loads(exc.read())

    try:
        for _ in range(100):
            if process.poll() is not None:
                raise RuntimeError("HTTP server exited before startup")
            try:
                assert http("/health")[0] == 200
                break
            except urllib.error.URLError:
                time.sleep(0.1)
        else:
            raise RuntimeError("HTTP server did not start")
        status, response = http("/v1/calibrate", request.read_bytes())
        assert status == 200, response
        assert abs(response["baseline"] - result["baseline"]) < 1e-8
        assert len(response["R"]) == 3 and len(response["T"][0]) == 1
        assert http("/v1/calibrate", b"{broken")[0] == 400
        assert http("/v1/calibrate", b"{}")[0] == 400
        assert http("/v1/calibrate", b"{}", "text/plain")[0] == 415
        assert http("/missing")[0] == 404
    finally:
        process.terminate()
        try:
            process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()
    print("PASS: CLI, overwrite refusal, image detection/previews, HTTP calibration and error responses")
