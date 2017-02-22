# -*- coding: utf-8 -*-  
import random
import pygame
class Context:
	def __init__(self):
		path = str(__file__)
		path = path[0:path.rfind("/")+1]
		self.rootDir = path

		self.screen = None
		self.userName = "hello%d" % random.randint(1,1000000)
		self.password = "111"
		self.stateMachine = None
		self.viewer = None
		self.width = 1024
		self.height = 768
		self.fullscreen = False
		self.avatar_images = []
		self.avatar_image_size = 50

		self.kt_images = []
		self.kt_image_size = 30

	def LoadImages(self):
		for i in range(20):
			_img = pygame.image.load(self.rootDir + "images/avatar_ (%d).png" % (i+1)).convert_alpha()
			_img = pygame.transform.scale(_img,(self.avatar_image_size,self.avatar_image_size))
			self.avatar_images.append(_img)
		for i in range(4):
			_img = pygame.image.load(self.rootDir + "images/kt_ (%d).png" % (i+1)).convert_alpha()
			_img = pygame.transform.scale(_img,(self.kt_image_size,self.kt_image_size))
			self.kt_images.append(_img)

	def _get_image(self,idx,list):
		if idx >= 0 and idx < len(list):
			return list[idx]
		return None
	def GetAvatarImage(self,idx):
		return self._get_image(idx,self.avatar_images)

	def GetKTImage(self,idx):
		return self._get_image(idx,self.kt_images)



__instance = Context()
def GetInstance():
	return __instance
