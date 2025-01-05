This directory is the remote sense cluster.

The Fishinator uses it to get data about light levels, temperatures, and 
a water-depth measurement.  Also available are voltage levels from the
remote sense cluster circuit.

Via bluetooth serial, the remote sense cluster sends its data
to the FishinatorPi device.

This directory includes the arduino-interface code for the ATMEL 328p
microcontroller.  A subdirectory holds the (current state of) the
KiCAD files for manufacturing a remote sense cluster printed circuit board.  
The KiCAD design is based on the prototype-board impementation with the Fishinator.

