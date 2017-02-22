# -*- coding: utf-8 -*-  
from StateMachine import State_Base
import StateMachine
import Aurora
import Context
class State_Login(State_Base.State_Base):
	def __init__(self,machine):
		State_Base.State_Base.__init__(self,machine)

	def OnUpdate(self,fTime):
		_ctx = Context.GetInstance()

		if Aurora.app().StartLogin(_ctx.userName,_ctx.password):
			self.stateMachine.ChangeState(self.stateMachine.ES_Running)
		else:
			print("Login Failed")
			self.stateMachine.ChangeState(self.stateMachine.ES_Failed)

	def OnLeave(self):
		pass

	def OnEnter(self):
		pass
