# GUIgtkSocketC-

sudo apt-get install libmysqlcppconn-dev
sudo apt-get install libgtkmm-3.0-dev
sudo apt-get install libgtkmm-3.


g++ `pkg-config gtkmm-3.0 --cflags --libs` -c GUI_Socket.c
g++ GUI_Socket.o -o GUI_Socket `pkg-config gtkmm-3.0 --cflags --libs`
./GUI‗Socket
