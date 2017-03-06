#-------------------------------------------------------------
# module: import paho.mqqt.client
#
# You can use the client class as an instance, 
# within a class or by subclassing. 
# The general usage flow is as follows:
#
#  1. Create a client instance
#  2. Connect to a broker using one of the connect*() functions
#  3. Call one of the loop*() functions to maintain network traffic flow with the broker
#  4. Use subscribe() to subscribe to a topic and receive messages
#  5. Use publish() to publish messages to the broker
#  6. Use disconnect() to disconnect from the broker
# 
#  Callbacks will be called to allow the application to process events as necessary.
#-------------------------------------------------------------


import paho.mqtt.client as mqtt
import time


#00--callback functions
#-------------------------------------------------
#00a--on recieve a CONNACK response 
def on_connect(client, userdata, flags, rc):
    print("Connected ==> result code = "+str(rc))

    #--always renew subscribe every connect and reconnect 
    #client.subscribe("$SYS/#")
    client.subscribe("#")

#00b--on receive a PUBLISH message 
def on_message(client, userdata, msg):
    print ('>topic:%s \n msg: %s' %(msg.topic ,msg.payload))

#--to be tested later!
def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(mosq, obj, level, string):
    print(string)


#01--init client
#------------------------------------------------------------
xbrokername = "localhost"
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(xbrokername, 1883, 60)

#02--start background thread
# Calling loop_start() once, before or after connect*(), 
# runs a thread in the background to call loop() automatically. 
# This frees up the main thread for other work that may be blocking. 
# This call also handles reconnecting to the broker. 
client.loop_start()

#1--main loop
while 1:
	xtopic = 'myrpi/ctime'
	xmsg = time.ctime()
	client.publish(xtopic,payload=xmsg,qos=0,retain=False)
	time.sleep(2)

#99--end
# Call loop_stop() to stop the background thread. 
# The force argument is currently ignored.
client.loop_stop()
client.disconnect()
