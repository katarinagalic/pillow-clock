
import serial
from serial.serialutil import SerialException
import time

class Command():
	def __init__(self,value):
		self.command_string = ""
		self.value = value

	def getBytes(self):
		return self.__str__().encode()

	def __str__(self):
		return "%s %s"%(self.command_string,self.value)