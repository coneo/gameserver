/*
 * Author: LiZhaojia 
 *
 * Last modified: 2014-09-29 19:06 +0800
 *
 * Description: 
 */

#include "endpoint.h"

#include "net_exception.h"
#include "../componet/scope_guard.h"
#include "../componet/tools.h"

#include <string>
#include <cstring>
#include <iostream>

#include <sys/socket.h>      //socket
#include <sys/ioctl.h>       //ioctl
#include <unistd.h>          //close
#include <linux/sockios.h>   //SIOCGIFADDR
#include <net/if.h>          //struct ifreq
#include <netinet/in.h>     //struct sockaddr_in

namespace water{
namespace net{

IpV4::IpV4(const std::string& ipStr)
{
    fromString(ipStr);
}

IpV4::IpV4(uint32_t ipValue)
:value(ipValue)
{
}

std::string IpV4::toString() const
{
    std::stringstream ss;
    ss
    << (int)bytes[0] << "." 
    << (int)bytes[1] << "." 
    << (int)bytes[2] << "." 
    << (int)bytes[3];

    return ss.str();
}

void IpV4::fromString(const std::string& str)
{
    std::vector<std::string> items;
    componet::splitString(&items, str, ".");
    items.resize(4);

    bytes[0] = componet::fromString<int>(items[0]);
    bytes[1] = componet::fromString<int>(items[1]);
    bytes[2] = componet::fromString<int>(items[2]);
    bytes[3] = componet::fromString<int>(items[3]);
}

IpV4 IpV4::getAddrByIfrName(const std::string& ifrName)
{
    int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
    ON_EXIT_SCOPE_DO( ::close(fd) );

    if(fd == -1)
        SYS_EXCEPTION(NetException, "::socket");

    struct ifreq ifr;
    std::memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    ifrName.copy(ifr.ifr_name, sizeof(ifr.ifr_name));

    if(::ioctl(fd, SIOCGIFADDR, &ifr) == -1)
        SYS_EXCEPTION(NetException, "::ioctl");

    IpV4 ip;
    ip.value = ((const struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr.s_addr;
    return ip;
}

std::ostream& operator << (std::ostream& os, const IpV4& ip)
{
    os << ip.toString();
    return os;
}

std::istream& operator >> (std::istream& is, IpV4& ip)
{
    std::string ipStr;
    is >> ipStr;
    ip.fromString(ipStr);
    return is;
}

namespace Tool
{

}

}} //namespace water::net


