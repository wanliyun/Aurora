# -*- coding: utf-8 -*-  

from StateMachine import State_Init
from StateMachine import State_Login
from StateMachine import State_Running
from StateMachine import State_Failed

import Aurora

class StateMachine:
	[ES_Init,ES_Login,ES_Running,ES_Failed] = range(4)
	def __init__(self):
		self.state = StateMachine.ES_Init;
		self.states = {}
		self.states[StateMachine.ES_Init] = State_Init.State_Init(self)
		self.states[StateMachine.ES_Login] = State_Login.State_Login(self)
		self.states[StateMachine.ES_Running] = State_Running.State_Running(self)
		self.states[StateMachine.ES_Failed] = State_Failed.State_Failed(self)

		self.states[self.state].OnEnter()

	def Update(self,fDeltaTime):
		self.states[self.state].DoUpdate(fDeltaTime)

	def ChangeState(self,newState):
		if newState < StateMachine.ES_Init or newState > StateMachine.ES_Running:
			print("Bad state")
		
		self.states[self.state].OnLeave()
		self.state = newState
		self.states[self.state].OnEnter()

