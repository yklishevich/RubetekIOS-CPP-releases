#pragma once

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>


namespace dawn    {
namespace nix     {
namespace netstat {

    typedef union iaddr iaddr;

    union iaddr
    {
        unsigned      u    ;
        unsigned char b[4] ;
    };

    inline std::string state2str(unsigned state)
    {
        switch(state)
        {
            case 0x1 : return "ESTABLISHED" ;
            case 0x2 : return "SYN_SENT"    ;
            case 0x3 : return "SYN_RECV"    ;
            case 0x4 : return "FIN_WAIT1"   ;
            case 0x5 : return "FIN_WAIT2"   ;
            case 0x6 : return "TIME_WAIT"   ;
            case 0x7 : return "CLOSE"       ;
            case 0x8 : return "CLOSE_WAIT"  ;
            case 0x9 : return "LAST_ACK"    ;
            case 0xA : return "LISTEN"      ;
            case 0xB : return "CLOSING"     ;
            default  : return "UNKNOWN"     ;
        }
    }

    inline void addr2str(iaddr addr, unsigned port, char *buf)
    {
        if(port) snprintf(buf, 64, "%d.%d.%d.%d:%d", addr.b[0], addr.b[1], addr.b[2], addr.b[3], port);
        else     snprintf(buf, 64, "%d.%d.%d.%d:*", addr.b[0], addr.b[1], addr.b[2], addr.b[3]);
    }

    enum class proto
    {
          tcp
        , udp
    };

    inline void print_header(std::ostream& out = std::cout)
    {
        out << "Proto Recv-Q Send-Q Local Address          Foreign Address        State\n";
    }
    inline void print_proto(proto p, std::ostream& out = std::cout)
    {
        assert((p == proto::tcp) || (p == proto::udp));

        char buf[512];
        char lip[64];
        char rip[64];
        iaddr laddr, raddr;
        unsigned lport, rport, state, txq, rxq, num;
        int n;
        FILE* fp;

        fp = fopen(p == proto::tcp ? "/proc/net/tcp" : "/proc/net/udp", "r");
        if(fp != 0)
        {
            if (fgets(buf, 512, fp))
            {
                while(fgets(buf, 512, fp))
                {
                    n = sscanf(buf, " %d: %x:%x %x:%x %x %x:%x", &num, &laddr.u, &lport, &raddr.u, &rport, &state, &txq, &rxq);
                    if(n == 8)
                    {
                        addr2str(laddr, lport, lip);
                        addr2str(raddr, rport, rip);
                        if (p == proto::tcp) snprintf(buf, 512, "tcp   %6d %6d %-22s %-22s %s\n", txq, rxq, lip, rip, state2str(state).data());
                        else                 snprintf(buf, 512, "udp   %6d %6d %-22s %-22s\n"   , txq, rxq, lip, rip                         );
                        out << buf;
                    }
                }
            }
            fclose(fp);
        }
    }

    inline void print(std::ostream& out = std::cout)
    {
        print_header(out);
        print_proto(proto::tcp, out);
        print_proto(proto::udp, out);
    }

}}}
