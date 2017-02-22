import Aurora

class Avatar(Aurora.CellEntity):
	def __init__(self):
		print("Avatar __init__")

	def OnCreate(self):
		print("<<<<<<Avatar OnCreate",self.id,">>>>")

	def OnDestroy(self):
		print("<<<<<<Avatar OnCellDestroy>>>>")

	def haha(self,args):
		print("haha",args)
		#self.OwnClient("client_haha", (10,22,34)  )
		#self.OtherClient("client_haha", (10,22,34)  )
		#self.AllClient("client_haha", (10,22,34)  )
		self.Base("base_haha2", (1,2,3,4,5,6)  )

		#self.attr1 = self.attr1 + 1
		self.attr2= self.attr2 + 2

	def cell_haha(self,args):
		print("cell_haha",args)
	
	def set_attr4(self,new):
		print("<<<<<<Avatar attr4 new = %d,old = %d>>>>"%(new,self.attr4))

	def test_pos_rot_sync(self,args):
		print(self._x,self._y,self._z,"  ",self._Y,self._P,self._R)
		self._x = self._x + 1
