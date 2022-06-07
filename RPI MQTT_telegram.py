#!/usr/bin/python3
import paho.mqtt.client as mqtt #import the client1
import time
import datetime  # Importing the datetime library
import telepot   # Importing the telepot library
from telepot.loop import MessageLoop    # Library function to communicate with telegram bot
import RPi.GPIO as GPIO     # Importing the GPIO library to use the GPIO pins of Raspberry pi
from time import sleep      # Importing the time library to provide the delays in program

GPIO.setmode(GPIO.BCM)      # Use Board pin numbering
#GPIO.setup(red_led_pin, GPIO.OUT) # Declaring the GPIO 21 as output pin
#GPIO.setup(green_led_pin, GPIO.OUT) # Declaring the GPIO 20 as output pin

now = datetime.datetime.now() # Getting date and time
chat_id = "1632758162"

#def handle(msg):
#    bot.sendMessage(chat_id, str("Er is een mug in uw kamer!"))

# Insert your telegram token below
bot = telepot.Bot('5319894677:AAFJiI6Zlc11kMRuZeVdPXbzLEGuJmcAbiE')
print (bot.getMe())

############
def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    if(message.payload.decode("utf-8")) == "MUG!":
        print("yes!")
        bot.sendMessage(chat_id, str("Er is een mug in uw kamer!"))
        time.sleep(60)
#   print("message qos=",message.qos)
#    print("message retain flag=",message.retain)
########################################
#broker_address="192.168.0.205"
broker_address="10.6.120.6"
#broker_address="iot.eclipse.org"
print("creating new instance")
client = mqtt.Client("P1") #create new instance
client.on_message=on_message #attach function to callback
print("connecting to broker")
client.connect(broker_address, 1885) #connect to broker, port
client.loop_start() #start the loop
print("Subscribing to topic","test")
while True:
        client.subscribe("test")
        #print("Publishing message to topic","test")
        #client.publish("test","OFF")
        time.sleep(60) # wait
        #client.loop_stop() #stop the loop
