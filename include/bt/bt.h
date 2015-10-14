#ifndef LIB_BT_H
#define LIB_BT_H

#include <memory>

namespace lib
{
    class bt
    {
    public:
        bt(const std::string &mac);
        ~bt();

        void connect();
        void disconnect();
        void write(std::string &cmd);
        void read(std::string &buf);
        bool isConnected() const;

    private:
        class impl;
        std::unique_ptr<impl> m_impl;
    };
}
#endif
// LIB_BT_H
