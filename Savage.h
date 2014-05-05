#ifndef SAVAGE_H
#define SAVAGE_H

#include "Prerequisites.h"

namespace Savage
{
    const uint32_t S2Magic = 0xF197DE9A;

    class Packet
    {
    public:
        Packet(uint8_t* buf, size_t len)
        {
            m_Buffer = buf;
            m_Length = len;
            m_Pos = m_LastMove = 0;
        } 
        ~Packet(void)
        { }

        size_t Length(void) { return m_Length; }
        void Advance(int amount)
        {
            uint32_t newPos = max(0, min(m_Length, m_Pos + amount));
            m_LastMove = (int)(newPos - m_Pos);
            m_Pos = newPos;
        }
        void Back(void)
        {
            m_Pos -= m_LastMove;
            m_LastMove = 0;
        }

        template<typename T>
        T Read(void)
        {
            assert(m_Pos + sizeof(T) <= m_Length);
            T data = *(T*)&m_Buffer[m_Pos];
            Advance(sizeof(T));
            return data;
        }
        uint32_t ReadDWord(void)
        { return Read<uint32_t>(); }
        uint16_t ReadWord(void)
        { return Read<uint16_t>(); }
        uint8_t  ReadByte(void)
        { return Read<uint8_t>(); }
    private:
        uint32_t m_Pos;
        uint8_t* m_Buffer;
        size_t   m_Length;
        int      m_LastMove;        
    };

    enum ePacketIds
    {
        kClientSnapshot = 0xC7
    };
};

#endif
