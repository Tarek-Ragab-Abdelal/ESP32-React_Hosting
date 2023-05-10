## Overview

- This project is basicly a web hosting using ESP32, what is special that the web app is creating using react.js
- This is done using python scripts that build and compresses the exported files to "gzip" format.
- These files are then needs to be uploaded to the filesystem of ESP32 chip using littleFS.
- WebSockets connection takes place to send data bidirectionally between the client and the ESP32 server.

## Environment Dependencies

1. [VSCode](https://code.visualstudio.com/) or [VSCodium](https://vscodium.com/)
2. [Platform IO extension](https://platformio.org/install/ide?install=vscode)
3. [Node.js](https://nodejs.org/en/)
4. Platform to be on of the following (Due to "mklittlefs")
   - Darwin_amd64
   - Linux_aarch64
   - Linux_amd64
   - Linux_arm
   - Windows_amd64
   - Windows_i386

## Proceudres of running the project

1. Go to "src/main.cpp" and edit the defined SSID and Password to yours.
2. Upload the code
   - Platform IO -> Project Tasks -> esp32dev -> General -> Upload
3. Upload the React Web App
   - Platform IO -> Project Tasks -> esp32dev -> Platform -> Upload Filesystem Image
