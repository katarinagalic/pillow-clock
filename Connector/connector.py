import datetime

def getTime(): 
	return datetime.datetime.now()

def getUserInfo(user_id):
	return { "name" : "Name", "wake_time": "2012-04-23T18:25:43:511Z"}

if __name__ == "__main__":
	time = getTime()
	wake_time = getUserInfo("wake_time")

	print(time)
	print(wake_time)