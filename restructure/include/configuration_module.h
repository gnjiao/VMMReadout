
#ifndef CONFIGURATION_MODULE_H
#define CONFIGURATION_MODULE_H

// vmm
#include "config_handler.h"
#include "socket_handler.h"

// Qt
#include <QString>


//////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------ //
//  Configuration
// ------------------------------------------------------------------------ //
//////////////////////////////////////////////////////////////////////////////

class Configuration : public QObject
{
    Q_OBJECT

    public :
        explicit Configuration(QObject *parent = 0);
        virtual ~Configuration(){};
        Configuration& setDebug(bool dbg) { m_dbg = dbg; return *this; }
        bool dbg() { return m_dbg; }

        Configuration& LoadConfig(ConfigHandler& config);
        Configuration& LoadSocket(SocketHandler& socket);

        void SendConfig();
        void fillGlobalRegisters(std::vector<QString>& globalRegisters);
        void fillChannelRegisters(std::vector<QString>& channelRegisters);
        SocketHandler& socket() { return *m_socketHandler; }
        ConfigHandler& config() { return *m_configHandler; }

    private :
        bool m_dbg;

        SocketHandler *m_socketHandler;
        ConfigHandler *m_configHandler;


}; // class Configuration



#endif
