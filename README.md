# cv-guided-esp32-robot

A lightweight ESP32 robot project that combines an ESP32 camera server and an ESP32-based robot motion controller.

## Overview

This workspace contains two main subsystems:

- `src/camera_control`: ESP32 camera firmware and web server code for live video streaming.
- `src/car_control`: a Python control interface plus the ESP32 UDP driver code for robot movement and a servo.

## What is included

### Camera control

- `src/camera_control/Camera_updated.ino`
  - ESP32 camera firmware using the Arduino ESP32 camera libraries.
  - Configures the camera and connects to WiFi using station mode.
  - Starts a web server so the camera stream can be accessed from a browser.
- `src/camera_control/board_config.h`
  - Select your camera board model here.
  - Default is `CAMERA_MODEL_AI_THINKER`.
- `src/camera_control/camera_pins.h`
  - Pin mappings for the selected camera board.

### Car control

- `src/car_control/car_control/car_control_node.py`
  - Simple Python UDP controller for sending drive commands to the ESP32 robot.
  - Sends commands to `192.168.4.1:4210` by default.
- `src/car_control/car_control/esp32_driver_code/esp32_driver_code.ino`
  - ESP32 firmware for the robot motion controller.
  - Uses WiFi SoftAP mode and reads UDP commands to drive motors and move a servo.
- `src/car_control/package.xml` and `src/car_control/setup.py`
  - ROS package metadata and Python packaging boilerplate.

## How it works

1. The camera ESP32 connects to an existing WiFi network in station mode.
2. The camera server publishes a web interface for viewing the video stream.
3. The robot ESP32 creates a WiFi access point and listens on UDP port `4210`.
4. The Python controller sends single-character commands to the robot ESP32.

## Command mapping

- `w`: forward
- `s`: backward
- `a`: spin left
- `d`: spin right
- `x`: stop
- `u`: servo 180°
- `i`: servo center (90°)
- `j`: servo 0°
- `q`: quit controller

## Setup

### Camera firmware

1. Open `src/camera_control/Camera_updated.ino` in the Arduino IDE or PlatformIO.
2. Open `src/camera_control/board_config.h` and uncomment the correct `CAMERA_MODEL_*` for your board.
3. Set your WiFi SSID and password in `Camera_updated.ino`:
   - `ssid`
   - `password`
4. Select an ESP32 board with PSRAM if using high-resolution streaming.
5. Upload the sketch to the ESP32 camera board.

### Robot motion firmware

1. Open `src/car_control/car_control/esp32_driver_code/esp32_driver_code.ino` in the Arduino IDE or PlatformIO.
2. Update any motor or servo pins if needed.
3. Upload the sketch to the ESP32 robot controller.

### Running the Python controller

1. Connect your laptop to the ESP32 robot access point:
   - SSID: `esp32`
   - Password: `12345678`
2. Run the controller script:

```bash
python3 src/car_control/car_control/car_control_node.py
```

3. Enter commands at the prompt to move the robot.

## Notes

- The camera firmware uses `PIXFORMAT_JPEG` and the stream is optimized for PSRAM-equipped ESP32 modules.
- The robot firmware currently uses hardcoded motor pin assignments and the `ESP32Servo` library.
- The ROS package metadata in `src/car_control` is a placeholder and can be updated with your package name and description.

## File structure

- `src/camera_control/`
  - `Camera_updated.ino`
  - `app_httpd.cpp`
  - `board_config.h`
  - `camera_index.h`
  - `camera_pins.h`
  - `ci.yml`
  - `partitions.csv`
- `src/car_control/`
  - `package.xml`
  - `setup.py`
  - `car_control/`
    - `car_control_node.py`
    - `esp32_driver_code/esp32_driver_code.ino`

## License

Update this README with your project license and contributors as needed.
