FROM ubuntu:20.04

# Install dependencies

RUN apt-get update && \
        DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get install -y \
            g++-arm-linux-gnueabi \
            build-essential \
            git ca-certificates gpg wget

RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null
RUN apt-get update && apt-get install -y kitware-archive-keyring cmake ninja-build
            
ENTRYPOINT ["/bin/bash"]


