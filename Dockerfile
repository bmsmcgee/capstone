FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y vim git tar make cmake dpkg && \
    rm -rf /var/lib/apt/lists/*

CMD ["bash"]