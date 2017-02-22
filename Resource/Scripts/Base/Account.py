# -*- coding: utf-8 -*-  
import Aurora

class Account(Aurora.BaseEntity):
	def __init__(self):
		print("Avatar __init__")
		print(self.__dict__)
		self.AccountName = "哇哈哈"
		self.AccountName2 = 1
		#self.AccountName3 = ""
		self.a = 1
		self.b = 1
		print(dir(self))
		print(self.__dict__)

	def OnCreate(self):
		print("<<<<<<Account OnCreate id=",self.id,">>>>")
		print(self.__dict__)
		self.testDict.level = self.testDict.level + 1

	def OnGetClient(self):
		print("<<<<<<Account OnGetClient>>>>")

	def OnLoseClient(self):
		print("<<<<<<Account OnLoseClient>>>>")

	def OnTick(self,delta):
		#self.AccountName2 = self.AccountName2 + 1
		#self.AccountName = "哇哈哈%d" % self.AccountName2
		#self.testDict.level = self.testDict.level + 1
		self.testDict.level = self.testDict.level + 1
		print(str(self.testDict))

	def OnDestroy(self):
		print("<<<<<<Account OnDestroy>>>>")

