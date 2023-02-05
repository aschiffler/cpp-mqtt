# Using paho c++ MQTT Client in a CMake project
* Paho libaries (c, c++) are linked statically
* OpenSSL and Standard C libaries are linked dynamically

## Start Coding in the Browser

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=master&repo=593240255&machine=basicLinux32gb&devcontainer_path=.devcontainer%2Fdevcontainer.json&location=WestEurope)

Within the codespace instance a mosquitto MQTT broker is started automatically for testing. See folder test for configuration and folder .devcontainer for start-up commands.

(_You will need a github account to do so_)


## Notes
* CMake is set up only for compiling and running on Linux OS. To cross-build and/or run on Windows modifications are necessary for the linked libraries.