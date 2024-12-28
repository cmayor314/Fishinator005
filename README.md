# Fishinator005
Fishinator005 is an aquaponic system.  It is for growing both plants and fish together in an enclosed space.

The current working system is here, but still being filled in.
The documentation will take a long time.

The software/hardware includes a 
  -remote sense cluster
  -basic local control system directly on Raspberry Pi
  -basic RESTful interface on Raspberry Pi for control by Supervisor
  -remote control system (Supervisor) with more computing power
  -web-based UI

Components:

#Supervisor - Control and analysis from remote machine with better computational ability via node-red and node.js

#FishinatorPi - currently Raspberry-pi implmentation of Fishinator direct control via node-red and node.js

#RemoteSense - Atmel Mega328p implementation of reading local sensors, sent directly to Fishinator
