1. In the source folder checkout and build mavlink
git clone https://github.com/mavlink/mavlink.git

2. build the roscopter package using rosmake

3. roslaunch 

$ roscd roscopter
$ nodes/roscopter.py --device=/dev/ttyACM0 --baudrate=115200 --enable-control=true
$ python arducopter.py
