FROM mcr.microsoft.com/devcontainers/universal:2-linux

RUN apt-get update && apt-get install -y \ 
    mingw-w64 \
    apt-get clean autoclean && apt-get autoremove --yes && rm -rf /var/lib/{apt,dpkg,cache,log}/
