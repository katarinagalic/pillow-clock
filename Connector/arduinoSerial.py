import serial
from serial.serialutil import SerialException
import time



class ArduinoSerial():
	def __init__(self, portString):
		self.serial = serial.Serial()
		self.serial.baudrate = 9600
		self.serial.port = portString
		self.serial.timeout= 5

	def connect(self):
		try:
			self.serial.open()
			time.sleep(2)
		except Exception as e:
			raise e

	def readLine(self):
		self.serial.flushInput()
		result = self.serial.read_until('\r\n')
		return result


	def sendCommand(self,command):
		self.serial.write(("%s\r\n"%command).encode())
		print("Sent command: %s"%(command))

	def close(self):
		self.serial.close()