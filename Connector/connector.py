import datetime
import threading
import time
from setTime import SetTimeCommand
from command import Command 
from arduinoSerial import ArduinoSerial
from requestServer import requestServer
import serial.tools.list_ports
from sendData import sendData
from requests.exceptions import ConnectionError

def getTime(): 
	return datetime.datetime.now()

def getUserInfo(user_id):
	return { "name" : "Name", "wake_time": "2012-04-23T18:25:43:511Z"}
		
if __name__ == "__main__":

	ports = list(serial.tools.list_ports.comports())
	port = None
	for p in ports:
		try:
			serial = ArduinoSerial(p.device)
			serial.connect()
			serial.sendCommand("Testing Connection")
			response = str(serial.readLine())
			if ("Testing Connection" in response):
				print (p.device)
				port = p.device
		except Exception:
			pass
		serial.close()

	if (port == None):
		print("Arduino not connected")
		exit()

	serial = ArduinoSerial(port)
	serial.connect()

	try:
		current_time = getTime()
		setTimeCommand = SetTimeCommand(current_time)
		#serial.sendCommand(setTimeCommand)
		user = input('Username:')
		test = requestServer(user)
		testU = test.getUsername(user)
		testA = test.getAlarmTime(user)
		print (testU)
		print (testA)
		#serial.sendCommand(testU)
		#serial.sendCommand(testA)
		serial.sendCommand('getData')



		print("waiting for response")
		response = serial.readLine()
		finResponse = response.decode()
		text = ''.join(s for s in finResponse if ord(s)>31 and ord(s)<126)
		text = text.replace('I got: getData', '')
		text = text.split(']')
		fin =[s.strip('[') for s in text]
		newList = []
		for word in fin:
		    word = word.split(",")
		    newList.extend(word)
		for word in newList: 
			word = word.lstrip()

		newList = [s.replace('/', '-') for s in newList]

		userID = test.getIDNumber(user)
		for i in range(0,len(newList)-1,2):
		 	sender = sendData(userID,newList[i],newList[i+1])
		 	sender.send()

	except ConnectionError:
		print("Server not running")

	serial.close()

