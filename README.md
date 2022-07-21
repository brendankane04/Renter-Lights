# Renter Lights

Automatic light control which doesn't involve messing with your light switches & talking to your landlords

# Firmware 

## Files

**Wifi_Interface.cpp:** Driver to send TCP messages over wifi<br>
**SG90.cpp:** Driver for the SG90 servo, turns it on & off.<br>
**SR501.cpp:** Driver the SR501 PIR sensor. The handler init is can be run independently.<br>
main.cpp: main file. Combines all the modules.<br>

# Hardware

**Hardware.sch:** The schematics for the minimum unit<br>
**Hardware.kicad_pcb:** The layout file<br>

# Mechanical


# Backend

A mosquitto MQTT broker server on a raspberry pi is used to interface the devicecs. Just get one up & running.
