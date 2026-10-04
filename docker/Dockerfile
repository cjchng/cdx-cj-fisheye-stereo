FROM debian:bookworm-slim AS build
ARG BUILD_JOBS=2
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ cmake make libopencv-dev python3 ca-certificates \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /opt/project
COPY CMakeLists.txt ./
COPY include/ include/
COPY src/ src/
COPY third_party/ third_party/
COPY tests/ tests/
RUN cmake -S . -B /opt/build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build /opt/build --parallel "${BUILD_JOBS}"

FROM build AS test
RUN useradd --uid 10001 --create-home app && chown -R app:app /opt/build
USER 10001:10001
CMD ["ctest", "--test-dir", "/opt/build", "--output-on-failure"]

FROM debian:bookworm-slim AS runtime
RUN apt-get update && apt-get install -y --no-install-recommends \
    libopencv-calib3d406 libopencv-imgcodecs406 curl ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --uid 10001 --create-home app
COPY --from=build /opt/build/fisheye_server /opt/build/fisheye_stereo /usr/local/bin/
COPY examples/ /app/examples/
COPY api/ /app/api/
COPY third_party/httplib-LICENSE /app/licenses/httplib-LICENSE
ENV FISHEYE_BIND_ADDRESS=0.0.0.0
WORKDIR /app
USER 10001:10001
EXPOSE 8080
HEALTHCHECK --interval=15s --timeout=3s --start-period=10s --retries=3 \
    CMD curl --fail --silent http://127.0.0.1:8080/health || exit 1
CMD ["fisheye_server", "8080"]
