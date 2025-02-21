FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y vim git tar make cmake dpkg \
    libxrender1 libxcb-render0 libxcb-render-util0 libxcb-shape0 \
    libxcb-randr0 libxcb-xfixes0 libxcb-sync1 libxcb-shm0 \
    libxcb-icccm4 libxcb-keysyms1 libxcb-image0 libxkbcommon0 \
    libxkbcommon-x11-0 libx11-xcb1 libsm6 libice6 libglib2.0-0 && \
    rm -rf /var/lib/apt/lists/*

CMD ["bash"]