# -*- coding: utf-8 -*-  
import Aurora

class Avatar(Aurora.BaseEntity):
	def __init__(self):
		import sys
		print("<<<<<<Avatar __init__")
		print(sys.path)
		print("Avatar __init__>>>>>>")

		entMgr = Aurora.entities()
		print(entMgr)
		keys = entMgr.keys()
		print(keys)
		values = entMgr.values();
		print(values)
		items = entMgr.items()
		print(items)
		print(entMgr.has_key(self.id))
		print(len(entMgr))

		if len(entMgr) > 0:
			val = entMgr[keys[0]]

	def OnCreate(self):
		print("<<<<<<Avatar OnCreate>>>>")

	def OnTick(self,delta):
		pass
		#self.attr1 = self.attr1 + int(delta)
		#print("<<<<<<Avatar[%d] OnTick(%d)>>>>"%(self.id,self.attr1))

	def OnGetClient(self):
		print("<<<<<<Avatar OnGetClient>>>>")
		self.AvatarID = 12
		self.attr1 = 13
		self.attr2 = 14
		self.EnterSpace(0,0)

	def OnLoseClient(self):
		print("<<<<<<Avatar OnLoseClient>>>>")

	def base_haha(self,args):
		#tupleArg = pickle.loads(args)
		print("<<<<<<Avatar base_haha",self.id,args,">>>>")
		self.attr4= self.attr4 + 2

	def base_haha2(self,args):
		print("<<<<<<Avatar base_haha2",self.id,args,">>>>")
		self.Client("echoBack2",(2**8,))
	def base_haha3(self,args):
		print("<<<<<<Avatar base_haha3",self.id,args,">>>>")
		self.Cell("cell_haha",(30,40) )

