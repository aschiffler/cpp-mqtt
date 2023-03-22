# Using paho c++ MQTT Client in a CMake project
* Paho libaries (c, c++) are linked statically
* OpenSSL and Standard C libaries are linked dynamically

## Start Coding in the Browser

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=master&repo=593240255&machine=basicLinux32gb&devcontainer_path=.devcontainer%2Fdevcontainer.json&location=WestEurope)

Within the codespace instance a mosquitto MQTT broker is started automatically for testing. See folder test for configuration and folder .devcontainer for start-up commands.

(_You will need a github account to do so_)

## Use Codespace to compile and develop
After the codespace instance has started it ask you to choose your tool chain. Click von the marked entry

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img01.png width=50%>

Then you may recognize that in one of the console windows the log output of a mosquitto broker can be observed. The configuration of this instance is taken from the file [./test/mosquitto.conf](https://github.com/aschiffler/cpp-mqtt/blob/3bd354dac157c1e4dabcb64a730dda1329b4c63f/test/mosquitto.conf#L1)

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img02.png width=50%>

To be ready for compilation you have to choose the configuration from the bottom menu. Choose either Degub or Release.

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img03.png width=50%>

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img04.png width=50%>

Then click on "Run" which will first ask for the build target choose 'cpp-mqtt'

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img05.png width=50%>

The build process will first fetch the git submodules for the Paho C/C++ client libaries, build the application and finaly create a self-signed certififcate for the use of TLS.

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img06.png width=50%>

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img07.png width=50%>

You can then see the sample application running and watch the different print outs

<img src=https://github.com/aschiffler/cpp-mqtt/raw/main/doc/img08.png width=100%>

## Build local
If you prefer to see and use the sample code on your own environment on a local PC clone the source code from here and proceed like this.

```bash
git clone https://github.com/aschiffler/cpp-mqtt.git
cd cpp-mqtt
# create makefiles
cmake -S ./ -B ./build -G Ninja
# yes run twice due to git submodule init for paho client libraries
cmake -S ./ -B ./build -G Ninja
# build
cmake  --build ./build --target all --
```
This will create an executable application named cpp-mqtt in the build folder.

## Blog post
Read here the corresponding blog post:
[https://cedalo.com/blog/implement-paho-mqtt-c-cmake-project/](https://cedalo.com/blog/implement-paho-mqtt-c-cmake-project/)

## Notes
* CMake is set up only for compiling and running on Linux OS. To cross-build and/or run on Windows modifications are necessary for the linked libraries.
