This directory contains the **Remote Sense Cluster** for the fishinator.

This Sense Cluster is "remote" in the sense that it may be placed anywhere (within bluetooth range) with respect to the FishinatorPi (which controls the Fishinator), it is not connected by wire.  The Sense Cluster *must* be reasonably attached to the Fishinator, the system for which it senses.

The Fishinator uses this remote sense cluster to get data about light levels, temperatures, and a water-depth measurement.  Also available are voltage levels from the remote sense cluster circuit.

Via bluetooth serial, the remote sense cluster sends its data to the FishinatorPi device.

This directory includes:

  - the arduino-interface code (C/C++) for the ATMEL 328p microcontroller.
  - A subdirectory holds the (current state of) the KiCAD files for manufacturing a remote sense cluster printed circuit board.  

The KiCAD design is based on the prototype-board impementation with the Fishinator.

![RemoteSense schematic](https://github.com/user-attachments/assets/2a24a804-4afe-48ba-848e-7956d633f929)

![Remote Sense Cluster Prototype-small](https://github.com/user-attachments/assets/d23274c6-848a-4721-bae0-063ce0db4bba)
