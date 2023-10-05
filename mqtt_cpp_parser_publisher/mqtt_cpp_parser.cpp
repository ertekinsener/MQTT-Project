#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <mqtt/async_client.h>
#include <windows.h>

// MQTT Broker
const std::string mqttBrokerAddress = "tcp://localhost:1883";

void readAndPublishSerialData(mqtt::async_client &mqttClient, HANDLE comPort)
{
    // COM port
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(comPort, &dcbSerialParams))
    {
        std::cerr << "Error getting COM port state." << std::endl;
        CloseHandle(comPort);
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;

    if (!SetCommState(comPort, &dcbSerialParams))
    {
        std::cerr << "Error setting COM port state." << std::endl;
        CloseHandle(comPort);
        return;
    }

    // buffer to read serial data
    const int bufferSize = 256;
    char buffer[bufferSize];

    while (true)
    {
        DWORD bytesRead;
        std::string receivedData;

        // Read characters one by one until a newline character is received
        char c;
        do
        {
            if (!ReadFile(comPort, &c, 1, &bytesRead, NULL))
            {
                DWORD error = GetLastError();
                if (error == ERROR_IO_PENDING)
                {
                }
                else
                {
                    std::cerr << "Error reading from COM port. Error code: " << error << std::endl;
                    break;
                }
            }
            else
            {
                if (bytesRead > 0)
                {
                    receivedData += c;
                }
            }
        } while (c != '\n');

        if (!receivedData.empty())
        {
            int systemState;
            std::string ledState1, ledState2;
            int buttonPressCount;
            std::stringstream ss(receivedData);
            char delimiter = ',';
            ss.ignore(2); // Ignore the "$$"
            ss >> systemState;
            ss.ignore(1); // Ignore the ","
            std::getline(ss, ledState1, ',');
            std::getline(ss, ledState2, ',');
            ss >> buttonPressCount;

            // Publish parsed fields to MQTT topics and write them to console
            mqtt::delivery_token_ptr pubTok;

            pubTok = mqttClient.publish("/system_state", std::to_string(systemState), 0, false);
            std::cout << "Published: /system_state - " << std::to_string(systemState) << std::endl;

            pubTok = mqttClient.publish("/led_state_1", ledState1, 0, false);
            std::cout << "Published: /led_state_1 - " << ledState1 << std::endl;

            pubTok = mqttClient.publish("/led_state_2", ledState2, 0, false);
            std::cout << "Published: /led_state_2 - " << ledState2 << std::endl;

            pubTok = mqttClient.publish("/button_press_count", std::to_string(buttonPressCount), 0, false);
            std::cout << "Published: /button_press_count - " << std::to_string(buttonPressCount) << std::endl;
        }
    }
}

int main()
{
    // Initialize the MQTT client
    mqtt::async_client mqttClient(mqttBrokerAddress, "");

    // Connect to the MQTT broker
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try
    {
        mqttClient.connect(connOpts);
    }
    catch (const mqtt::exception &exc)
    {
        std::cerr << "Error connecting to the MQTT broker: " << exc.what() << std::endl;
        return 1;
    }

    LPCWSTR comPortName = L"COM3";

    HANDLE comPort = CreateFile(comPortName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (comPort == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error opening COM port." << std::endl;
        return 1;
    }

    readAndPublishSerialData(mqttClient, comPort);

    // Clean up and disconnect
    CloseHandle(comPort);
    mqttClient.disconnect();

    return 0;
}
