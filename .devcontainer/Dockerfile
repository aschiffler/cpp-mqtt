FROM mcr.microsoft.com/devcontainers/universal:2-focal

RUN apt-key adv --keyserver hkps://keyserver.ubuntu.com --refresh-keys
RUN apt-get update && apt-get install -y \ 
    mingw-w64 \
    && apt-get clean autoclean && apt-get autoremove --yes && rm -rf /var/lib/{apt,dpkg,cache,log}/
