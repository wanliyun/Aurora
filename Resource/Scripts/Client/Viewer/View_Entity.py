# -*- coding: utf-8 -*-  
import pygame
import Context
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

def translate(_x,_z):
	_ctx = Context.GetInstance()
	_x = _ctx.width / 2  + _x
	_z = _ctx.height / 2 - _z
	return _x,_z

class View_Entity():
	def __init__(self):
		self.x = 0
		self.y = 0

	
	def setWorldPos(self,x,z):
		self.x,self.y = translate(x,z)


	def Draw(self,screen):
		pass

	def Update(self,fDeltaTime,screen = None):
		if screen:
			self.Draw(screen)
