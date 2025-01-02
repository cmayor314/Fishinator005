# Fishinator005
Fishinator005 is an **Aquaponic** system.  It is for growing both plants and fish together in a enclosed space
for the purposes of food.

## Aquaponics

Aquaponics is conservative of water; the only significant water loss is due to evaporation.
Two crops are fed for the price of one: you feed the fish and ammonia from their gills is plant food.

## What's here

The current working system is here, but still being filled in.
The documentation will take a long time.

The software/hardware includes a:

  - remote sense cluster
  
  - basic *local-to-Fishinator* control system running directly on Raspberry Pi + some hardware
  
  - basic RESTful interface on Raspberry Pi for control by Supervisor Flows
  
  - Supervisor system Control and Analysis on a remote machine with more computing power available
  
  - web-based UI

## Components:

### Supervisor 
- Control and analysis from remote machine with better computational ability via node-red and node.js

  ![ui_Control Fishinator](https://github.com/user-attachments/assets/f0a340a5-24ef-4ec4-8ce9-3065dece5ce1)
![ui_Fishinator Live Graphs](https://github.com/user-attachments/assets/bda65660-0e32-4fd4-8100-da0c39c7e05c)
![ui_Fishinator Services](https://github.com/user-attachments/assets/201bdedc-3213-401d-9b0f-3c479d052240)


### FishinatorPi 
- currently Raspberry-pi implmentation of Fishinator direct control via node-red and node.js

  ![ui_Fishinator Dashboard](https://github.com/user-attachments/assets/2d53bbf2-d707-4cd0-874e-793f6db97b89)


### RemoteSense 
- Atmel Mega328p implementation of reading local sensors, sent directly to Fishinator
			-KiCAD schematic files (schematic complete, placement and routing not started)
			-arduino-interface code
  
![Remote Sense Cluster Prototype-small](https://github.com/user-attachments/assets/5ee2cc5d-6b11-4d74-8e21-8d420c308ce2)

![RemoteSense schematic](https://github.com/user-attachments/assets/0b566a1b-1002-4e11-a33f-8e814c3706d1)
