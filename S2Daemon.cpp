#include "S2Daemon.h"
#include "Savage.h"

S2Daemon* g_pDaemon = NULL;

static void dump(uint8_t* buf, size_t len)
{
    static const size_t kBytesPerRow = 16;

    for(size_t i = 0; i < len; i += kBytesPerRow)
    {
        for(size_t j = 0; j < kBytesPerRow && (i+j) < len; j++)
        {
            printf("%02X ", buf[i+j]);
        }
        printf("  ");
        for(size_t j = 0; j < kBytesPerRow && (i+j) < len; j++)
        {
            printf("%c", buf[i+j] >= ' ' && buf[i+j] <= '~' ? buf[i+j] : '.');
        }

        printf("\r\n");
    }
}

S2Daemon::S2Daemon(void)
{
}
S2Daemon::~S2Daemon(void)
{
}

size_t S2Daemon::OnSendPacket(uint8_t* buf, size_t len)
{
    if(len >= 8)
    {
        Savage::S2Packet pkt(buf, len);
        if(pkt.CmdId() != 0x5B)
        {
            printf("Sent cmd 0x%02X\r\n", pkt.CmdId());
            dump(buf, len);
            printf("-----\r\n");
        }
    }

    return len;
}

size_t S2Daemon::OnReceivePacket(uint8_t* buf, size_t len)
{
    if(len >= 8)
    {
        Savage::S2Packet pkt(buf, len);
        switch(pkt.CmdId())
        {
            case Savage::ekGameData:
            {
                //printf("GAMEDATA PACKET\r\n-----\r\n");
                //dump(buf+7, len-7);
                //printf("-----\r\n");
            } break;
        }

        return pkt.Length();
    }

    //if(pkt.Length() >= 8)// && pkt.ReadDWord() == Savage::kS2Magic)
    //{
    //    uint32_t seq = pkt.ReadDWord();
    //    pkt.ReadByte(); /* Packet type */
    //    uint8_t uid = pkt.ReadWord();
    //    uint8_t cmd = pkt.ReadByte();
    //    //if(cmd != Savage::ekClientSnapshot)
    //    //{
    //        printf("<< cmd 0x%02X\r\n", cmd);
    //        dump(buf, len);
    //        printf("-----\r\n");
    //    //}
    //    switch(cmd)
    //    {
    //        case Savage::ekClientSnapshot:
    //        {
    //            //printf("Received client snapshot from %X...\r\n", uid);
    //        } break;
    //        case Savage::ekGameData:
    //        {
    //            printf("GAMEDATA PACKET\r\n-----\r\n");
    //            dump(buf, len);
    //            printf("-----\r\n");
    //        } break;
    //        case Savage::ekConnect:
    //        {
    //            printf("CONNECT PACKET\r\n-----\r\n");
    //            dump(buf, len);
    //            printf("-----\r\n");
    //        } break;
    //    };
    //}

    return len;
}
