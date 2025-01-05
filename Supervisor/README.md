This directory is the project directory for the
**node-red** implemention of Supervisory (indirect)
control over the Fishinator Raspberry Pi direct control.

The FishinatorPi direct control (on the Raspberry Pi) does **not**
need this Supervisor to function properly. The FishinatorPi direct
control can independently operate the Fishinator. 

Because the mathematics used for signal analysis consume
heavier computation, these flows are meant to run on a 
computer with considerably more computational power than a 
Raspberry Pi.

If you have a fast Raspberry Pi from the future, the 
Supervisor code and the FishinatorPi code could run on 
the same system, but you would need to make some tweaks
(mostly with respect to IP addresses, which ports are used, etc)

The programming interface is on localhost:1880,
and the UI interface is on localhost:1880/ui


![ui_Fishinator Live Graphs](https://github.com/user-attachments/assets/e7489ee1-dbdd-451e-b4db-8984cda18cb8)

![ui_Fishinator Analysis](https://github.com/user-attachments/assets/5dbfef36-4350-462b-a232-9f9a4d03644c)
