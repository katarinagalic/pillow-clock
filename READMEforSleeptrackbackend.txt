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
