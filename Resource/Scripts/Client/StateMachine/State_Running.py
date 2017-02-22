# -*- coding: utf-8 -*-  
from StateMachine import State_Base
import StateMachine
import pygame
import Aurora
class State_Running(State_Base.State_Base):
	def __init__(self,machine):
		State_Base.State_Base.__init__(self,machine)
		self.moveDelta = 3;

	def OnUpdate(self,fTime):
		player = Aurora.entities().player()
		if not player or not player.isControlled:
			return


		pressed = pygame.key.get_pressed()
		if pressed[pygame.K_EQUALS]:
			self.moveDelta *= 2
		if pressed[pygame.K_MINUS]:
			if self.moveDelta > 1:
				self.moveDelta /= 2
			else:
			    self.moveDelta = 1

		bSync = False
		if pressed[pygame.K_UP]:
			bSync = True
			player._z = player._z + self.moveDelta
		if pressed[pygame.K_DOWN]:
			player._z = player._z - self.moveDelta
			bSync = True
		if pressed[pygame.K_LEFT]:
			player._x = player._x - self.moveDelta
			bSync = True
		if pressed[pygame.K_RIGHT]:
			player._x = player._x + self.moveDelta
			bSync = True
		if bSync:
			player.SyncPosRot()

		if pressed[pygame.K_a]:
			player.Cell("haha",(1,2,3))
		if pressed[pygame.K_b]:
			player.Base("base_haha",(1,2))
		if pressed[pygame.K_c]:
			player.Base("base_haha3",(1,2))

	def OnLeave(self):
		pass

	def OnEnter(self):
		pass

	def OnKeyDownEvent(self,pyEvent):
		pass
