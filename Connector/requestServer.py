import requests
import re

class requestServer():

	def __init__ (self, user= ''): 
		self.__response = requests.get('http://127.0.0.1:8000/edit/' + user)

	def getResponse (self):
		return self.__response.text

	def getUsername (self, user):
		resp = self.getResponse()
		x = re.findall(r'value(\S+)', resp)
		try: 
			fin = x[1]
			fin = fin.strip('=')
			fin = fin.strip('"')
			fin = fin.strip (' ')
			return fin
		except: 
			return ('No Username under ' + user)

	def getAlarmTime (self, user):
		resp = self.getResponse()
		x = re.findall(r'value(\S+)', resp)
		try: 
			fin = x[2]
			fin = fin.strip('=')
			fin = fin.strip('"')
			fin = fin.strip (' ')
			return fin
		except: 
			return ('No Alarm Time under ' + user)

	def getIDNumber (self,user): 
		resp = self.getResponse()
		x = re.search('(?<=useridnumber)(.*)', resp)
		try: 
			fin = x.group(1)
			fin = fin.strip('</p>')
			fin = fin.strip (' ')
			return fin
		except: 
			return ('No UserId under ' + user)




if __name__ == "__main__": 

	user = input('Username:')
	test = requestServer(user)
	testU = test.getUsername(user)
	testA = test.getAlarmTime(user)
	testI = test.getIDNumber(user)
	print (testU)
	print (testA)
	print (testI)



