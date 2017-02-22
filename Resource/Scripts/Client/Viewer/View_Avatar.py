# -*- coding: utf-8 -*-  
import pygame
import Context
from Viewer import View_Entity


class View_Avatar(View_Entity.View_Entity):
	def __init__(self):
		View_Entity.View_Entity.__init__(self);
		self.imageIdx = 0

	def Draw(self,screen):
		_ctx = Context.GetInstance()
		_img = _ctx.GetKTImage(self.imageIdx)
		width = _img.get_width() / 2 
		if _img:
			screen.blit(_img, [self.x - width, self.y - width])


