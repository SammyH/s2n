#include "S2Daemon.h"
#include "Savage.h"

S2Daemon* g_pDaemon = NULL;

S2Daemon::S2Daemon(void)
{
}
S2Daemon::~S2Daemon(void)
{
}

size_t S2Daemon::OnReceivePacket(uint8_t* buf, size_t len)
{
    Savage::Packet pkt(buf, len);

    if(pkt.Length() >= 4 && pkt.ReadDWord() == Savage::S2Magic)
    {
        pkt.ReadByte(); /* unknown */
        uint8_t uid = pkt.ReadWord();
        uint8_t cmd = pkt.ReadByte();
        switch(cmd)
        {
            case Savage::kClientSnapshot:
            {
                printf("Received client snapshot from %X...\r\n", uid);
            } break;
        };
    }

    return pkt.Length();
}
