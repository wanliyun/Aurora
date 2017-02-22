# -*- coding: utf-8 -*-  
import Aurora
class Account(Aurora.ClientEntity):
	def __init__(self):
		print("<<<<<<Account OnBaseCreate>>>>")
		print("Account __init__")
		print(self.__dict__)
		print(self.AccountName)
		print(self.AccountName2)

	def OnBaseCreate(self):
		print("<<<<<<Account OnBaseCreate>>>>")
		print(self.__dict__)

	def OnCellCreate(self):
		print("<<<<<<Account OnCellCreate>>>>")

	def OnTick(self,fDelta):
		pass

	def set_AccountName2(self,newVal):
		print(newVal)

	def OnEnterAOI(self):
		print("<<<<<<Account OnEnterAOI >>>>")

	def OnLeaveAOI(self):
		print("<<<<<<Account OnLeaveAOI >>>>")

	def OnLoseBase(self):
		print("<<<<<<Avatar OnLoseBase>>>>")


	def set_testDict_level(self,newVal):
		print("<<<<<<set_testDict_level %d>>>>"% newVal)
	
class ControlledAccount(Account):
	def OnStartControll(self):
		print("Account OnStartControll")

	def OnStopControlled(self):
		print("Account OnStopControll")

	def OnPlayerEnterSpace(self):
		print("Account OnPlayerEnterSpace")

	def OnPlayerLeaveSpace(self):
		print("Account OnPlayerLeaveSpace")

