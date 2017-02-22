# -*- coding: utf-8 -*-  
from StateMachine import State_Base
import StateMachine

class State_Failed(State_Base.State_Base):
	def __init__(self,machine):
		State_Base.State_Base.__init__(self,machine)

	def OnUpdate(self,fTime):
		self.stateMachine.ChangeState(self.stateMachine.ES_Init)

	def OnLeave(self):
		pass

	def OnEnter(self):
		pass
