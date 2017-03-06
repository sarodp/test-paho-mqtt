#!/usr/bin/python
#
#file: testsub.py
#
#-----------------------------------
import paho.mqtt.subscribe as subscribe
import paho.mqtt.publish as publish
import time

#01--init var's
xhostbroker = "localhost"
xtopicRCV = '#'
xtopicSND = 'myrpi/time'


#02--callback subscribe
def on_rcv_message(client, userdata, message):
    print("> topic=%s msg=%s" % (message.topic, message.payload))
    if message.topic <> xtopicSND:
		xmsg= time.ctime()
		publish.single(xtopicSND, xmsg, hostname=xhostbroker)
    

#1--simple subscribe
print ('> subscribe.simple: wait for next publish \n...')
xmsg = subscribe.simple(xtopicRCV, hostname=xhostbroker)
print("> topic=%s msg=%s" % (xmsg.topic, xmsg.payload))
print ('> subscribe.simple: done\n>')
    

#2--loop_forever
print ('> subscribe.callback: loop wait publish forever \n....')
subscribe.callback(on_rcv_message, xtopicRCV, hostname=xhostbroker)
