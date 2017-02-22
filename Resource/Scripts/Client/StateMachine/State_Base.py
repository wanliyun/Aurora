# -*- coding: utf-8 -*-  
import pygame
import Aurora
import Context

class State_Base:
	def __init__(self,theMachine):
		self.stateMachine = theMachine

	def DoUpdate(self,fTime):
		bExit = False
		for event in pygame.event.get():
			if event.type == pygame.KEYDOWN:
				if event.key == pygame.K_ESCAPE:
					bExit = True
				elif event.key == pygame.K_F11:
					_ctx = Context.GetInstance()
					_ctx.fullscreen = not _ctx.fullscreen
					_ctx.viewer.ResetScreen()

				else:
					self.OnKeyDownEvent(event)
			elif event.type == pygame.QUIT:
				bExit = True

		self.OnUpdate(fTime)

		if bExit:
			Aurora.app().Stop()

	def OnUpdate(self,fTime):
		pass

	def OnKeyDownEvent(self,pyEvent):
		pass

	def OnLeave(self):
		pass

	def OnEnter(self):
		pass

