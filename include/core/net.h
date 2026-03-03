#ifndef __CORE_NET_H__
#define __CORE_NET_H__

#include <core/types.h>

#define net_u8(x)  U8(x)
#define net_u16(x) __builtin_bswap16(U16(x))
#define net_u32(x) __builtin_bswap32(U32(x))
#define net_u64(x) __builtin_bswap64(U64(x))

struct net_address {
    u32 ipv4;
    u16 port;
};

#endif /* __CORE_NET_H__ */
