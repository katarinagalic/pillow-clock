import requests
import datetime

class sendData():
	def __init__(self, userID, startTime, endTime):
		self.__date = datetime.datetime.now().date()
		self.__url  = "http://127.0.0.1:8000/nights/"
		self.__user = userID
		self.__start = startTime
		self.__end = endTime
		self.__data = {
		'sleeper': self.__user,
		'start_sleep': self.__start,
		'end_sleep': self.__end
		}
		
	def send (self):
		requests.post(url = self.__url, data = self.__data)

if __name__ == "__main__":
	pass
	# testArray = ["2019-10-31 20:21:01", "2019-10-31 20:22:01"]
	# test = sendData ("2", testArray[0], testArray[1])
	# test.send()

	# test = ("I got: getData"
	# 		"[2019/05/26 14:41:54, 2019/05/26 14:42:02]"
	# 		"[2019/05/26 14:42:03, 2019/05/26 14:42:03]"
	# 		"[2019/05/26 14:42:03, 2019/05/26 14:42:03]"
	# 		"[2019/05/26 14:42:03, 2019/05/26 14:42:03]"
	# 		"[2019/05/26 14:42:03, 2019/05/26 14:42:04]"
	# 		"[2019/05/26 14:42:04, 2019/05/26 14:42:05]"
	# 		"[2019/05/26 14:42:05, 2019/05/26 14:42:05]"
	# 		"[2019/05/26 14:42:05, 2019/05/26 14:42:05]"
	# 		"[2019/05/26 14:42:05, 2019/05/26 14:42:06]"
	# 		"[2019/05/26 14:42:06, 2019/05/26 14:42:13]"
	# 		"[2019/05/26 14:42:14, 2019/05/26 14:42:16]")
	# test = test.replace('I got: getData', '')
	# test = test.replace('[', '')
	# fin = test.split(',')
	# print (fin[0])
