FROM ubuntu:22.04

# Install compiler, cmake, headers
RUN apt-get update && apt-get install -y \
    build-essential cmake linux-libc-dev git vim && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Default build
RUN mkdir -p build && cd build && cmake .. && make

CMD ["/bin/bash"]
