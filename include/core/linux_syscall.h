#ifndef __CORE_LINUX_SYSCALL_H__
#define __CORE_LINUX_SYSCALL_H__

#include <core/types.h>
#include <core/asm.h>

enum x64_linux_syscall_no {
    SYS_READ = 0,
    SYS_WRITE = 1,
    SYS_OPEN = 2,
    SYS_CLOSE = 3,
    SYS_STAT = 4,
    SYS_FSTAT = 5,
    SYS_LSTAT = 6,
    SYS_POLL = 7,
    SYS_LSEEK = 8,
    SYS_MMAP = 9,
    SYS_MPROTECT = 10,
    SYS_MUNMAP = 11,
    SYS_BRK = 12,
    SYS_RT_SIGACTION = 13,
    SYS_RT_SIGPROCMASK = 14,
    SYS_RT_SIGRETURN = 15,
    SYS_IOCTL = 16,
    SYS_PREAD64 = 17,
    SYS_PWRITE64 = 18,
    SYS_READV = 19,
    SYS_WRITEV = 20,
    SYS_ACCESS = 21,
    SYS_PIPE = 22,
    SYS_SELECT = 23,
    SYS_SCHED_YIELD = 24,
    SYS_MREMAP = 25,
    SYS_MSYNC = 26,
    SYS_MINCORE = 27,
    SYS_MADVISE = 28,
    SYS_SHMGET = 29,
    SYS_SHMAT = 30,
    SYS_SHMCTL = 31,
    SYS_DUP = 32,
    SYS_DUP2 = 33,
    SYS_PAUSE = 34,
    SYS_NANOSLEEP = 35,
    SYS_GETITIMER = 36,
    SYS_ALARM = 37,
    SYS_SETITIMER = 38,
    SYS_GETPID = 39,
    SYS_SENDFILE = 40,
    SYS_SOCKET = 41,
    SYS_CONNECT = 42,
    SYS_ACCEPT = 43,
    SYS_SENDTO = 44,
    SYS_RECVFROM = 45,
    SYS_SENDMSG = 46,
    SYS_RECVMSG = 47,
    SYS_SHUTDOWN = 48,
    SYS_BIND = 49,
    SYS_LISTEN = 50,
    SYS_GETSOCKNAME = 51,
    SYS_GETPEERNAME = 52,
    SYS_SOCKETPAIR = 53,
    SYS_SETSOCKOPT = 54,
    SYS_GETSOCKOPT = 55,
    SYS_CLONE = 56,
    SYS_FORK = 57,
    SYS_VFORK = 58,
    SYS_EXECVE = 59,
    SYS_EXIT = 60,
    SYS_WAIT4 = 61,
    SYS_KILL = 62,
    SYS_UNAME = 63,
    SYS_SEMGET = 64,
    SYS_SEMOP = 65,
    SYS_SEMCTL = 66,
    SYS_SHMDT = 67,
    SYS_MSGGET = 68,
    SYS_MSGSND = 69,
    SYS_MSGRCV = 70,
    SYS_MSGCTL = 71,
    SYS_FCNTL = 72,
    SYS_FLOCK = 73,
    SYS_FSYNC = 74,
    SYS_FDATASYNC = 75,
    SYS_TRUNCATE = 76,
    SYS_FTRUNCATE = 77,
    SYS_GETDENTS = 78,
    SYS_GETCWD = 79,
    SYS_CHDIR = 80,
    SYS_FCHDIR = 81,
    SYS_RENAME = 82,
    SYS_MKDIR = 83,
    SYS_RMDIR = 84,
    SYS_CREAT = 85,
    SYS_LINK = 86,
    SYS_UNLINK = 87,
    SYS_SYMLINK = 88,
    SYS_READLINK = 89,
    SYS_CHMOD = 90,
    SYS_FCHMOD = 91,
    SYS_CHOWN = 92,
    SYS_FCHOWN = 93,
    SYS_LCHOWN = 94,
    SYS_UMASK = 95,
    SYS_GETTIMEOFDAY = 96,
    SYS_GETRLIMIT = 97,
    SYS_GETRUSAGE = 98,
    SYS_SYSINFO = 99,
    SYS_TIMES = 100,
    SYS_PTRACE = 101,
    SYS_GETUID = 102,
    SYS_SYSLOG = 103,
    SYS_GETGID = 104,
    SYS_SETUID = 105,
    SYS_SETGID = 106,
    SYS_GETEUID = 107,
    SYS_GETEGID = 108,
    SYS_SETPGID = 109,
    SYS_GETPPID = 110,
    SYS_GETPGRP = 111,
    SYS_SETSID = 112,
    SYS_SETREUID = 113,
    SYS_SETREGID = 114,
    SYS_GETGROUPS = 115,
    SYS_SETGROUPS = 116,
    SYS_SETRESUID = 117,
    SYS_GETRESUID = 118,
    SYS_SETRESGID = 119,
    SYS_GETRESGID = 120,
    SYS_GETPGID = 121,
    SYS_SETFSUID = 122,
    SYS_SETFSGID = 123,
    SYS_GETSID = 124,
    SYS_CAPGET = 125,
    SYS_CAPSET = 126,
    SYS_RT_SIGPENDING = 127,
    SYS_RT_SIGTIMEDWAIT = 128,
    SYS_RT_SIGQUEUEINFO = 129,
    SYS_RT_SIGSUSPEND = 130,
    SYS_SIGALTSTACK = 131,
    SYS_UTIME = 132,
    SYS_MKNOD = 133,
    SYS_USELIB = 134,
    SYS_PERSONALITY = 135,
    SYS_USTAT = 136,
    SYS_STATFS = 137,
    SYS_FSTATFS = 138,
    SYS_SYSFS = 139,
    SYS_GETPRIORITY = 140,
    SYS_SETPRIORITY = 141,
    SYS_SCHED_SETPARAM = 142,
    SYS_SCHED_GETPARAM = 143,
    SYS_SCHED_SETSCHEDULER = 144,
    SYS_SCHED_GETSCHEDULER = 145,
    SYS_SCHED_GET_PRIORITY_MAX = 146,
    SYS_SCHED_GET_PRIORITY_MIN = 147,
    SYS_SCHED_RR_GET_INTERVAL = 148,
    SYS_MLOCK = 149,
    SYS_MUNLOCK = 150,
    SYS_MLOCKALL = 151,
    SYS_MUNLOCKALL = 152,
    SYS_VHANGUP = 153,
    SYS_MODIFY_LDT = 154,
    SYS_PIVOT_ROOT = 155,
    SYS__SYSCTL = 156,
    SYS_PRCTL = 157,
    SYS_ARCH_PRCTL = 158,
    SYS_ADJTIMEX = 159,
    SYS_SETRLIMIT = 160,
    SYS_CHROOT = 161,
    SYS_SYNC = 162,
    SYS_ACCT = 163,
    SYS_SETTIMEOFDAY = 164,
    SYS_MOUNT = 165,
    SYS_UMOUNT2 = 166,
    SYS_SWAPON = 167,
    SYS_SWAPOFF = 168,
    SYS_REBOOT = 169,
    SYS_SETHOSTNAME = 170,
    SYS_SETDOMAINNAME = 171,
    SYS_IOPL = 172,
    SYS_IOPERM = 173,
    SYS_CREATE_MODULE = 174,
    SYS_INIT_MODULE = 175,
    SYS_DELETE_MODULE = 176,
    SYS_GET_KERNEL_SYMS = 177,
    SYS_QUERY_MODULE = 178,
    SYS_QUOTACTL = 179,
    SYS_NFSSERVCTL = 180,
    SYS_GETPMSG = 181,
    SYS_PUTPMSG = 182,
    SYS_AFS_SYSCALL = 183,
    SYS_TUXCALL = 184,
    SYS_SECURITY = 185,
    SYS_GETTID = 186,
    SYS_READAHEAD = 187,
    SYS_SETXATTR = 188,
    SYS_LSETXATTR = 189,
    SYS_FSETXATTR = 190,
    SYS_GETXATTR = 191,
    SYS_LGETXATTR = 192,
    SYS_FGETXATTR = 193,
    SYS_LISTXATTR = 194,
    SYS_LLISTXATTR = 195,
    SYS_FLISTXATTR = 196,
    SYS_REMOVEXATTR = 197,
    SYS_LREMOVEXATTR = 198,
    SYS_FREMOVEXATTR = 199,
    SYS_TKILL = 200,
    SYS_TIME = 201,
    SYS_FUTEX = 202,
    SYS_SCHED_SETAFFINITY = 203,
    SYS_SCHED_GETAFFINITY = 204,
    SYS_SET_THREAD_AREA = 205,
    SYS_IO_SETUP = 206,
    SYS_IO_DESTROY = 207,
    SYS_IO_GETEVENTS = 208,
    SYS_IO_SUBMIT = 209,
    SYS_IO_CANCEL = 210,
    SYS_GET_THREAD_AREA = 211,
    SYS_LOOKUP_DCOOKIE = 212,
    SYS_EPOLL_CREATE = 213,
    SYS_EPOLL_CTL_OLD = 214,
    SYS_EPOLL_WAIT_OLD = 215,
    SYS_REMAP_FILE_PAGES = 216,
    SYS_GETDENTS64 = 217,
    SYS_SET_TID_ADDRESS = 218,
    SYS_RESTART_SYSCALL = 219,
    SYS_SEMTIMEDOP = 220,
    SYS_FADVISE64 = 221,
    SYS_TIMER_CREATE = 222,
    SYS_TIMER_SETTIME = 223,
    SYS_TIMER_GETTIME = 224,
    SYS_TIMER_GETOVERRUN = 225,
    SYS_TIMER_DELETE = 226,
    SYS_CLOCK_SETTIME = 227,
    SYS_CLOCK_GETTIME = 228,
    SYS_CLOCK_GETRES = 229,
    SYS_CLOCK_NANOSLEEP = 230,
    SYS_EXIT_GROUP = 231,
    SYS_EPOLL_WAIT = 232,
    SYS_EPOLL_CTL = 233,
    SYS_TGKILL = 234,
    SYS_UTIMES = 235,
    SYS_VSERVER = 236,
    SYS_MBIND = 237,
    SYS_SET_MEMPOLICY = 238,
    SYS_GET_MEMPOLICY = 239,
    SYS_MQ_OPEN = 240,
    SYS_MQ_UNLINK = 241,
    SYS_MQ_TIMEDSEND = 242,
    SYS_MQ_TIMEDRECEIVE = 243,
    SYS_MQ_NOTIFY = 244,
    SYS_MQ_GETSETATTR = 245,
    SYS_KEXEC_LOAD = 246,
    SYS_WAITID = 247,
    SYS_ADD_KEY = 248,
    SYS_REQUEST_KEY = 249,
    SYS_KEYCTL = 250,
    SYS_IOPRIO_SET = 251,
    SYS_IOPRIO_GET = 252,
    SYS_INOTIFY_INIT = 253,
    SYS_INOTIFY_ADD_WATCH = 254,
    SYS_INOTIFY_RM_WATCH = 255,
    SYS_MIGRATE_PAGES = 256,
    SYS_OPENAT = 257,
    SYS_MKDIRAT = 258,
    SYS_MKNODAT = 259,
    SYS_FCHOWNAT = 260,
    SYS_FUTIMESAT = 261,
    SYS_NEWFSTATAT = 262,
    SYS_UNLINKAT = 263,
    SYS_RENAMEAT = 264,
    SYS_LINKAT = 265,
    SYS_SYMLINKAT = 266,
    SYS_READLINKAT = 267,
    SYS_FCHMODAT = 268,
    SYS_FACCESSAT = 269,
    SYS_PSELECT6 = 270,
    SYS_PPOLL = 271,
    SYS_UNSHARE = 272,
    SYS_SET_ROBUST_LIST = 273,
    SYS_GET_ROBUST_LIST = 274,
    SYS_SPLICE = 275,
    SYS_TEE = 276,
    SYS_SYNC_FILE_RANGE = 277,
    SYS_VMSPLICE = 278,
    SYS_MOVE_PAGES = 279,
    SYS_UTIMENSAT = 280,
    SYS_EPOLL_PWAIT = 281,
    SYS_SIGNALFD = 282,
    SYS_TIMERFD_CREATE = 283,
    SYS_EVENTFD = 284,
    SYS_FALLOCATE = 285,
    SYS_TIMERFD_SETTIME = 286,
    SYS_TIMERFD_GETTIME = 287,
    SYS_ACCEPT4 = 288,
    SYS_SIGNALFD4 = 289,
    SYS_EVENTFD2 = 290,
    SYS_EPOLL_CREATE1 = 291,
    SYS_DUP3 = 292,
    SYS_PIPE2 = 293,
    SYS_INOTIFY_INIT1 = 294,
    SYS_PREADV = 295,
    SYS_PWRITEV = 296,
    SYS_RT_TGSIGQUEUEINFO = 297,
    SYS_PERF_EVENT_OPEN = 298,
    SYS_RECVMMSG = 299,
    SYS_FANOTIFY_INIT = 300,
    SYS_FANOTIFY_MARK = 301,
    SYS_PRLIMIT64 = 302,
    SYS_NAME_TO_HANDLE_AT = 303,
    SYS_OPEN_BY_HANDLE_AT = 304,
    SYS_CLOCK_ADJTIME = 305,
    SYS_SYNCFS = 306,
    SYS_SENDMMSG = 307,
    SYS_SETNS = 308,
    SYS_GETCPU = 309,
    SYS_PROCESS_VM_READV = 310,
    SYS_PROCESS_VM_WRITEV = 311,
    SYS_KCMP = 312,
    SYS_FINIT_MODULE = 313,
    SYS_SCHED_SETATTR = 314,
    SYS_SCHED_GETATTR = 315,
    SYS_RENAMEAT2 = 316,
    SYS_SECCOMP = 317,
    SYS_GETRANDOM = 318,
    SYS_MEMFD_CREATE = 319,
    SYS_KEXEC_FILE_LOAD = 320,
    SYS_BPF = 321,
    SYS_EXECVEAT = 322,
    SYS_USERFAULTFD = 323,
    SYS_MEMBARRIER = 324,
    SYS_MLOCK2 = 325,
    SYS_COPY_FILE_RANGE = 326,
    SYS_PREADV2 = 327,
    SYS_PWRITEV2 = 328,
    SYS_PKEY_MPROTECT = 329,
    SYS_PKEY_ALLOC = 330,
    SYS_PKEY_FREE = 331,
    SYS_STATX = 332,
    SYS_IO_PGETEVENTS = 333,
    SYS_RSEQ = 334,
    SYS_URETPROBE = 335,
    SYS_UPROBE = 336,
    SYS_PIDFD_SEND_SIGNAL = 424,
    SYS_IO_URING_SETUP = 425,
    SYS_IO_URING_ENTER = 426,
    SYS_IO_URING_REGISTER = 427,
    SYS_OPEN_TREE = 428,
    SYS_MOVE_MOUNT = 429,
    SYS_FSOPEN = 430,
    SYS_FSCONFIG = 431,
    SYS_FSMOUNT = 432,
    SYS_FSPICK = 433,
    SYS_PIDFD_OPEN = 434,
    SYS_CLONE3 = 435,
    SYS_CLOSE_RANGE = 436,
    SYS_OPENAT2 = 437,
    SYS_PIDFD_GETFD = 438,
    SYS_FACCESSAT2 = 439,
    SYS_PROCESS_MADVISE = 440,
    SYS_EPOLL_PWAIT2 = 441,
    SYS_MOUNT_SETATTR = 442,
    SYS_QUOTACTL_FD = 443,
    SYS_LANDLOCK_CREATE_RULESET = 444,
    SYS_LANDLOCK_ADD_RULE = 445,
    SYS_LANDLOCK_RESTRICT_SELF = 446,
    SYS_MEMFD_SECRET = 447,
    SYS_PROCESS_MRELEASE = 448,
    SYS_FUTEX_WAITV = 449,
    SYS_SET_MEMPOLICY_HOME_NODE = 450,
    SYS_CACHESTAT = 451,
    SYS_FCHMODAT2 = 452,
    SYS_MAP_SHADOW_STACK = 453,
    SYS_FUTEX_WAKE = 454,
    SYS_FUTEX_WAIT = 455,
    SYS_FUTEX_REQUEUE = 456,
    SYS_STATMOUNT = 457,
    SYS_LISTMOUNT = 458,
    SYS_LSM_GET_SELF_ATTR = 459,
    SYS_LSM_SET_SELF_ATTR = 460,
    SYS_LSM_LIST_MODULES = 461,
    SYS_MSEAL = 462,
    SYS_SETXATTRAT = 463,
    SYS_GETXATTRAT = 464,
    SYS_LISTXATTRAT = 465,
    SYS_REMOVEXATTRAT = 466,
    SYS_OPEN_TREE_ATTR = 467,
    SYS_FILE_GETATTR = 468,
    SYS_FILE_SETATTR = 469,
    SYS_LISTNS = 470,
};

enum linux_fperm {
    LINUX_O_RDONLY      = 00,
    LINUX_O_WRONLY      = 01,
    LINUX_O_RDWR        = 02,
    LINUX_O_CREAT       = 0100,
    LINUX_O_DIRECTORY   = 0200000,
};

enum linux_stat_mode {
    LINUX_S_IFMT   = 0170000,

    LINUX_S_IFSOCK = 0140000,
    LINUX_S_IFLNK  = 0120000,
    LINUX_S_IFREG  = 0100000,
    LINUX_S_IFBLK  = 0060000,
    LINUX_S_IFDIR  = 0040000,
    LINUX_S_IFCHR  = 0020000,
    LINUX_S_IFIFO  = 0010000,

    LINUX_S_ISUID  = 0004000,
    LINUX_S_ISGID  = 0002000,
    LINUX_S_ISVTX  = 0001000,

    LINUX_S_IRUSR  = 0000400,
    LINUX_S_IWUSR  = 0000200,
    LINUX_S_IXUSR  = 0000100,

    LINUX_S_IRGRP  = 0000040,
    LINUX_S_IWGRP  = 0000020,
    LINUX_S_IXGRP  = 0000010,

    LINUX_S_IROTH  = 0000004,
    LINUX_S_IWOTH  = 0000002,
    LINUX_S_IXOTH  = 0000001,
};

#define linux_syscall0(n)                           \
({                                                  \
    usz _ret;                                       \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n)                                   \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall1(n, a1)                       \
({                                                  \
    usz _ret;                                       \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1)                                  \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall2(n, a1, a2)                   \
({                                                  \
    usz _ret;                                       \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1),                                 \
          "S" (a2)                                  \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall3(n, a1, a2, a3)               \
({                                                  \
    usz _ret;                                       \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1),                                 \
          "S" (a2),                                 \
          "d" (a3)                                  \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall4(n, a1, a2, a3, a4)           \
({                                                  \
    usz _ret;                                       \
    register u64 _r10 asm("r10") = (a4);            \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1),                                 \
          "S" (a2),                                 \
          "d" (a3),                                 \
          "r" (_r10)                                \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall5(n, a1, a2, a3, a4, a5)       \
({                                                  \
    usz _ret;                                       \
    register u64 _r10 asm("r10") = (a4);            \
    register u64 _r8  asm("r8")  = (a5);            \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1),                                 \
          "S" (a2),                                 \
          "d" (a3),                                 \
          "r" (_r10),                               \
          "r" (_r8)                                 \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_syscall6(n, a1, a2, a3, a4, a5, a6)   \
({                                                  \
    usz _ret;                                       \
    register u64 _r10 asm("r10") = (a4);            \
    register u64 _r8  asm("r8")  = (a5);            \
    register u64 _r9  asm("r9")  = (a6);            \
    asm volatile (                                  \
        ASM_STMT("syscall")                         \
        : "=a" (_ret)                               \
        : "a" (n),                                  \
          "D" (a1),                                 \
          "S" (a2),                                 \
          "d" (a3),                                 \
          "r" (_r10),                               \
          "r" (_r8),                                \
          "r" (_r9)                                 \
        : "rcx", "r11", "memory"                    \
    );                                              \
    _ret;                                           \
})

#define linux_read(fd, buff, size)                          linux_syscall3(SYS_READ,            U64(fd), U64(buff), U64(size))
#define linux_write(fd, buff, size)                         linux_syscall3(SYS_WRITE,           U64(fd), U64(buff), U64(size))
#define linux_open(path, flags)                             linux_syscall2(SYS_OPEN,            U64(path), U64(flags))
#define linux_open_mode(path, flags, mode)                  linux_syscall3(SYS_OPEN,            U64(path), U64(flags), U64(mode))
#define linux_close(fd)                                     linux_syscall1(SYS_CLOSE,           U64(fd))

#define linux_mmap(addr, length, prot, flags, fd, offset)   PTR(linux_syscall6(SYS_MMAP,        U64(addr), U64(length), U64(prot), U64(flags), U64(fd), U64(offset)))

#define linux_socket(family, domain, type)                  linux_syscall3(SYS_SOCKET,          U64(family), U64(domain), U64(type))
#define linux_connect(sock, addr, len)                      linux_syscall3(SYS_CONNECT,         U64(sock), U64(addr), U64(len))
#define linux_accept(sock, addr, len)                       linux_syscall3(SYS_ACCEPT,          U64(sock), U64(addr), U64(len))
#define linux_send(sock, buff, size, flags)                 linux_syscall6(SYS_SENDTO,          U64(sock), U64(buff), U64(size), U64(flags), 0, 0)
#define linux_recv(sock, buff, size, flags)                 linux_syscall6(SYS_RECVFROM,        U64(sock), U64(buff), U64(size), U64(flags), 0, 0)

#define linux_shutdown(sock, how)                           linux_syscall2(SYS_SHUTDOWN,        U64(sock), U64(how))
#define linux_bind(sock, addr, size)                        linux_syscall3(SYS_BIND,            U64(sock), U64(addr), U64(size))
#define linux_listen(sock, len)                             linux_syscall2(SYS_LISTEN,          U64(sock), U64(len))

#define linux_setsockopt(sock, level, opt, optval, len)     linux_syscall5(SYS_SETSOCKOPT,      U64(sock), U64(level), U64(opt), U64(optval), U64(len))
#define linux_getsockopt(sock, level, opt, optval, len)     linux_syscall5(SYS_GETSOCKOPT,      U64(sock), U64(level), U64(opt), U64(optval), U64(len))

#define linux_getrlimit(resource, rlim)                     linux_syscall2(SYS_GETRLIMIT,       U64(resource), U64(rlim))
#define linux_clone3(clone_args, size)                      linux_syscall2(SYS_CLONE3,          clone_args, size)
#define linux_waitpid(pid, status, options)                 linux_syscall3(SYS_WAIT4,           U64(pid), U64(status), U64(options))

#define linux_clock_gettime(clk_id, tp)                     linux_syscall2(SYS_CLOCK_GETTIME,   U64(clk_id), U64(tp))

#define linux_gettid()                                      linux_syscall0(SYS_GETTID)
#define linux_getdents64(fd, dirp, count)                   linux_syscall3(SYS_GETDENTS64,      U64(fd), U64(dirp), U64(count))

typedef typeof(void (int))  *sighandler_t;
sighandler_t signal(int, sighandler_t);
#define linux_signal(signum, handler) signal(signum, handler)

#define LINUX_MMAP_FAILURE(addr)    (S64(addr) < 0)
#define LINUX_MMAP_SUCCESS(addr)    !LINUX_MMAP_FAILURE(addr)

#define LINUX_SYSCALL_SUCCESS(rc)   ((rc) >= 0)
#define LINUX_SYSCALL_FAILURE(rc)   !LINUX_SYSCALL_SUCCESS(rc)

/* special data structures */
struct linux_rlimit {
    umax rlim_curr;
    umax rlim_max;
};

enum linux_clone_flags {
    LINUX_CSIGNAL                = 0x000000ff,
    LINUX_CLONE_VM               = 0x00000100,
    LINUX_CLONE_FS               = 0x00000200,
    LINUX_CLONE_FILES            = 0x00000400,
    LINUX_CLONE_SIGHAND          = 0x00000800,
    LINUX_CLONE_PIDFD            = 0x00001000,
    LINUX_CLONE_PTRACE           = 0x00002000,
    LINUX_CLONE_VFORK            = 0x00004000,
    LINUX_CLONE_PARENT           = 0x00008000,
    LINUX_CLONE_THREAD           = 0x00010000,
    LINUX_CLONE_NEWNS            = 0x00020000,
    LINUX_CLONE_SYSVSEM          = 0x00040000,
    LINUX_CLONE_SETTLS           = 0x00080000,
    LINUX_CLONE_PARENT_SETTID    = 0x00100000,
    LINUX_CLONE_CHILD_CLEARTID   = 0x00200000,
    LINUX_CLONE_DETACHED         = 0x00400000,
    LINUX_CLONE_UNTRACED         = 0x00800000,
    LINUX_CLONE_CHILD_SETTID     = 0x01000000,
    LINUX_CLONE_NEWCGROUP        = 0x02000000,
    LINUX_CLONE_NEWUTS           = 0x04000000,
    LINUX_CLONE_NEWIPC           = 0x08000000,
    LINUX_CLONE_NEWUSER          = 0x10000000,
    LINUX_CLONE_NEWPID           = 0x20000000,
    LINUX_CLONE_NEWNET           = 0x40000000,
    LINUX_CLONE_IO               = 0x80000000,
};

enum linux_signals {
    LINUX_SIGHUP    = 1,
    LINUX_SIGINT    = 2,
    LINUX_SIGQUIT   = 3,
    LINUX_SIGILL    = 4,
    LINUX_SIGTRAP   = 5,
    LINUX_SIGABRT   = 6,
    LINUX_SIGBUS    = 7,
    LINUX_SIGFPE    = 8,
    LINUX_SIGKILL   = 9,
    LINUX_SIGUSR1	= 10,
    LINUX_SIGSEGV	= 11,
    LINUX_SIGUSR2	= 12,
    LINUX_SIGPIPE	= 13,
    LINUX_SIGALRM	= 14,
    LINUX_SIGTERM	= 15,
    LINUX_SIGSTKFLT = 16,
    LINUX_SIGCHLD   = 17,
    LINUX_SIGCONT   = 18,
    LINUX_SIGSTOP   = 19,
    LINUX_SIGTSTP   = 20,
    LINUX_SIGTTIN   = 21,
    LINUX_SIGTTOU   = 22,
    LINUX_SIGURG    = 23,
    LINUX_SIGXCPU   = 24,
    LINUX_SIGXFSZ   = 25,
    LINUX_SIGVTALRM = 26,
    LINUX_SIGPROF   = 27,
    LINUX_SIGWINCH  = 28,
    LINUX_SIGIO     = 29,
    LINUX_SIGLOST   = 29,
    LINUX_SIGPWR    = 30,
    LINUX_SIGSYS    = 31,
#define SIGIOT      SIGABRT
#define SIGPOLL     SIGIO
#define SIGUNUSED   SIGSYS
    LINUX_SIGRTMIN = 34,
};

struct linux_clone_args {
    u64 flags;
    u64 pid_fd;
    u64 child_tid;
    u64 parent_tid;
    u64 exit_signal;
    u64 stack;
    u64 stack_size;
    u64 tls;
    u64 set_tid;
    u64 set_tid_size;
    u64 cgroup;
};

struct linux_dirent64 {
    u64     d_ino;
    s64     d_off;
    ushort  d_reclen;
    uchar   d_type;
    char    d_name[];
};

enum linux_address_family_types {
    LINUX_SOCKET_AF_UNSPEC  = 0,
    LINUX_SOCKET_AF_LOCAL   = 1,
    LINUX_SOCKET_AF_INET    = 2,
    LINUX_SOCKET_AF_INET6   = 10,
};

enum linux_socket_protocol_types {
    LINUX_SOCKET_PROTOCOL_TYPE_TCP = 6,
    LINUX_SOCKET_PROTOCOL_TYPE_UDP = 17,
};

enum linux_socket_types {
    LINUX_SOCKET_TYPE_STREAM    = 1,
    LINUX_SOCKET_TYPE_DGRAM     = 2,
    LINUX_SOCKET_TYPE_RAW       = 3,
};

struct linux_sockaddr_in {
    u32 family;
    u32 addr;
    u16 port;
};

#define LINUX_SOCKADDR_SIZE 16
struct linux_sockaddr {
    u8 pad[LINUX_SOCKADDR_SIZE];
};
#define LINUX_SOCKADDR(addr) ((struct linux_sockaddr *)addr)
#define LINUX_DIRENT64_REF(de) (struct linux_dirent64 *)(de)
#define get_next_dirent(linux_dirent)   LINUX_DIRENT64_REF(PTR(linux_dirent) + linux_dirent->d_reclen)

#endif /* __CORE_LINUX_SYSCALL_H__ */
