# Farm Flask: An IoT-Enabled Irrigation System for The Sustainable Student Farm

**Background**

The web app monitors soil irrigation using a Wireless Sensor Network (WSN) built upon many Arduino-ESP8266 microcontrollers. The Arduino-ESPs are nodes in my network that use radio communication (ESP-NOW protocol) and interface with soil sensors gathering data on soil moisture. The user can visually monitor irrigation from a webpage that's powered by a Python Flask server capable of receiving and storing data from the WSN and handling user requests.

**Problems Addressed**

- Inefficient water use due to irrigation being monitored with human error and low precision
- Transmitting data on a field that lacks Wi-Fi

**The Hardware** 

The hardware component is the WSN, which communicates packets of sensor data across the field and to the web server. Each node on the WSN consists of an Arduino-ESP8266 development board, a solar panel powered by a lithium ion battery, and a capacitative moisture sensor if it is a Field node. 
The nodes are subjected to 4 distinct roles:

Field - multiple ESPs interfacing with sensors and form a star topology with the Terminal to preform many-to-one packet delivery

Terminal - a single ESP that receives sensor data from all Field nodes in the form of an array and delivers it to a Relay

Relay - multiple ESPs that deliver packets across longer distances (150-200 m); one Relay receives data from the Terminal, delivers it to the next Relay, and the data hops between every Relay until it reaches the Gateway

Gateway - the final ESP in the WSN that receives data from the last Relay and delivers it to the web server

**The Backend**  

The backend component consists of a Raspberry Pi computer that hosts a Python Flask web server. The server receives the WSN's data from the Gateway node in the form of an array through its serial port. The server generates a timestamp and parses the data (using ReGex) so it can be stored into an external data store and used for visualizations. The server also handles requests so the user can monitor irrigation from a web browser.

**The Frontend** 

The frontend component consists of a web page that is rendered when a user makes a HTTP request to the server from a web browser. It displays visualizations of current soil moisture data in the form of gauge charts for each sensor. It also displays long-term soil moisture data collected by each sensor in the form of line charts. These charts are implemented from Google Charts javascript code.
