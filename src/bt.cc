#include <bt/bt.h>
#include <string>
#include <iostream>
#include <algorithm>    // std::remove

extern "C"
{
 #include <unistd.h>
 #include <sys/socket.h>
 #include <bluetooth/bluetooth.h>
 #include <bluetooth/rfcomm.h>
 #include <bluetooth/hci.h>
 #include <bluetooth/hci_lib.h>
}

namespace lib
{
    class bt::impl
    {
    public:
        impl(const std::string &mac, char eol);
        ~impl();

        void connect();
        void disconnect();
        void list();
        bool isConnected() const;
        void write(const std::string &cmd);
        void read(std::string &buf);

    private:
        int m_fd;
        char m_eol;
        std::string m_mac;
        void setParameters();
        void clear();
    };
}

// Implementation class
lib::bt::impl::impl(const std::string &mac, char eol)
    : m_fd(-1)
    , m_eol(eol)
    , m_mac(mac)
{

 // allocate a socket
 m_fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
 if(m_fd == -1)
  throw std::runtime_error("Error, could not open bluetooth socket");

 connect();
}

lib::bt::impl::~impl()
{
 disconnect();
}

void lib::bt::impl::connect()
{
 struct sockaddr_rc m_addr = {0};
 m_addr.rc_family = AF_BLUETOOTH;
 m_addr.rc_channel = (uint8_t) 1;
 str2ba( m_mac.c_str(), &m_addr.rc_bdaddr);

 if(::connect(m_fd, (struct sockaddr *)&m_addr, sizeof(m_addr)))
 {
  throw std::runtime_error("Error, could not connect to bluetooth device");
 }
 else
 {
  struct timeval tv;
  tv.tv_sec = 2;
  setsockopt(m_fd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
  setsockopt(m_fd, SOL_SOCKET, SO_SNDTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
 }
}

void lib::bt::impl::disconnect()
{
 close(m_fd);
}

void lib::bt::impl::write(const std::string &cmd)
{
 if (!isConnected())
  throw std::logic_error("Error, can not send data using a closed port");
 
 ssize_t bytes = ::write(m_fd, (char*)cmd.c_str(), cmd.size());
 
 if (bytes == -1)
  throw std::runtime_error("Error, a problem occurs trying to write data");
 else if (bytes == 0)
  throw std::runtime_error("Error, could not write any data");
}


void lib::bt::impl::read(std::string &buf)
{
 if (!isConnected())
  throw std::logic_error("Error, can not read data from a closed port");

 unsigned char c = 0x0;
 while (c != m_eol)
 {
  // A little bit hacking, but efficient
  if(::read(m_fd, &c, sizeof(char)) > 0 && c != 0x0A && c != 0x0D && c != '>')
   buf += c;
 }
}


bool lib::bt::impl::isConnected() const
{
 return m_fd != -1;
}

// Public API
lib::bt::bt(const std::string &mac, char eol)
    : m_impl(new lib::bt::impl(mac,eol))
{
}

lib::bt::~bt()
{
}

void lib::bt::connect()
{
 m_impl->connect();
}

void lib::bt::disconnect()
{
 m_impl->disconnect();
}

void lib::bt::write(const std::string &cmd)
{
 m_impl->write(cmd);
}

void lib::bt::read(std::string &buf)
{
 m_impl->read(buf);
}

bool lib::bt::isConnected() const
{
 return m_impl->isConnected();
}
