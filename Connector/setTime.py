import datetime
import serial
from serial.serialutil import SerialException
import time
from command import Command


class SetTimeCommand(Command):
	def __init__(self, time):
		super(SetTimeCommand, self).__init__(time)
		#self.command_string = "SETTIME"
		self.value = time