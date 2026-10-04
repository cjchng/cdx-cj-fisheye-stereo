# Fisheye stereo app: Docker and container utilities

Container files live in [fisheye-stereo/docker](../docker/). They provide a multi-stage Debian Bookworm image, Docker Compose services, and a shell utility for building, running, checking and testing the app. Docker Engine or Docker Desktop with Compose v2 is required. No host OpenCV installation is needed.

## Start the API

Run from the `fisheye-stereo` source directory:

```sh
./docker/manage.sh up
./docker/manage.sh health
./docker/manage.sh smoke
```

`up` builds the image, starts the API in the background and waits for its health check. `smoke` submits the included synthetic corner dataset. The synthetic baseline should be approximately 120.104 mm. This is a software check, not a physical camera measurement.

Check the published port from your host:

```sh
curl --fail http://127.0.0.1:8080/health
curl --fail-with-body --max-time 180 \
  -H 'Content-Type: application/json' \
  --data-binary @examples/synthetic-request.json \
  http://127.0.0.1:8080/v1/calibrate
```

Both `health` and `smoke` utility commands run curl inside the API container. The host commands above additionally exercise Docker's published port.

## Utilities

| Command | Purpose |
|---|---|
| `./docker/manage.sh build` | Build the runtime image without starting the API |
| `./docker/manage.sh up` | Build and start API, waiting for health |
| `./docker/manage.sh status` | Show this project's containers |
| `./docker/manage.sh logs` | Follow API logs; Ctrl-C stops following |
| `./docker/manage.sh health` | GET /health inside the API container |
| `./docker/manage.sh smoke` | POST the synthetic example inside the API container |
| `./docker/manage.sh test` | Build the test image and run both CTest suites in Linux |
| `./docker/manage.sh cli --help` | Show CLI syntax in a disposable container |
| `./docker/manage.sh shell` | Open a disposable shell with data mounted |
| `./docker/manage.sh config` | Print resolved Compose configuration |
| `./docker/manage.sh down` | Remove this project's containers and network |

`down` preserves local data and Docker images. No global prune command is used. CLI/shell commands assume the runtime image was built first. Their containers are removed on exit. In PowerShell, use the direct Compose commands below; the helper requires a POSIX shell such as Bash, zsh or WSL.

## Run the calibration CLI on host files

The default writable mount is `docker/data` on the host, exposed as `/data` inside CLI/shell containers. The helper creates the default directory and runs these utilities with your host UID/GID to avoid root-owned output files on Linux.

Try a synthetic calibration:

```sh
./docker/manage.sh build
./docker/manage.sh cli calibrate /app/examples/synthetic-request.json /data/result.json
```

Read the output at `docker/data/result.json`. The calibration tool refuses to overwrite it; choose a new filename for another run.

For real images, arrange files as:

```text
docker/data/
  pairs.yml
  images/
    left01.png
    right01.png
    ...
```

Copy the structure from `examples/pairs.yml` and enter the board dimensions, actual square size and image pairs. Paths in `pairs.yml` are relative to that file.

```sh
./docker/manage.sh cli detect /data/pairs.yml /data/observations.json /data/previews
# Inspect numbered corner previews on the host before calibration.
./docker/manage.sh cli calibrate /data/observations.json /data/result-real.json
```

Use fresh output filenames and preview folders when repeating detection. A plain checkerboard can have ambiguous corner order; follow the [calibration guide](Job-006_cdx-fisheye-stereo-guide.md) before accepting the result. The model still requires supported forward-hemisphere observations; Docker does not extend the lens model.

## Configuration

Optionally copy `docker/.env.example` to `docker/.env` and edit it:

```text
FISHEYE_HTTP_PORT=8080
BUILD_JOBS=2
FISHEYE_DATA_DIR=/absolute/path/to/existing/calibration-data
COMPOSE_PROJECT_NAME=fisheye-stereo
```

The last two entries are optional. Create a custom data directory yourself and ensure your host user can write it. Paths with spaces should be quoted in `.env`. Exported environment variables override `.env` settings. Use the same settings for up, status, logs and down so they address the same project.

For example, to use a different host port:

```sh
export FISHEYE_HTTP_PORT=18080
./docker/manage.sh up
curl --fail http://127.0.0.1:18080/health
./docker/manage.sh down
```

The application listens on port 8080 on all container interfaces; Compose publishes it only on the host's 127.0.0.1 address. Native execution still defaults to loopback. The new server environment variable `FISHEYE_BIND_ADDRESS` accepts only `127.0.0.1` or `0.0.0.0`; its container value is `0.0.0.0`. The API remains unauthenticated plain HTTP for local use.

## Direct Docker Compose commands

These commands work from the source directory. The helper also consistently loads `docker/.env`; specify it explicitly for the direct form if you created one.

```sh
docker compose -f docker/compose.yaml build api
docker compose -f docker/compose.yaml up -d --wait api
docker compose -f docker/compose.yaml logs -f api
docker compose -f docker/compose.yaml build tests
docker compose -f docker/compose.yaml run --rm tests
docker compose -f docker/compose.yaml run --rm --user "$(id -u):$(id -g)" cli --help
docker compose -f docker/compose.yaml down
```

The `cli`, `shell` and `tests` services have a tools profile. Naming a service directly in `run`/`build` enables it without starting the other tools. To inspect all services with `config`, add `--profile tools` before `config`.

Build and run without Compose:

```sh
docker build --target runtime -f docker/Dockerfile -t fisheye-stereo:local .
docker run --rm --init -p 127.0.0.1:8080:8080 fisheye-stereo:local
```

Stop that foreground container with Ctrl-C. Compose adds read-only root filesystems, temporary `/tmp`, and restricted capabilities for API/CLI/shell services; a bare `docker run` command does not apply those Compose settings automatically.

## Image contents and dependencies

- `build` stage: C++ compiler, CMake, Debian OpenCV development packages, Python and sources.
- `test` stage: build artifacts plus the existing C++ and Python integration tests, running as UID 10001.
- `runtime` stage: server and CLI executables, OpenCV runtime libraries, curl, sample input and OpenAPI specification. Runs as UID 10001 by default.

The build context is the app directory, not the entire research workspace. `.dockerignore` excludes host builds, data, outputs, `.env` and caches. No privileged mode, host network, Docker socket mount or GPU is required. The image uses the host Docker platform by default; cross-architecture builds are not separately verified. Debian supplies OpenCV 4.6, whereas the original host verification used OpenCV 4.10.

Base image and apt package updates can change future builds. This is a version-family-pinned development image, not a digest-and-package-locked archival build. The build needs access to Docker Hub and Debian package mirrors.

## Troubleshooting

- Docker daemon unavailable: start Docker Desktop or the Engine, then retry.
- Port occupied: choose a different `FISHEYE_HTTP_PORT`.
- Host output permissions: use `manage.sh cli` or pass your UID/GID to Compose, and check custom data-directory ownership.
- Input/output paths: the container sees `/data`, not arbitrary host paths. Mount the required directory first.
- Calibration error: inspect board corner identities and pose diversity; see the original guide. Container health only checks the HTTP process.
- Build dependency/network failure: retry after connectivity is restored; do not treat a failed build as a tested deployment.

## Verification status (2026-10-04)

Docker Engine 28.3.0 and Compose 2.38.1 were available. Shell syntax and resolved Compose configuration passed checks, including all four services, host-loopback port publication, container bind address, data mount and test target. The updated native C++ app compiled and both existing CTest suites passed; invalid bind addresses were rejected.

The Docker build downloaded the base image and completed the runtime dependency layer. Development-package downloads then slowed substantially; a separate Debian package download check timed out. The verification build was stopped before compilation, so the final image, Linux CTest execution, published API port and mounted CLI output are **not yet verified**. No application container was left running. Completed build layers remain in Docker's normal cache; an interrupted apt layer may need to download its packages again.

To finish verification when network throughput recovers, run `./docker/manage.sh build`, `./docker/manage.sh test`, `./docker/manage.sh up`, the host curl commands above, and a CLI calibration into `/data`, then `./docker/manage.sh down`.

References: [Docker build specification](https://docs.docker.com/reference/compose-file/build/), [Compose services](https://docs.docker.com/reference/compose-file/services/), [Dockerfile reference](https://docs.docker.com/reference/dockerfile).
