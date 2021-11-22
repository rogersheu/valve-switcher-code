# Valve switcher code
Combination of Arduino + Igor Pro code to operate a 5-way valve switcher. The valve switcher was custom built by Wolfgang Nadler, at Drexel University. It came with the ability to run an automated program via an Arduino MEGA, along with local input driven by on/off switches on the box.

However, we wanted to ensure synchronicity of the readings with the times from other instruments. The most immediate way to get the time was to read PC time, but PC time suffers from eventual drift, going as high as a few seconds over the course of a day. The solution was to ensure our computers were synchronized to the same service using Dimension 4, then to grab PC time through an external Igor program, which can be found in the .ipf file.

Via the Igor file, one can set the sample time at each port, then start the automated program, which also tracks the times at which the valve changes states. Both the Arduino and the Igor program need to run, but both cannot run simultaneously because of the nature of the COM port. Therefore, the Arduino program must first be uploaded and run on a loop, then the Igor program can control the Arduino via ASCII signaling.
