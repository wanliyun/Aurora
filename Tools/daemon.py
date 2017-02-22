import sys
from subprocess import call

while True:
	print("<<Start Proc>>"," ".join(sys.argv[1:]))
	call(sys.argv[1:])
