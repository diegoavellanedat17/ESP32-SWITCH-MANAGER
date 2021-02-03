# ESP32-SWITCH-MANAGER

This is a robust way in which you can have your hardware connected to the Internet and working with a physical asynchronous switch and handling reconnections in different scenarios.

## Feautures 


With my experience in developing IoT projects I have realized that it is necessary to handle asynchronous tasks. For example, in the case of a switch, it is necessary that the main task (power on and off) can continue to be handled physically and not depend on the reconnections or reset states of the WiFI device, in this case the ESP32.


It's for this reason that I designed a system that has a UART communication between the ESP32 and the ATMEGA328P.

We connect to an already known MQTT broker as clients with ESP32, this is in charge of sending and receiving messages to ATMEGA328P through the UART interface.

In this repository is the firmware that allows the communication between the ATMEGA329P microcontroller and the ESP32 as well as the connections with the MQTT broker for publication and subscription.
