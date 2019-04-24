#include "GUI_Socket.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string.h> // memset
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <string>
using namespace std;

#define PORT "8888"
//#define BACKLOG 2
#define IP_ADDR "192.168.11.10"
#define MAXLEN 1024

int BACKLOG =0;
static unsigned int cli_count = 0;
static int uid = 20;
vector<int> cliarray;

Gui_Ex::Gui_Ex()
:
  // m_button1("Button 1"),
   m_adjustment( Gtk::Adjustment::create(1.0, 1.0, 5.0, 1.0, 5.0, 0.0) ),
   m_button2("Enter"),
   m_button3("Connect"),
   m_spin(m_adjustment),
   m_label("Choose # clients")


{
    set_title("Grid");
  set_border_width(12);


  m_grid.attach(m_label, 0,0,1,1); //column, row, width (# col span), height (# row span)
  m_grid.attach(m_spin, 1,0,1,1);
  m_spin.set_wrap();
  m_spin.set_numeric(true);

  m_grid.attach(m_button2, 2,0,1,1);

  m_grid.attach(m_button3, 0,1,3,1);
  m_button3.set_sensitive(false);

  m_button2.signal_clicked().connect(sigc::mem_fun(*this, &Gui_Ex::on_spin));
  m_button3.signal_clicked().connect(sigc::mem_fun(*this, &Gui_Ex::on_connect));
  //m_button3.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &Gui_Ex::on_connect), m_spin.get_value_as_int()));  

  add(m_grid);
  m_grid.show_all();
  //show_all_children();

}

Gui_Ex::~Gui_Ex()
{
}


void Gui_Ex::on_spin()
{
  cout<<"Spin value =: " << m_spin.get_value_as_int() << endl;
  BACKLOG = m_spin.get_value_as_int();
  m_spin.set_sensitive(false);
  m_button2.set_sensitive(false);
  m_button3.set_sensitive(true);

}

void Gui_Ex::on_connect()
{
    int connfd =0, n = 0;
    int *new_sock, sock;

    cout << BACKLOG << endl;
    pthread_t thread;
    struct addrinfo hints, *res;
    int reuseaddr = 1; // True 

    // Get the address info 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(IP_ADDR, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        //return 1;
    }

    // Create the socket 
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
       // return 1;
    }

    // Enable the socket to reuse the address 
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        Gui_Ex::close(sock);
       // return 1;
    }

    // Bind to the address 
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
       Gui_Ex:: close(sock);
        //return 0;
    }

    freeaddrinfo(res);

    // Listen 
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
       // return 0;
    }
    cout << "listening for connections" << endl;
    // Main loop 
    bool running = true;
    // Initialize clients 
    while (running)
    {  
      size_t size = sizeof(struct sockaddr_in);
      struct sockaddr_in their_addr;
      int clilen = sizeof(their_addr);
      int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
      if (newsock == -1) 
      {
        perror("accept");
       // return -1;
      }
      cli_count++;
      printf("Got a connection from %s on port %d\n", inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
      cliarray.push_back(newsock);
      if (cli_count == BACKLOG)
      {
         cout << "Max clients reached" << endl;
        running = false;
        break;
      }
    }
}
