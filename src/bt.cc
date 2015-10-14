#include <bt/bt.h>
#include <string>
#include <iostream>

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
        impl(const std::string &mac);
        ~impl();

        void connect();
        void disconnect();
        void list();
        bool isConnected() const;
        void write(std::string &cmd);
        void read(std::string &buf);

    private:
        int m_fd;
        std::string m_mac;
        void setParameters();
        void clear();
    };
}

// Implementation class
lib::bt::impl::impl(const std::string &mac)
    : m_fd(-1)
    , m_mac(mac)
{

 // allocate a socket
 m_fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
 if(m_fd == -1)
  throw std::runtime_error("Error, could not open serial port");

 connect();
}

lib::bt::impl::~impl()
{
 disconnect();
}

void lib::bt::impl::connect()
{
 // set the connection parameters (who to connect to)
 struct sockaddr_rc m_addr = {0};
 m_addr.rc_family = AF_BLUETOOTH;
 m_addr.rc_channel = (uint8_t) 1;
 str2ba( m_mac.c_str(), &m_addr.rc_bdaddr );
 
 if(::connect(m_fd, (struct sockaddr *)&m_addr, sizeof(m_addr)))
 {
  std::cerr << "Failed to connect the device!\n";
 }
}

void lib::bt::impl::disconnect()
{
 close(m_fd);
}

void lib::bt::impl::write(std::string &cmd)
{
 ssize_t bytes = ::write(m_fd, (char*)cmd.c_str(), sizeof(cmd));
 
 if (bytes == -1)
  throw std::runtime_error("Error, a problem occurs trying to write data");
 else if (bytes == 0)
  throw std::runtime_error("Error, could not write any data");
}

void lib::bt::impl::read(std::string &buf)
{
 char msg[18];
 int len = ::read(m_fd, msg, sizeof(msg));
 if( len>0 ) 
 {
  msg[len]=0; //
  std::cout << msg[0] << msg[1] << "\n";
 }
 buf = msg;
}

bool lib::bt::impl::isConnected() const
{
 return m_fd != -1;
}


// Public API
lib::bt::bt(const std::string &mac)
    : m_impl(new lib::bt::impl(mac))
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

void lib::bt::write(std::string &cmd)
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
