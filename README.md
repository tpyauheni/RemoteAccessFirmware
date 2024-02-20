# Remote Access Firmware

Firmware for WeMos D1 R1 that:
1. Reads key presses from 4x4 matrix keyboard to activate/deactivate some functions.
2. Reads source code, sent by MQTT protocol.
3. Compiles source code to bytecode and transfers it via serial port.

To execute compiled bytecode on computer, see [tpyauheni/RemoteAccessSoftware](https://github.com/tpyauheni/RemoteAccessSoftware). To control connected computers via MQTT protocol, see [tpyauheni/RemoteAccessController](https://github.com/tpyauheni/RemoteAccessController)

## Building

To build this repository, simply install Platform IO and upload firmware via serial port to connected WeMos D1 R1.
