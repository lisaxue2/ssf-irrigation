# Soil Irrigation Web App

**Background**

The web app monitors soil irrigations using a Wireless Sensor Network(WSN) built with many ESP8266s microcontrollers. The ESPs are nodes in my network that use radio communication(ESP-NOW protocol) and interface with soil sensors, gathering data on soil moisture. The user can then access and visualize the data from a web page that is built on a web server capable of receiving and storing data from the WSN and handling user requests.

**Problems Addressed**

- Inefficient water use due to irrigation being monitored with human error and little precision
- Lack of Wi-Fi in the field


**The Hardware** 

The WSN communicates packets of data across the field and to the web server. The ESPs nodes on the network are subjected to 4 roles:

Field Sensors- multiple ESPs interfacing with capacitative soil moisture sensors that form a star topology with the Terminal and execute many-to-one message delivery

Terminal- one ESP that retrieves sensor data from all Field nodes in the form of an array

Relays- multiple ESPs that send data retrieved from the Terminal across longer distances

Gateway- the final ESP in the WSN that retrieves data from the last Relay and delivers it to the web server

**The Backend**  

A Raspberry Pi computer hosts the Python Flask web server which receives the WSN's data through its serial port from the Gateway node. The server parses and stores the data into an external datastore. The server handles requests so the user can monitor irrigation from a web browser.

**The Frontend** 

A web page is rendered when a user makes a HTTP request to the server from a web browser. It displays visualizations of current soil moisture data in the form of gauge charts and long-term data in the form of line charts. These charts are implemented from Google Charts javascript code.
