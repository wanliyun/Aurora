
import Viewer
import pygame
import Aurora
from StateMachine import StateMachine
import random
import sys
import Context

def OnInit():
	random.seed()
	print(sys.path)
	_ctx = Context.GetInstance()
	_ctx.stateMachine = StateMachine()
	_ctx.viewer = Viewer.GetInstance()
	_ctx.viewer.Init()
	_ctx.LoadImages()

def OnUpdate(fTime):
	_ctx = Context.GetInstance()
	_ctx.stateMachine.Update(fTime)
	_ctx.viewer.Update(fTime)

def OnFini():
	_ctx = Context.GetInstance()
	_ctx.viewer.Fini()


