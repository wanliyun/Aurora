# -*- coding: utf-8 -*-  
import Aurora
import Viewer
from Viewer import View_Avatar
class Avatar(Aurora.ClientEntity):
	def __init__(self):
		print("<<<<<<Avatar OnBaseCreate>>>>")
		print("Avatar __init__")
		self._view = View_Avatar.View_Avatar()


	def OnBaseCreate(self):
		print("<<<<<<Avatar OnBaseCreate>>>>")
		print("AvatarID %d" % self.AvatarID)
		print("Name" , self.Name)
		print("attr1 %d" % self.attr1)
		print("attr2 %d" % self.attr2)
		print("attr4 ", self.attr4)
		print("attr5 ", self.attr5)
		print("attr6 ", self.attr6)

		print("attr7 ", self.attr7)
		print("attr8 ", self.attr8)
		print("<<<<<<Avatar OnBaseCreate>>>>")
		Viewer.GetInstance().RegisterEntity(self.id,self._view)

	def OnCellCreate(self):
		print("<<<<<<Avatar OnCellCreate>>>>")

	def OnTick(self,fDelta):
		#print("<<<<<<Avatar[%d] OnTick(%f,%f,%f)>>>>"%(self.id,self._x,self._y,self._z))
		self._view.setWorldPos(self._x,self._z)
		#self._view.x = self._x
		#self._view.y = self._y
		#print("<<<<<<Avatar[%d] OnTick(%f)>>>>"%(self.id,fDelta))
		#pass

	def OnEnterAOI(self):
		print("<<<<<<Avatar OnEnterAOI ",self.id,">>>>")
		Viewer.GetInstance().RegisterEntity(self.id,self._view)

	def OnLeaveAOI(self):
		Viewer.GetInstance().UnRegisterEntity(self.id)
		print("<<<<<<Avatar OnLeaveAOI ",self.id,">>>>")

	def OnLoseBase(self):
		print("<<<<<<Avatar OnLoseBase>>>>")
	
	def set_attr1(self,new):
		print("<<<<<<Avatar attr1 new = %d,old = %d>>>>"%(new,self.attr1))

	def set_attr2(self,new):
		print("<<<<<<Avatar attr2 new = %d,old = %d>>>>"%(new,self.attr2))

	def set_attr3(self,new):
		print("<<<<<<Avatar attr3 new = %d,old = %d>>>>"%(new,self.attr3))

	def set_attr4(self,new):
		print("<<<<<<Avatar attr4 new = %d,old = %d>>>>"%(new,self.attr4))


	def client_haha(self,arg):
		print("client_haha",arg)
	def echoBack2(self,arg):
		print("echoBack2",arg)

class ControlledAvatar(Avatar):
	def OnStartControll(self):
		self._view.imageIdx = 1
		print("OnStartControll")

	def OnStopControlled(self):
		self._view.imageIdx = 0
		print("OnStopControll")

	def OnPlayerEnterSpace(self):
		print("OnPlayerEnterSpace")

	def OnPlayerLeaveSpace(self):
		print("OnPlayerLeaveSpace")

	def OnKey(self,key):
		if( key == 'a' ):
			self.Cell("haha",(1,2,3))
		elif(key == 'b'):
			self.Base("base_haha",(1,2))
		elif(key == 'c'):
			self.Base("base_haha3",(1,2))
		elif(key == 'p'):
			self.Cell("test_pos_rot_sync",())
		elif(key == 'o'):
			if(self.isControlled):
				print(self._x)
				self._x += 1
			else:
				print("you not not controlled")
		elif(key == 't'):
			if(self.isControlled):
				self.SyncPosRot()
			else:
				print("you not not controlled")

