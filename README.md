# mavLinkGCS
This QT project shows how a basic ground control station can be setup for the hardware running on mavlink based communication.

### Assumptions
1. PX4 is running on the same machine in SITL mode with a quadrotor vehicle.
2. The quadrotor is being controlled using a PS4 controller which can be connected through bluetooth or USB to the machine.
3. The PS4 controller's L2 is thrust, left joystick up/down is pitch and left/right is roll, and right joystick left/right is yaw.

### Files
controller.h and controller.cpp : Implements a PS4 controller class to recieve manual position command.
mlinkudp.h and mlinkudp.cpp : This is a udp socket class to send and recieve udp packets.
mydrone.h and mydrone.cpp : The main class that handels mavlink message processing.
