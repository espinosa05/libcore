#ifndef __CORE_LINUX_H__
#define __CORE_LINUX_H__

#include <core/types.h>

#define FD_INVALID (-1)

const char *linux_errno_cstr(usz errno_val);
const char *linux_signal_str(int sig_num);

#endif /* __CORE_LINUX_H__ */
