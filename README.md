# Plant Care System

## Overview
This plant care system is designed to monitor and adjust the environment of your plants automatically. Utilizing temperature and humidity sensors, it ensures that your plants are kept within optimal growth conditions. When the environment deviates from the set parameters, the system can cool the area with a fan or hydrate the plants by spraying water. All environmental data is sent to an online dashboard for real-time monitoring.

## Features
- **Temperature Monitoring**: Measures the ambient temperature to ensure it stays below a set threshold.
- **Humidity Monitoring**: Checks the humidity level to ensure it remains above a set lower limit.
- **Automatic Water Spraying**: Activates a spray mechanism if the humidity drops too low.
- **Air Circulation**: Turns on a fan if the temperature exceeds the set limit.
- **Real-Time Dashboard**: Sends temperature and humidity data to an online dashboard for monitoring.
- **Plant Presence Detection**: Utilizes an ultrasonic sensor to detect if a plant is in front of the device.

## Hardware Requirements
- Arduino or compatible microcontroller
- DHT11 temperature and humidity sensor
- Servo motor for spray mechanism
- DC motor for fan
- Ultrasonic sensor for plant detection
- LEDs for status indication
- WiFi module (ESP8266 used here for internet connectivity)
- Miscellaneous: resistors, wires, breadboard, etc.

## Software Requirements
- Arduino IDE for programming
- DHT library for temperature and humidity sensing
- Servo library for controlling the servo motor
- WiFiEsp library for WiFi connectivity
- PubSubClient library for MQTT communication

## Setup Instructions
1. **Hardware Assembly**:
   - Connect the DHT11 sensor, servo motor, DC motor, ultrasonic sensor, LEDs, and WiFi module to the Arduino as per the pin configuration defined in the code.
2. **Software Configuration**:
   - Install the required libraries through the Arduino IDE's Library Manager.
   - Replace `ssid`, `pass`, `IO_USERNAME`, and `IO_KEY` with your WiFi credentials and Adafruit IO credentials respectively.
   - Upload the provided code to your Arduino.
3. **Dashboard Setup**:
   - Create an account on Adafruit IO (or similar MQTT service).
   - Set up feeds for temperature and humidity.
   - Create a dashboard and add blocks to visualize the temperature and humidity data.

## Usage
- Place the plant in front of the device. The system will automatically detect the presence of the plant.
- The system measures the temperature and humidity at regular intervals.
- If the temperature exceeds the set limit, the fan will turn on to cool the area.
- If the humidity drops below the set limit and the cooldown period has passed, the system will activate the spray to moisten the area.
- The green LED indicates that the system is actively monitoring and adjusting the environment. The red LED indicates that no plant is detected.
- All sensor readings are published to the online dashboard for real-time monitoring.

## Contributing
We welcome contributions and suggestions! Please open an issue or pull request on our GitHub repository.

## License
This project is open source and available under the MIT License.
