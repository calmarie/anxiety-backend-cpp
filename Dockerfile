FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

WORKDIR /app

RUN apt-get update && apt-get install -y libargon2-dev && rm -rf /var/lib/apt/lists/*

COPY cmake/ cmake/
COPY CMakeLists.txt ./
COPY configs/ configs/
COPY src/ src/

RUN cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug \
 && cmake --build build-debug -j$(nproc)

EXPOSE 8080

ENTRYPOINT ["./build-debug/anxiety_backend"]
CMD ["--config", "./configs/static_config.yaml", "--config_vars", "./configs/config_vars.docker.yaml"]


