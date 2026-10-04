#!/bin/sh
set -eu
docker_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
# Changing to the script directory makes relative data paths and .env consistent.
cd "$docker_dir"
compose() { docker compose -f "$docker_dir/compose.yaml" "$@"; }
help() {
    cat <<'EOF'
Usage: ./docker/manage.sh COMMAND [ARGS...]
  build             Build the runtime image
  up                Build and start API; wait for a healthy container
  down              Remove this Compose project's containers and network
  status            Show service state
  logs              Follow API logs (Ctrl-C leaves the service running)
  health            Check health inside the API container
  smoke             POST the synthetic example inside the API container
  test              Build and run C++ and CLI/HTTP tests in Linux
  cli ARGS...       Run calibration CLI with /data mounted, as your UID/GID
  shell [ARGS...]   Open a disposable shell (or use -c 'command') with /data
  config            Print resolved Compose configuration
  help              Show this help

Settings: docker/.env or exported FISHEYE_HTTP_PORT, FISHEYE_DATA_DIR,
BUILD_JOBS, COMPOSE_PROJECT_NAME. Default data folder: docker/data.
EOF
}
command=${1:-help}
if [ "$#" -gt 0 ]; then shift; fi
case "$command" in
    build) compose build api ;;
    up) compose up --build --detach --wait --wait-timeout 90 api ;;
    down) compose down ;;
    status) compose ps --all ;;
    logs) compose logs --follow api ;;
    health) compose exec -T api curl --fail --silent --show-error http://127.0.0.1:8080/health ;;
    smoke) compose exec -T api curl --fail-with-body --silent --show-error --max-time 180 \
        -H 'Content-Type: application/json' --data-binary @/app/examples/synthetic-request.json \
        http://127.0.0.1:8080/v1/calibrate ;;
    test) compose build tests; compose run --rm --no-deps tests ;;
    cli|shell)
        # Compose creates a missing bind directory itself. For the default path,
        # create it here so the host user owns it, including on Linux.
        mkdir -p "$docker_dir/data"
        compose run --rm --no-deps --user "$(id -u):$(id -g)" "$command" "$@"
        ;;
    config) compose config ;;
    help|--help|-h) help ;;
    *) help >&2; exit 2 ;;
esac
