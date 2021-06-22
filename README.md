# hw4

1.XBee Controlled BB Car

use "sudo python3 xbee.py" to compile it.
use "/park/run d2 d1 k" to send RPC commands, where k is the direction(0,1,2,3).
The xbee will receive the position parameter and start to park the car.



2.Line Following BB Car

Use OpenMV to detect a straight line (printed on a paper).
Send the parameters of the detected line (end points of lines as x1, y1, x2, y2) to mbed through UART connections.
Steer BB car to go forward and follow the line.



3.BB Car Position Calibration

use OpenMV to scan the above AprilTag codes. We can use the scanned AprilTag to determine the viewing angle from OpenMV to the AprilTag surface.
steer BB Car such that the car is facing directly and perpendicular to the AprilTag surface.
