# -*- coding: utf-8 -*-  
import Aurora

class Other():
	def __init__(self):
		self.o_a =10;
		print("Other::__init__()");
	def other_test(self):
		print("Other::other_test()");

class NPC(Aurora.ClientEntity,Other):
	def __init__(self):
		Other.__init__(self);

		print("NPC __init__")

		# Aurora.ClientEntity.__init__(self)
	def testPY(self):
		print("NPC test start")
		import sys
		print(sys.path)
		print(self.id)
		print(self.o_a)
		print(dir(self))
		self.other_test();
		self.test_global = 1024
		self.test_global = self.test_global  + 1
		self.o_a = 2048
		print("NPC test finish")


