FROM ubuntu:18.04

ENV DEBIAN_FRONTEND noninteractive
# Core Linux Deps
RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y --fix-missing --no-install-recommends \
    apt-utils                        \
    build-essential                  \
    cmake                            \ 
    git                              \
    ca-certificates                  \
    libgtk2.0-dev                    \ 
    pkg-config                       \ 
    libavcodec-dev                   \ 
    libavformat-dev                  \ 
    libswscale-dev                   \
    imagemagick                   && \
    apt-get clean                 && \
    rm -rf /var/lib/apt/lists/*   && \
    apt-get clean && rm -rf /tmp/* /var/tmp/*
ENV DEBIAN_FRONTEND noninteractive

RUN git clone --branch 4.4.0 --depth 1 https://github.com/opencv/opencv.git && \
    cd opencv && mkdir build && cd build && \
    cmake                                   \
        -D CMAKE_BUILD_TYPE=RELEASE         \
        -D CMAKE_INSTALL_PREFIX=/usr/local  \
        -D INSTALL_C_EXAMPLES=OFF           \
        -D INSTALL_PYTHON_EXAMPLES=OFF      \
        -D BUILD_TESTS=OFF                  \
        -D BUILD_PERF_TESTS=OFF             \
        -D BUILD_EXAMPLES=OFF               \
        -D BUILD_DOCS=OFF                   \
        -D OPENCV_GENERATE_PKGCONFIG=ON     \
        ..           && \
    make -j $(nproc) && \
    make install     && \
    ldconfig         && \
    rm -rf /opencv

COPY ./src /opt/src

RUN cd /opt/src                     && \
    mkdir build                     && \
    cd build                        && \
    cmake ..                        && \
    make -j $(nproc)                && \
    make install                    && \
    ldconfig