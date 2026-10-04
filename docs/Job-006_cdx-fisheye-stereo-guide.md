# C++ fisheye stereo calibration: running methods and API protocol

This example estimates two fisheye camera intrinsics, their relative rotation R, translation T and baseline B using a checkerboard of known dimensions. The implementation provides a C++ library, image-based command-line workflow, and local HTTP JSON API. Source: [fisheye-stereo](https://github.com/cjchng/cdx-cj-fisheye-stereo/tree/main).

## Scope and coordinate convention

The implementation uses OpenCV's four-coefficient fisheye model (KB-style angular polynomial), not Scaramuzza's OCam polynomial. Its supported calibration observations are in each camera's forward hemisphere, with positive camera z. Do not use it as a general solution for the rearward portions of 220-degree or 280-degree lenses. Such observations need a projection model and pose solver supporting signed axial directions. The code cannot infer unsupported rearward rays from pixels alone, so enforce this during capture.

Camera axes: x right, y down, z forward. All result matrices act on column vectors:

```text
X_right = R * X_left + T
baseline = norm(T)
camera2_center_in_camera1 = -transpose(R) * T
X_left = transpose(R) * X_right - transpose(R) * T
```

R is dimensionless. T and baseline use the declared square-size unit. T is NOT camera 2's centre expressed in camera 1; use the supplied `camera2_center_in_camera1` for that. Rotation matrices are provided directly; no ambiguous Euler-angle convention is imposed.

## Prerequisites and build

For container builds and utilities, see the [Docker guide](Job-007_cdx-fisheye-docker-guide.md). Container files are under `fisheye-stereo/docker`.

Use a C++17 compiler, CMake >=3.16 and OpenCV >=4.5 with core, calib3d, imgproc and imgcodecs. The HTTP dependency is vendored as cpp-httplib v0.18.1, including its MIT license.

For a typical macOS installation:

```sh
brew install cmake opencv
```

For Ubuntu/Debian:

```sh
sudo apt-get install build-essential cmake libopencv-dev
```

From this workspace's `fisheye-stereo` directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
```

If CMake cannot locate OpenCV, append `-DOpenCV_DIR=/path/to/directory/containing/OpenCVConfig.cmake` to configuration. On Windows, use an OpenCV C++ build matching the compiler and architecture; multi-configuration generators place executables under `build/Release` and need `--config Release` for build and tests.

## Image capture and corner identity

1. Rigidly mount the cameras. Lock focus, zoom and resolution. Both cameras must see the same board face.
2. Measure the checker square size. `board_cols` and `board_rows` count INNER intersections, not squares. A 10 by 7 square board has 9 by 6 inner corners.
3. Capture around 15-30 varied paired views, moving and tilting the board through the shared usable view. Both exposures must correspond to the same board pose; synchronize if there is motion.
4. Avoid changing image crop, rotation or scale between views. This example requires the same dimensions for both cameras and all images.
5. Keep a physically identifiable reference corner and row direction. Plain checkerboards have orientation ambiguity. Matching index numbers in the two images must identify the same physical points. A low fitted residual is not a substitute for this check.

The software enforces 6-100 pairs as a practical input policy, not as a claim that six views guarantee good calibration. Bad pose diversity can still cause an ill-conditioned solve. Detection uses `findChessboardCornersSB`; severe distortion may need a different detector or externally prepared corner observations.

## Command-line workflow

Copy and edit [examples/pairs.yml](../examples/pairs.yml). Image paths are resolved relative to that configuration file. The included paths are placeholders, not supplied camera images.

```yaml
%YAML:1.0
---
schema_version: 1
board_cols: 9
board_rows: 6
square_size: 30.0
unit: mm
pairs:
  - { left: "images/left01.png", right: "images/right01.png", reverse_left: 0, reverse_right: 0 }
  # Add every other paired view explicitly; at least six pairs are required.
```

Run detection:

```sh
./build/fisheye_stereo detect examples/pairs.yml observations.json previews
```

Inspect the numbered PNG previews. For each pair, corner 0 and every subsequent ID must identify the same physical corner in both cameras. `reverse_left: 1` or `reverse_right: 1` reverses that image's entire detected list, correcting a 180-degree ordering reversal. It does not fix arbitrary permutations. If you change flags, rerun with fresh output names. For square grids or unusual ordering, provide correctly ordered coordinates through the API instead. Order may differ between separate board poses, but must agree within each pair.

After reviewing the observations:

```sh
./build/fisheye_stereo calibrate observations.json result.json
```

Results contain K and D for both cameras, R, T, camera-centre position, baseline, view count and training reprojection RMS values in pixels. Output extensions `.json` and `.yml` select OpenCV FileStorage formats. The CLI refuses existing output files/directories. A failed detection may leave partial preview images; use a new preview directory when retrying. A failed pair aborts extraction rather than silently changing the dataset.

CLI exit codes: 0 success; 2 invalid arguments, dataset or missing image; 3 OpenCV parsing/detection/solver exception; 4 other runtime/I/O failure. Human-readable messages go to stderr. Successful calibration prints a summary to stdout and writes the structured result to the requested file.

## C++ library API

Header: [calibration.hpp](../include/calibration.hpp). Link the CMake target `stereo_calibration` from your own executable.

```cpp
#include "calibration.hpp"
#include <stdexcept>

cv::FileStorage input("observations.json", cv::FileStorage::READ);
if (!input.isOpened()) throw std::runtime_error("Cannot open observations");
stereo::Dataset data = stereo::readDataset(input.root());
stereo::Result result = stereo::calibrate(data);
double baseline = result.baseline; // data.unit
cv::Mat R = result.R;              // 3x3 CV_64F
cv::Mat T = result.T;              // 3x1 CV_64F
std::string json = stereo::resultJson(data, result);
```

`Dataset` accepts `vector<vector<Point2d>> left, right`, image size, board size, square size and unit. Each view must contain all inner corners in row-major board order: `(column * square_size, row * square_size, 0)`. The function validates finite in-image coordinates and dimensions. It creates board coordinates, calibrates each camera with recomputed extrinsics and fixed skew, then calls `fisheye::stereoCalibrate` with `CALIB_FIX_INTRINSIC`. Intrinsics are held fixed in the stereo stage. This version always estimates intrinsics from the submitted views; it does not accept pre-calibrated K/D.

Input errors throw `std::invalid_argument`; OpenCV failures throw `cv::Exception`; unexpected invalid solver output throws `std::runtime_error`. Catch these in embedding applications.

## HTTP REST protocol, version 1

Machine-readable contract: [OpenAPI 3.0 specification](../api/openapi.yaml).

Start the service in another terminal:

```sh
./build/fisheye_server 8080
```

It binds to `127.0.0.1` by default. Set `FISHEYE_BIND_ADDRESS=0.0.0.0` when forwarding a container port; the supplied Compose configuration publishes only on host loopback. It is a local synchronous example without TLS/authentication or job persistence. Stop with Ctrl-C. One calibration runs at a time; requests while busy get 429. A client disconnect does not cancel an active solve. The request-body limit is 4 MiB; the same 6-100-view limit applies. Requests contain numerical observations only, with no server-side file paths or image uploads.

### GET /health

```sh
curl --fail http://127.0.0.1:8080/health
```

Response: HTTP 200, `Content-Type: application/json`:

```json
{"status":"ok","schema_version":1}
```

### POST /v1/calibrate

Use the complete JSON file produced by `detect`:

```sh
curl --fail-with-body --max-time 300 \
  -H 'Content-Type: application/json' \
  --data-binary @observations.json \
  http://127.0.0.1:8080/v1/calibrate \
  -o result-http.json
```

Request fields:

| Field | Type and constraints |
|---|---|
| `schema_version` | Integer, exactly 1 |
| `image_width`, `image_height` | Integers in 1..32768; original pixel coordinates |
| `board_cols`, `board_rows` | Integers in 3..30; inner corners |
| `square_size` | Finite positive number; true square side length |
| `unit` | String `mm`, `cm`, or `m`; a label, not an automatic conversion |
| `views` | Array of 6..100 paired observation objects |
| `views[i].left`, `views[i].right` | Each is an array of exactly `board_cols * board_rows` `[x,y]` pixel pairs, in matched board order |

Unknown fields are ignored. Missing required fields, wrong types, nonfinite/out-of-image points and count mismatches are rejected. Reusing identical views or incorrectly associating corners is not fully detected by validation; dataset geometry remains the caller's responsibility.

Schematic request (abbreviated, not executable JSON):

```text
{
  "schema_version": 1,
  "image_width": 1280, "image_height": 960,
  "board_cols": 9, "board_rows": 6,
  "square_size": 30.0, "unit": "mm",
  "views": [
    {"left": [[x0,y0], ... 54 points], "right": [[x0,y0], ... 54 points]},
    ... at least 6 paired views
  ]
}
```

Success is HTTP 200 with a JSON object. All matrices are arrays of rows, including column vectors:

| Response field | Meaning / shape |
|---|---|
| `schema_version`, `model` | 1, `opencv_fisheye_kb4` |
| `transform` | `X_right = R * X_left + T` |
| `unit`, `square_size`, `board_cols`, `board_rows`, `image_width`, `image_height` | Input provenance |
| `views_used` | Number of paired views used |
| `K_left`, `K_right` | 3x3 intrinsic matrices; focal scales and centres in pixels |
| `D_left`, `D_right` | 4x1 columns, ordered k1, k2, k3, k4 |
| `R` | 3x3 relative rotation |
| `T` | 3x1 relative translation, e.g. `[[-120],[3],[4]]` |
| `camera2_center_in_camera1` | 3x1 vector `-R^T T` |
| `baseline` | Nonnegative norm of T in `unit` |
| `rms_left_px`, `rms_right_px`, `rms_stereo_px` | OpenCV's training reprojection RMS values |

Application errors use `{"error":{"code":"INVALID_INPUT","message":"..."}}`. Status codes:

| HTTP | Meaning |
|---|---|
| 400 | Malformed JSON or invalid input fields |
| 404 | Unknown endpoint |
| 413 | Request body exceeds 4 MiB |
| 415 | Content-Type is not application/json |
| 422 | OpenCV calibration failed, e.g. ill-conditioned geometry |
| 429 | Another calibration is running |
| 500 | Unexpected internal failure |

Transport failures before a complete HTTP response may not return JSON. A 200 response means the solver completed; it is not a metrology accuracy certificate. The service does not silently drop outliers or apply an arbitrary RMS acceptance threshold.

## Verification and practical accuracy

Verified on 2026-10-04 with AppleClang 17 and a locally built OpenCV 4.10.0. Both CTest suites passed: synthetic geometry and CLI/HTTP integration. The noiseless synthetic baseline was 120.104 mm, recovered as 120.104 mm; stereo training RMS was approximately 3.96e-12 pixels. This tiny residual reflects noiseless data generated by the same model, not achievable real-camera accuracy. Integration tests cover corner detection and preview output, overwrite refusal, successful HTTP calibration, malformed JSON, missing fields, wrong media type and unknown endpoints.

A complete [synthetic JSON request](../examples/synthetic-request.json) is included, so the HTTP service can be tried immediately with `--data-binary @examples/synthetic-request.json`. It is simulated test data, not a measurement of your cameras. Python 3 enables the optional CLI/HTTP CTest; the geometry test is C++ only.

The synthetic C++ test generates 24 board poses with known intrinsics, distortion, rotation and translation `[-120,3,4]` mm. It tests recovery of R/T/baseline, inverse pose, millimetre-to-metre scaling, JSON round-trip and invalid inputs. It uses noiseless points generated by the same OpenCV model, so it tests wiring and conventions, not physical accuracy or model suitability.

Generate a complete synthetic REST request without camera images:

```sh
./build/stereo_test synthetic-request.json
./build/fisheye_stereo calibrate synthetic-request.json synthetic-result.json
curl --fail-with-body -H 'Content-Type: application/json' \
  --data-binary @synthetic-request.json \
  http://127.0.0.1:8080/v1/calibrate
```

No real stereo image dataset was supplied. Before using the result for measurement, check separate test views, independent known distances, stability across subsets, and errors across viewing angles. Printed square-size error directly scales the recovered baseline. Lack of overlap, moving mounts, unsynchronized motion, noncentral lenses and unmodelled rearward rays require changes to acquisition or modelling.

The example is intended for a rigid pair with sufficient shared field of view. Back-to-back cameras without a common visible board need a different calibration setup, such as a rigid multi-face target with known geometry.

## Dependencies and references

- [OpenCV fisheye model and C++ APIs](https://docs.opencv.org/4.10.0/db/d58/group__calib3d__fisheye.html)
- [OpenCV calibration and checkerboard detection APIs](https://docs.opencv.org/4.10.0/d9/d0c/group__calib3d.html)
- [cpp-httplib v0.18.1 documentation](https://github.com/yhirose/cpp-httplib/tree/v0.18.1)
- [Vendored HTTP library license](../third_party/httplib-LICENSE)

The vendored `httplib.h` SHA-256 is `f870541e065de607c8a7f64fc8743b475cd1388fff5238afa8841f75dcbf2b89`.
