CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers

--------------------- INTRODUCTION ---------------------
Name: Pillow Alarm Project
Development: On-going

Description: A pillow that vibrates when set alarm goes off. 
	- Features: 
		+ Vibration
		+ Let users snooze for 5 minutes
		+ Battery saving

The Pillow Alarm Clock module is consisted of:
	- Sleeptrackbackend: The Django framework for the website
	- Connector: The Python module to connect the website with the Feather
	- Alarm: The code for the Adafruit Feather hardware


--------------------- REQUIREMENTS ---------------------

Sleeptrackbackend module requires the following modules:

 * model
 * forms
 * views
 * urls
 * templates

Alarm module requires the following modules:
	Wire (Arduino)
	SPI (Arduino)
	SD (Arduino)
	RTClib (Adafruit)

Connector module requires the following modules:
	EEEDEEENNN
--------------------- INSTALLATION ---------------------	
Installation and Usage: (See User Manual)

Contributing:
	- If you want to contribute to our projects, here are the things you need:
		+ Django
		+ Pycharm IDE
		+ Adafruit Feather and Arduino IDE
		+ Python 3.6+
	- To begin with, there are two things that you can access without our hardware:
		+ The Feather code
		+ Django
		+ The Feather hardware itself (You can also update it to add more features)
	- Any changes you make will need overall agreement from different parts of the whole project

************************CONFIGURATION************************
------------------- models.py ------------------
You need models:
	- You want to add different characters for different forms
		+ password for more security

Current models:
	- User_Alarm: 
		+ user_id is username as string with 100 maximum characters 
		+ alarm_time is alarm with assigned form of TimeField
	- Night:
		+ function:
			_conversion: find the difference in sleep time to find the hours of sleep
			_save: save the result

Dependencies:
	- django models

------------------- forms.py -------------------
You need forms:
	- You want to add different forms for users to put input into and save it to your database MySQL
	- You want to expand the database

Current forms:
	- UserAlarmForm: 
		+ ask for username
		+ ask for alarm time
		+ Meta: transfer information about the User_Alarm model
	- NightForm:
		+ ask for username (sleeper)
		+ ask for time an user start sleeping
		+ ask for time an user stop sleeping
		+ Meta: transfer information about the Night model

Dependencies:
	- django forms
	- User_Alarm and Night model 

------------------- views.py -------------------
You need views:
	- You want to add different forms for users to put input into and save it to your database MySQL
	- You want to have different functions for current buttons or forms on the website
	- You want to change the database (amount of sleep) functions to save more precise or more different kinds of data
		+ Currently: Matplotlib plots a set of data (start sleep and end sleep time)  

views can do a lot more than that, but those are some of the obvious uses of Views.


Dependencies:
	- django.contrib -> messages
	- django.core -> serializers
	- django.http -> JsonResponse, HttpResponse, HttpResponseBadRequest
	- django.shortcuts -> render, redirect, get_object_or_404
	- django.views.decorators.csrf -> csrf_exempt
	- django.views.decorators.http -> require_http_methods
	
	- matplotlib -> pylab, plt

///// --- current functions in views --- /////

create_user:
	- allow users to create new accounts with an username and alarm time
	- if receives input, save it to the database and inform user of next steps
	- else reload form and ask for the input again

edit_user: 
	- shows users their own input
	- allows users to make changes to their input
	- json data can be used to parse and save new alarm time, but currently it is not being used
	- when users make changes to their input, the new input is save and they are returned to new_user.html

sleeptrack:
	- get information of sleep time from users and send it to json database: data (sleeptrack.html)
	- parse data from json to plot the graph of sleep time in a week
		+ x: the day of sleep (night.day_id from Night model)
		+ y: the amount of sleep that day (night.conversion function from Night model)

nights: 
	- request the data input automatically from the arduino module when connected (in process)
	- allow user to manually update their sleep time
	- return data to sleeptrack.html -> sleeptrack starts graphing

user_list:
	- show users their accounts through and link to users.html

about_page:
	- explain the page and link to about.html

------------------- urls.py ------------------
You need urls:
	- You want to add different path to the website
	- The navigation bar (menu) of the website access different path through the urls

Current paths:
	- new_user/ : create a new user
	- edit/<currentuser>: edit alarm time of the current user
	- users/: access users list
	- about/: explanation of project and contact 
	- sleeptrack/<currentuser>: show the graph of sleep data
	- nights/: ask for users input on their sleeping time (either automatically or manually)

Dependencies:
	- django.urls -> path
	- views

------------------- templates ------------------
You need templates:
	- You want to make changes to the look of the website
		+ navigation bar
		+ color theme, background picture, logo
		+ CSS and HTML
	- You want add urls and path and link them through buttons/links
	- You want to create different access to more different forms through different paths

-------------connector.py-------------
imports:
	datetime
	time
	setTimeCommand from setTime
	Command from command
	ArduinoSerial from arduinoSerial
	requestServer from requestServer
	serial.tools.list_ports
	sendDate from sendData
	Connection from requests.exception

	*Imports many functions and classes that will be expanded upon later 
purpose:
	To connect the django webserver and the Arduino. Allows the user to enter a username and will recieve alarm time information under that username from the web server. It will then take that alarm time and send it to the Arduino, and in return, recieve that user's sleeptracking data and send it to the django server. 
functions:
	- getTime()
		Uses the datetime module to return the current date and time
main code:
	- Checks which port the arduino is connected to
		+ Sets variable ports to None
		+ Puts the ports into a list
		+ Goes through the list and checks if a port is connected to the Arduino
		+ If one is found it will print out the port that the Arduino is not connected and continue on with the rest of the program 
		+ If a port is not found to be connected to the Arduino, it will print out that the Arduino is no connected and exit from the program
	- Instantiates the connection with Arduino through the port that was found in the 
	  previous lines
	  	+ This is done using the Arduino Serial class
	- Following section wrapped in a try and except loop to test if the django server is 	  running
		+ If the server is not running and ConnectionError arises, it doesn't run the rest of the code and prints out to the user that the server isn't running
	- With the server running it continues on with main program
	- Uses the getTime() function to get the current time and save that in the value current_time
	- Passes current_time to the SetTimeCommand class and saves to setTimeCommand
	- Sends the setTimeCommand variable to the arduino
	- Prompts the user to input a username 
		+ This username is passed to the requestServer class in order to obtain the username and the alarm time from the django server
		+ These values are then send to the Arduino 
	- Connection is closed and then reopened and then the getData string is sent in order to get the sleeptracking data from the Arduino
	- Reads the response from the connector and stored in variable response 
	- The response is decoded from binary and editted to be formatted correctly so that it can be in the correct format to pass to the django server
	- Gets the user ID number by use of the getIDNumber function from the requestServer class. This value is used to send the sleep tracking data to the correct page 
	- Sleep tracking data obtained from arduino is sent to the user's page
	- Serial connection is closed

-------------arduinoSerial.py-------------
imports:
	serial
	SerialException from serial.serialutil
	time
purpose:
	Start the connection between the user and Arduino
class:
	ArduinoSerial
functions:
	- init
		+ Passed the paramater portString
		+ Sets the baudrate to the same one as the one on the Arduino(has to match)
		+ Sets the port to the port that is passed to it
		+ Sets the timeout time
	- connect
		+ Trys the connection between the Arduino and connector
		+ Lets the Arduino rest a bit after the connection is started. Gives it time to process other commands coming it's way
		+ If the connection can not be completed is passes an Exception
	- readLine
		+ Read the line of the arduino serial and puts that in the variable result 
		+ Returns result 
	- close 
		+ Closes the connection between the connector and the Arduino
-------------command.py-------------
imports: 
	serial
	SerialException from serial.serialutil
	time
purpose:
	Helps with decoding and encoding information
class: 
	Command
functions: 
	- init 
		+ Passed a value as a paramater 
	- getBytes
		+ returns an encoded string 
	- str
		+ returns the value passed as a string
-------------setTime.py-------------
imports:
	datetime 
	time
	Command from command
purpose: 
	To set the time to the current time
class: 
	SetTimeCommand
		+ inherits from Command
function:
	- init
		+ Sets value to the current time 
-------------requestServer.py-------------
imports: 
	requests
	re 
purpose: 
	To obtain the username, alarmtime, and user ID number from the django webserver. Is done using requests and regEx to get to the user's webpage and sort through the information on the server to get the desired infomartion. 
class: 
	requestServer
function: 
	- init
		+ Is passed a username that is used to get to the webpage for that user 
	- getResponse 
		+ Returns the information from the django web server page as a block of text 
	- getUsername 
		+ Gets the reponse from the web page
		+ Finds all the necessary information in a string and stores it in a list 
		+ The first value is a blank space, second value is the username, and third value is the alarm time
		+ Is editted to be formatted correctly
		+ If no value is put into the list, that means that a username could not be found and is told to the user that no username could be found under the one that was given
	- getAlarmTime 
		+ Goes through same process as getUsername but for the Alarm Time
	- getIDNumber 
		+ Searches for the string useridnumber in block of text
		+ Returns this value 
		+ The user ID number is important so that the sleep tracking data can later be sent back to this same web page and automatically update the graph
-------------sendData.py-------------
imports: 
	requests
	datetime
purpose: 
	To send the sleep tracking from the Arduino back to the django web server
class: 
	sendData
function: 
	- init 
		+ Passed the userID, startTime, and endTime as a paramater
		+ Sets the date to the current date 
		+ Sets the webpage that the data needs to be sent back to
		+ Puts the necessary data in the format that its needs to be to be able to be sent back to the web page and automatically update the user's sleeping graph
	- send
		+ Uses post to send the data to the url that was set in the init 


-------------FeatherCode.ino-------------
imports:
	Wire (Built-in Arduino)
	SPI (Built-in Arduino)
	SD (Built-in Arduino)
	RTClib (Adafruit Industries)
purpose:
	 Code for the Adafruit Feather M0 Adalogger microcontroller that runs the alarm device for Pillow Alarm Clock. Will first recieve and handle any input from the Serial. It will perform all duties associated with the alarm capabilites.
components:
	- Adafruit Feather M0 Adalogger (AFMA)
	- DYIMore DS3231 AT24C32 IIC High Precision Real Time Clock Module Clock 	- Timer Memory Module For Arduino (RTC)
	- Force Sensitive Resistor (FSR)
	- Coin vibration motor (CVM)
	- MicroSD Card (MSD)
functions:
	- setup
		+ Initial setup for the AFMA, includes checking if the RTC and MSD have initialized sucessfully. 
		+ Will hold if anything does not initialize, and throw an error to Serial.
		+ Note: Will only run once when the code is complied and uploaded to AFMA
	- loop
		+ Main loop for program, runs until a new program has been uploaded (full description below)
	- fsr
		+ Checks to see if the FSR detects any pressure. Returns true if pressure is detected, returns false if not.
	- parseCommand
		+ Takes any input from Serial, and compares this input to any known commands (below). 
		+ If recognized, call the appropriate function. 
		+ If not recognized, throw an error to Serial, but continue running
	- setAlarm
		+ Set the alarm time to the given time String (passed as an argument)
		+ Since the argument is given as a String, the function will parse the string into a DateTime value
	- setTime
		+ Set the RTC time to the given date and time String (passed as an argument)
		+ Since the argument is given as a String, the function will parse the string into a DateTime value
	- getData
		+ Print the data saved on the MSD to the Serial

main code (loop function):
	- Check to see if the Serial recieves any input, and compare any given input to all known input. If the given input matches a known input, handle accordingly.
	- When the FSR does not detect pressure, and the alarm has not been triggered (original alarm time was not reached), nothing happens.
	- When the FSR detects pressure, and the alarm has not been triggered, the AFMA checks if the starting sleep time has been stored to the MSD. 
		+ If the starting sleep time has not been stored, the AFMA writes the current time to the MSD
	- When the pressure sensor detects pressure, and the RTC time has reached the preset alarm time, the CVM engages until the FSR no longer detects pressure (alarm has been triggered).
	- When the pressure sensor no longer detects pressure, and the alarm has been triggered, add five minutes to the alarm time (snooze has been armed)
	- If the FSR detects pressure while snooze has been armed, wait until the new alarm time has been reached before triggering the alarm
	- (Repeat above three lines until FSR does not detect pressure while snooze is armed)
	- When FSR does not detect pressure while snooze is armed (5 continuous minutes after last alarm went off):
	+ Write the current time minus 5 min to the MSD (assumed to be when the user woke up)
	+ Set the alarm to the original alarm time, one day later
circuit:
	- RTC VCC connected to AFMA 3V
	- RTC GND connected to AFMA GND
	- RTC SDA connected to AFMA SDA
	- RTC SCL connected to AFMA SCL
	- CVM from AFMA BAT Pin to drain of 2N7000 MOSFET
	- 2N7000 MOSFET 
		+ source to AFMA GND
		+ gate to AFMA Pin 9
		+ drain to the negative lead of the CVM
	- FSR with positive lead to AFMA 3V and negative lead to AFMA A0 and pull-down resistor
	- 220Ω pull-down resistor connected to negative lead of FSR and AFMA GND


************************TROUBLESHOOTING************************

--------------------------------------------------------
License: MIT license 