# thimble-wifi-robot-controller
I'm just starting out learning Unity, and getting some electronics experience with monthly Thimble kits. So, here's a quick Unity sketch mocked up to control the WiFi robot built from the Thimble project kit. This was kind of hacked together over a couple of days, but I was able to chase the dog around with a phone-controlled robot so it was well worth it!

### Contains
##### Arduino 
The arduino sketch that is being used for this controller.

##### Unity
The Unity project with text inputs to specify the IP and Port of the robot, and inputs to control the values that are sent by the directional pad buttons + the "STOP ROBOT" button. 

NOTE: Due to the inconsistency I was having with UDP, it's set to continue firing packets once a button is pressed, and continue firing stop packets ('x' in the code) once the button stops being pressed.

### Possible Extensions (To Do?)
I'm having fun driving it around, but it would be really neat to do something like:
- Change the D-pad into a GUI joystick to give a more natural "tap-and-drag" motion.
- Perform motor power calculations based on the 2D vector from center to touch of GUI joystick so that it turns when dragged at an angle
- With a GUI joystick, encode the distance from center and send that in the UDP packets to adjust motor voltage.
- Incorporate distance sensor for extra safety

### Credit
UDP class for Unity which helped kick start the packet sending code.
https://forum.unity3d.com/threads/simple-udp-implementation-send-read-via-mono-c.15900/