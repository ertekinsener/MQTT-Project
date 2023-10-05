# mqtt_project

## Requirements
This project is a simple code for publishing Arduino events via a locally running MQTT Broker.

- CMake GUI v3.5 or newer
- Arduino IDE
- Visual Studio 2015 or newer
- [Eclipse Paho MQTT C++ Client Library](https://github.com/eclipse/paho.mqtt.cpp#eclipse-paho-mqtt-c-client-library)
- [microsoft/vcpkg](https://github.com/microsoft/vcpkg)
- Python 3.6 or newer
- paho-mqtt 1.6.1

### Installation
1. Clone the Repository: 
git clone https://github.com/ertekinsener/mqtt_project.git
2. Navigate to the project folder: 
cd mqtt_project
3. Install libraries as described on their respective GitHub pages.
- To install the Paho MQTT C++ library, run:
  ```
  vcpkg install paho-mqttpp3:x64-windows
  ```
- To install paho-mqtt 1.6.1 library, use pip:
  ```
  pip install paho-mqtt
  ```
 ### Arduino Code

 Arduino code is writen in C++ and bare metal.

 

