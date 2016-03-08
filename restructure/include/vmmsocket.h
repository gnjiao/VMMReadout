
// Qt
#include <QUdpSocket>

/////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------- //
//  VMMSocket
// ----------------------------------------------------------------------- //
/////////////////////////////////////////////////////////////////////////////

class VMMSocket : public QObject
{
    Q_OBJECT;

    public :
        explicit VMMSocket(QObject *parent = 0);
        virtual ~VMMSocket(){};

        VMMSocket& setDebug(bool dbg) { m_dbg = dbg; return *this; }
        bool dbg() { return m_dbg; }

        void bindSocket(quint16 port = 0,
            QAbstractSocket::BindMode mode = QAbstractSocket::DefaultForPlatform);
        quint64 writeDatagram(const QByteArray& data, const QHostAddress& host,
                    quint16 port);

        void TestUDP();
        void setName(std::string n = "") { m_name = n; }
        std::string getName() { return m_name; }
        void setBindingPort(quint16 port) { m_bindingPort = port; }
        quint16 getBindingPort() { return m_bindingPort; }

        bool checkAndReconnect(std::string fromWhere="");
        void closeAndDisconnect(std::string fromWhere="");

        QUdpSocket& socket() { return *m_socket; }
        QByteArray buffer() { return m_buffer; }
        void processReply(const QString &ip_sent_to, quint32 cmd_delay = 0,
                                quint32 count = 0);

        void Print();

    signals :
        void dataReady();

    public slots :

    private :
        bool m_dbg;
        std::string m_name;
        quint16 m_bindingPort;
        QUdpSocket *m_socket;
        QByteArray m_buffer;


}; // class VMMSocket