# Mosquitto Broker on my Pi 3  
---
### Install by default  
 https://mosquitto.org/2013/01/mosquitto-debian-repository/  
<br><br>
### Install with security.py  
 https://learn.adafruit.com/diy-esp8266-home-security-with-lua-and-mqtt/configuring-mqtt-on-the-raspberry-pi  
<br><br>
### Startup at boot config  
  https://www.reddit.com/r/raspberry_pi/comments/4ria15/running_mosquitto_mqtt_broker_at_startup_on_rpi_2/  
<br><br>
<br><br>    

### Quick start  
---
If you have installed a binary package the broker should have been started  
automatically. If not, it can be started with a basic configuration:  
  
    mosquitto  
  
Then use `mosquitto_sub` to subscribe to a topic:  
  
    mosquitto_sub -t 'test/topic' -v  
  
And to publish a message:  
  
    mosquitto_pub -t 'test/topic' -m 'hello world'  
------------------------
