#ifndef __CORE_LINUX_H__
#define __CORE_LINUX_H__

#include <core/types.h>

#define FD_INVALID (-1)

const char *linux_errno_cstr(usz errno_val);
const char *linux_signal_str(int sig_num);

/* special case because i don't like implementing it */
#define linux_stat  stat
#define linux_fstat fstat
#define linux_lstat lstat

#endif /* __CORE_LINUX_H__ */
