# -*- coding: utf-8 -*-  
import pygame
import Context

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
class Viewer:

	def Init(self):
		pygame.init()
		self.ResetScreen()
		pygame.display.set_caption('Aurora Client Viewer')
		self.background_position = [0, 0]
		_ctx = Context.GetInstance()
		self.background_image = pygame.image.load(_ctx.rootDir + "images/beijing.jpg").convert()
		self.entities = {}

	def ResetScreen(self):
		_ctx = Context.GetInstance()
		flag = 0
		if _ctx.fullscreen:
			flag = pygame.FULLSCREEN

		self.screen = pygame.display.set_mode((_ctx.width, _ctx.height), flag , 32)

	def Update(self,fDeltaTime):
		pygame.display.flip()
		self.screen.blit(self.background_image, self.background_position)
		for ent in self.entities.values():
			ent.Update(fDeltaTime,self.screen)
		
		
	def Fini(self):
		pygame.quit()

	def RegisterEntity(self,id,ent):
		if not ent:
			print("error RegisterEntity ,bad ent")
			return False
		if id in self.entities:
			print("error RegisterEntity ,id exists")
			return False

		self.entities[id] = ent
		return True

	def UnRegisterEntity(self,id):
		if id in self.entities:
			self.entities.pop(id)
		return True

__instance = Viewer()
def GetInstance():
	return __instance