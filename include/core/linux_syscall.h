#ifndef __CORE_LINUX_SYSCALL_H__
#define __CORE_LINUX_SYSCALL_H__

#include <core/types.h>
#include <core/asm.h>

enum x64_linux_syscall_no {
    LINUX_SYS_READ = 0,
    LINUX_SYS_WRITE = 1,
    LINUX_SYS_OPEN = 2,
    LINUX_SYS_CLOSE = 3,
    LINUX_SYS_STAT = 4,
    LINUX_SYS_FSTAT = 5,
    LINUX_SYS_LSTAT = 6,
    LINUX_SYS_POLL = 7,
    LINUX_SYS_LSEEK = 8,
    LINUX_SYS_MMAP = 9,
    LINUX_SYS_MPROTECT = 10,
    LINUX_SYS_MUNMAP = 11,
    LINUX_SYS_BRK = 12,
    LINUX_SYS_RT_SIGACTION = 13,
    LINUX_SYS_RT_SIGPROCMASK = 14,
    LINUX_SYS_RT_SIGRETURN = 15,
    LINUX_SYS_IOCTL = 16,
    LINUX_SYS_PREAD64 = 17,
    LINUX_SYS_PWRITE64 = 18,
    LINUX_SYS_READV = 19,
    LINUX_SYS_WRITEV = 20,
    LINUX_SYS_ACCESS = 21,
    LINUX_SYS_PIPE = 22,
    LINUX_SYS_SELECT = 23,
    LINUX_SYS_SCHED_YIELD = 24,
    LINUX_SYS_MREMAP = 25,
    LINUX_SYS_MSYNC = 26,
    LINUX_SYS_MINCORE = 27,
    LINUX_SYS_MADVISE = 28,
    LINUX_SYS_SHMGET = 29,
    LINUX_SYS_SHMAT = 30,
    LINUX_SYS_SHMCTL = 31,
    LINUX_SYS_DUP = 32,
    LINUX_SYS_DUP2 = 33,
    LINUX_SYS_PAUSE = 34,
    LINUX_SYS_NANOSLEEP = 35,
    LINUX_SYS_GETITIMER = 36,
    LINUX_SYS_ALARM = 37,
    LINUX_SYS_SETITIMER = 38,
    LINUX_SYS_GETPID = 39,
    LINUX_SYS_SENDFILE = 40,
    LINUX_SYS_SOCKET = 41,
    LINUX_SYS_CONNECT = 42,
    LINUX_SYS_ACCEPT = 43,
    LINUX_SYS_SENDTO = 44,
    LINUX_SYS_RECVFROM = 45,
    LINUX_SYS_SENDMSG = 46,
    LINUX_SYS_RECVMSG = 47,
    LINUX_SYS_SHUTDOWN = 48,
    LINUX_SYS_BIND = 49,
    LINUX_SYS_LISTEN = 50,
    LINUX_SYS_GETSOCKNAME = 51,
    LINUX_SYS_GETPEERNAME = 52,
    LINUX_SYS_SOCKETPAIR = 53,
    LINUX_SYS_SETSOCKOPT = 54,
    LINUX_SYS_GETSOCKOPT = 55,
    LINUX_SYS_CLONE = 56,
    LINUX_SYS_FORK = 57,
    LINUX_SYS_VFORK = 58,
    LINUX_SYS_EXECVE = 59,
    LINUX_SYS_EXIT = 60,
    LINUX_SYS_WAIT4 = 61,
    LINUX_SYS_KILL = 62,
    LINUX_SYS_UNAME = 63,
    LINUX_SYS_SEMGET = 64,
    LINUX_SYS_SEMOP = 65,
    LINUX_SYS_SEMCTL = 66,
    LINUX_SYS_SHMDT = 67,
    LINUX_SYS_MSGGET = 68,
    LINUX_SYS_MSGSND = 69,
    LINUX_SYS_MSGRCV = 70,
    LINUX_SYS_MSGCTL = 71,
    LINUX_SYS_FCNTL = 72,
    LINUX_SYS_FLOCK = 73,
    LINUX_SYS_FSYNC = 74,
    LINUX_SYS_FDATASYNC = 75,
    LINUX_SYS_TRUNCATE = 76,
    LINUX_SYS_FTRUNCATE = 77,
    LINUX_SYS_GETDENTS = 78,
    LINUX_SYS_GETCWD = 79,
    LINUX_SYS_CHDIR = 80,
    LINUX_SYS_FCHDIR = 81,
    LINUX_SYS_RENAME = 82,
    LINUX_SYS_MKDIR = 83,
    LINUX_SYS_RMDIR = 84,
    LINUX_SYS_CREAT = 85,
    LINUX_SYS_LINK = 86,
    LINUX_SYS_UNLINK = 87,
    LINUX_SYS_SYMLINK = 88,
    LINUX_SYS_READLINK = 89,
    LINUX_SYS_CHMOD = 90,
    LINUX_SYS_FCHMOD = 91,
    LINUX_SYS_CHOWN = 92,
    LINUX_SYS_FCHOWN = 93,
    LINUX_SYS_LCHOWN = 94,
    LINUX_SYS_UMASK = 95,
    LINUX_SYS_GETTIMEOFDAY = 96,
    LINUX_SYS_GETRLIMIT = 97,
    LINUX_SYS_GETRUSAGE = 98,
    LINUX_SYS_SYSINFO = 99,
    LINUX_SYS_TIMES = 100,
    LINUX_SYS_PTRACE = 101,
    LINUX_SYS_GETUID = 102,
    LINUX_SYS_SYSLOG = 103,
    LINUX_SYS_GETGID = 104,
    LINUX_SYS_SETUID = 105,
    LINUX_SYS_SETGID = 106,
    LINUX_SYS_GETEUID = 107,
    LINUX_SYS_GETEGID = 108,
    LINUX_SYS_SETPGID = 109,
    LINUX_SYS_GETPPID = 110,
    LINUX_SYS_GETPGRP = 111,
    LINUX_SYS_SETSID = 112,
    LINUX_SYS_SETREUID = 113,
    LINUX_SYS_SETREGID = 114,
    LINUX_SYS_GETGROUPS = 115,
    LINUX_SYS_SETGROUPS = 116,
    LINUX_SYS_SETRESUID = 117,
    LINUX_SYS_GETRESUID = 118,
    LINUX_SYS_SETRESGID = 119,
    LINUX_SYS_GETRESGID = 120,
    LINUX_SYS_GETPGID = 121,
    LINUX_SYS_SETFSUID = 122,
    LINUX_SYS_SETFSGID = 123,
    LINUX_SYS_GETSID = 124,
    LINUX_SYS_CAPGET = 125,
    LINUX_SYS_CAPSET = 126,
    LINUX_SYS_RT_SIGPENDING = 127,
    LINUX_SYS_RT_SIGTIMEDWAIT = 128,
    LINUX_SYS_RT_SIGQUEUEINFO = 129,
    LINUX_SYS_RT_SIGSUSPEND = 130,
    LINUX_SYS_SIGALTSTACK = 131,
    LINUX_SYS_UTIME = 132,
    LINUX_SYS_MKNOD = 133,
    LINUX_SYS_USELIB = 134,
    LINUX_SYS_PERSONALITY = 135,
    LINUX_SYS_USTAT = 136,
    LINUX_SYS_STATFS = 137,
    LINUX_SYS_FSTATFS = 138,
    LINUX_SYS_SYSFS = 139,
    LINUX_SYS_GETPRIORITY = 140,
    LINUX_SYS_SETPRIORITY = 141,
    LINUX_SYS_SCHED_SETPARAM = 142,
    LINUX_SYS_SCHED_GETPARAM = 143,
    LINUX_SYS_SCHED_SETSCHEDULER = 144,
    LINUX_SYS_SCHED_GETSCHEDULER = 145,
    LINUX_SYS_SCHED_GET_PRIORITY_MAX = 146,
    LINUX_SYS_SCHED_GET_PRIORITY_MIN = 147,
    LINUX_SYS_SCHED_RR_GET_INTERVAL = 148,
    LINUX_SYS_MLOCK = 149,
    LINUX_SYS_MUNLOCK = 150,
    LINUX_SYS_MLOCKALL = 151,
    LINUX_SYS_MUNLOCKALL = 152,
    LINUX_SYS_VHANGUP = 153,
    LINUX_SYS_MODIFY_LDT = 154,
    LINUX_SYS_PIVOT_ROOT = 155,
    LINUX_SYS__SYSCTL = 156,
    LINUX_SYS_PRCTL = 157,
    LINUX_SYS_ARCH_PRCTL = 158,
    LINUX_SYS_ADJTIMEX = 159,
    LINUX_SYS_SETRLIMIT = 160,
    LINUX_SYS_CHROOT = 161,
    LINUX_SYS_SYNC = 162,
    LINUX_SYS_ACCT = 163,
    LINUX_SYS_SETTIMEOFDAY = 164,
    LINUX_SYS_MOUNT = 165,
    LINUX_SYS_UMOUNT2 = 166,
    LINUX_SYS_SWAPON = 167,
    LINUX_SYS_SWAPOFF = 168,
    LINUX_SYS_REBOOT = 169,
    LINUX_SYS_SETHOSTNAME = 170,
    LINUX_SYS_SETDOMAINNAME = 171,
    LINUX_SYS_IOPL = 172,
    LINUX_SYS_IOPERM = 173,
    LINUX_SYS_CREATE_MODULE = 174,
    LINUX_SYS_INIT_MODULE = 175,
    LINUX_SYS_DELETE_MODULE = 176,
    LINUX_SYS_GET_KERNEL_SYMS = 177,
    LINUX_SYS_QUERY_MODULE = 178,
    LINUX_SYS_QUOTACTL = 179,
    LINUX_SYS_NFSSERVCTL = 180,
    LINUX_SYS_GETPMSG = 181,
    LINUX_SYS_PUTPMSG = 182,
    LINUX_SYS_AFS_SYSCALL = 183,
    LINUX_SYS_TUXCALL = 184,
    LINUX_SYS_SECURITY = 185,
    LINUX_SYS_GETTID = 186,
    LINUX_SYS_READAHEAD = 187,
    LINUX_SYS_SETXATTR = 188,
    LINUX_SYS_LSETXATTR = 189,
    LINUX_SYS_FSETXATTR = 190,
    LINUX_SYS_GETXATTR = 191,
    LINUX_SYS_LGETXATTR = 192,
    LINUX_SYS_FGETXATTR = 193,
    LINUX_SYS_LISTXATTR = 194,
    LINUX_SYS_LLISTXATTR = 195,
    LINUX_SYS_FLISTXATTR = 196,
    LINUX_SYS_REMOVEXATTR = 197,
    LINUX_SYS_LREMOVEXATTR = 198,
    LINUX_SYS_FREMOVEXATTR = 199,
    LINUX_SYS_TKILL = 200,
    LINUX_SYS_TIME = 201,
    LINUX_SYS_FUTEX = 202,
    LINUX_SYS_SCHED_SETAFFINITY = 203,
    LINUX_SYS_SCHED_GETAFFINITY = 204,
    LINUX_SYS_SET_THREAD_AREA = 205,
    LINUX_SYS_IO_SETUP = 206,
    LINUX_SYS_IO_DESTROY = 207,
    LINUX_SYS_IO_GETEVENTS = 208,
    LINUX_SYS_IO_SUBMIT = 209,
    LINUX_SYS_IO_CANCEL = 210,
    LINUX_SYS_GET_THREAD_AREA = 211,
    LINUX_SYS_LOOKUP_DCOOKIE = 212,
    LINUX_SYS_EPOLL_CREATE = 213,
    LINUX_SYS_EPOLL_CTL_OLD = 214,
    LINUX_SYS_EPOLL_WAIT_OLD = 215,
    LINUX_SYS_REMAP_FILE_PAGES = 216,
    LINUX_SYS_GETDENTS64 = 217,
    LINUX_SYS_SET_TID_ADDRESS = 218,
    LINUX_SYS_RESTART_SYSCALL = 219,
    LINUX_SYS_SEMTIMEDOP = 220,
    LINUX_SYS_FADVISE64 = 221,
    LINUX_SYS_TIMER_CREATE = 222,
    LINUX_SYS_TIMER_SETTIME = 223,
    LINUX_SYS_TIMER_GETTIME = 224,
    LINUX_SYS_TIMER_GETOVERRUN = 225,
    LINUX_SYS_TIMER_DELETE = 226,
    LINUX_SYS_CLOCK_SETTIME = 227,
    LINUX_SYS_CLOCK_GETTIME = 228,
    LINUX_SYS_CLOCK_GETRES = 229,
    LINUX_SYS_CLOCK_NANOSLEEP = 230,
    LINUX_SYS_EXIT_GROUP = 231,
    LINUX_SYS_EPOLL_WAIT = 232,
    LINUX_SYS_EPOLL_CTL = 233,
    LINUX_SYS_TGKILL = 234,
    LINUX_SYS_UTIMES = 235,
    LINUX_SYS_VSERVER = 236,
    LINUX_SYS_MBIND = 237,
    LINUX_SYS_SET_MEMPOLICY = 238,
    LINUX_SYS_GET_MEMPOLICY = 239,
    LINUX_SYS_MQ_OPEN = 240,
    LINUX_SYS_MQ_UNLINK = 241,
    LINUX_SYS_MQ_TIMEDSEND = 242,
    LINUX_SYS_MQ_TIMEDRECEIVE = 243,
    LINUX_SYS_MQ_NOTIFY = 244,
    LINUX_SYS_MQ_GETSETATTR = 245,
    LINUX_SYS_KEXEC_LOAD = 246,
    LINUX_SYS_WAITID = 247,
    LINUX_SYS_ADD_KEY = 248,
    LINUX_SYS_REQUEST_KEY = 249,
    LINUX_SYS_KEYCTL = 250,
    LINUX_SYS_IOPRIO_SET = 251,
    LINUX_SYS_IOPRIO_GET = 252,
    LINUX_SYS_INOTIFY_INIT = 253,
    LINUX_SYS_INOTIFY_ADD_WATCH = 254,
    LINUX_SYS_INOTIFY_RM_WATCH = 255,
    LINUX_SYS_MIGRATE_PAGES = 256,
    LINUX_SYS_OPENAT = 257,
    LINUX_SYS_MKDIRAT = 258,
    LINUX_SYS_MKNODAT = 259,
    LINUX_SYS_FCHOWNAT = 260,
    LINUX_SYS_FUTIMESAT = 261,
    LINUX_SYS_NEWFSTATAT = 262,
    LINUX_SYS_UNLINKAT = 263,
    LINUX_SYS_RENAMEAT = 264,
    LINUX_SYS_LINKAT = 265,
    LINUX_SYS_SYMLINKAT = 266,
    LINUX_SYS_READLINKAT = 267,
    LINUX_SYS_FCHMODAT = 268,
    LINUX_SYS_FACCESSAT = 269,
    LINUX_SYS_PSELECT6 = 270,
    LINUX_SYS_PPOLL = 271,
    LINUX_SYS_UNSHARE = 272,
    LINUX_SYS_SET_ROBUST_LIST = 273,
    LINUX_SYS_GET_ROBUST_LIST = 274,
    LINUX_SYS_SPLICE = 275,
    LINUX_SYS_TEE = 276,
    LINUX_SYS_SYNC_FILE_RANGE = 277,
    LINUX_SYS_VMSPLICE = 278,
    LINUX_SYS_MOVE_PAGES = 279,
    LINUX_SYS_UTIMENSAT = 280,
    LINUX_SYS_EPOLL_PWAIT = 281,
    LINUX_SYS_SIGNALFD = 282,
    LINUX_SYS_TIMERFD_CREATE = 283,
    LINUX_SYS_EVENTFD = 284,
    LINUX_SYS_FALLOCATE = 285,
    LINUX_SYS_TIMERFD_SETTIME = 286,
    LINUX_SYS_TIMERFD_GETTIME = 287,
    LINUX_SYS_ACCEPT4 = 288,
    LINUX_SYS_SIGNALFD4 = 289,
    LINUX_SYS_EVENTFD2 = 290,
    LINUX_SYS_EPOLL_CREATE1 = 291,
    LINUX_SYS_DUP3 = 292,
    LINUX_SYS_PIPE2 = 293,
    LINUX_SYS_INOTIFY_INIT1 = 294,
    LINUX_SYS_PREADV = 295,
    LINUX_SYS_PWRITEV = 296,
    LINUX_SYS_RT_TGSIGQUEUEINFO = 297,
    LINUX_SYS_PERF_EVENT_OPEN = 298,
    LINUX_SYS_RECVMMSG = 299,
    LINUX_SYS_FANOTIFY_INIT = 300,
    LINUX_SYS_FANOTIFY_MARK = 301,
    LINUX_SYS_PRLIMIT64 = 302,
    LINUX_SYS_NAME_TO_HANDLE_AT = 303,
    LINUX_SYS_OPEN_BY_HANDLE_AT = 304,
    LINUX_SYS_CLOCK_ADJTIME = 305,
    LINUX_SYS_SYNCFS = 306,
    LINUX_SYS_SENDMMSG = 307,
    LINUX_SYS_SETNS = 308,
    LINUX_SYS_GETCPU = 309,
    LINUX_SYS_PROCESS_VM_READV = 310,
    LINUX_SYS_PROCESS_VM_WRITEV = 311,
    LINUX_SYS_KCMP = 312,
    LINUX_SYS_FINIT_MODULE = 313,
    LINUX_SYS_SCHED_SETATTR = 314,
    LINUX_SYS_SCHED_GETATTR = 315,
    LINUX_SYS_RENAMEAT2 = 316,
    LINUX_SYS_SECCOMP = 317,
    LINUX_SYS_GETRANDOM = 318,
    LINUX_SYS_MEMFD_CREATE = 319,
    LINUX_SYS_KEXEC_FILE_LOAD = 320,
    LINUX_SYS_BPF = 321,
    LINUX_SYS_EXECVEAT = 322,
    LINUX_SYS_USERFAULTFD = 323,
    LINUX_SYS_MEMBARRIER = 324,
    LINUX_SYS_MLOCK2 = 325,
    LINUX_SYS_COPY_FILE_RANGE = 326,
    LINUX_SYS_PREADV2 = 327,
    LINUX_SYS_PWRITEV2 = 328,
    LINUX_SYS_PKEY_MPROTECT = 329,
    LINUX_SYS_PKEY_ALLOC = 330,
    LINUX_SYS_PKEY_FREE = 331,
    LINUX_SYS_STATX = 332,
    LINUX_SYS_IO_PGETEVENTS = 333,
    LINUX_SYS_RSEQ = 334,
    LINUX_SYS_URETPROBE = 335,
    LINUX_SYS_UPROBE = 336,
    LINUX_SYS_PIDFD_SEND_SIGNAL = 424,
    LINUX_SYS_IO_URING_SETUP = 425,
    LINUX_SYS_IO_URING_ENTER = 426,
    LINUX_SYS_IO_URING_REGISTER = 427,
    LINUX_SYS_OPEN_TREE = 428,
    LINUX_SYS_MOVE_MOUNT = 429,
    LINUX_SYS_FSOPEN = 430,
    LINUX_SYS_FSCONFIG = 431,
    LINUX_SYS_FSMOUNT = 432,
    LINUX_SYS_FSPICK = 433,
    LINUX_SYS_PIDFD_OPEN = 434,
    LINUX_SYS_CLONE3 = 435,
    LINUX_SYS_CLOSE_RANGE = 436,
    LINUX_SYS_OPENAT2 = 437,
    LINUX_SYS_PIDFD_GETFD = 438,
    LINUX_SYS_FACCESSAT2 = 439,
    LINUX_SYS_PROCESS_MADVISE = 440,
    LINUX_SYS_EPOLL_PWAIT2 = 441,
    LINUX_SYS_MOUNT_SETATTR = 442,
    LINUX_SYS_QUOTACTL_FD = 443,
    LINUX_SYS_LANDLOCK_CREATE_RULESET = 444,
    LINUX_SYS_LANDLOCK_ADD_RULE = 445,
    LINUX_SYS_LANDLOCK_RESTRICT_SELF = 446,
    LINUX_SYS_MEMFD_SECRET = 447,
    LINUX_SYS_PROCESS_MRELEASE = 448,
    LINUX_SYS_FUTEX_WAITV = 449,
    LINUX_SYS_SET_MEMPOLICY_HOME_NODE = 450,
    LINUX_SYS_CACHESTAT = 451,
    LINUX_SYS_FCHMODAT2 = 452,
    LINUX_SYS_MAP_SHADOW_STACK = 453,
    LINUX_SYS_FUTEX_WAKE = 454,
    LINUX_SYS_FUTEX_WAIT = 455,
    LINUX_SYS_FUTEX_REQUEUE = 456,
    LINUX_SYS_STATMOUNT = 457,
    LINUX_SYS_LISTMOUNT = 458,
    LINUX_SYS_LSM_GET_SELF_ATTR = 459,
    LINUX_SYS_LSM_SET_SELF_ATTR = 460,
    LINUX_SYS_LSM_LIST_MODULES = 461,
    LINUX_SYS_MSEAL = 462,
    LINUX_SYS_SETXATTRAT = 463,
    LINUX_SYS_GETXATTRAT = 464,
    LINUX_SYS_LISTXATTRAT = 465,
    LINUX_SYS_REMOVEXATTRAT = 466,
    LINUX_SYS_OPEN_TREE_ATTR = 467,
    LINUX_SYS_FILE_GETATTR = 468,
    LINUX_SYS_FILE_SETATTR = 469,
    LINUX_SYS_LISTNS = 470,
};

enum linux_fperm {
    LINUX_O_RDONLY      = 00,
    LINUX_O_WRONLY      = 01,
    LINUX_O_RDWR        = 02,
    LINUX_O_CREAT       = 0100,
    LINUX_O_DIRECTORY   = 0200000,
};

#define LINUX_SEEK_SET SEEK_SET
#define LINUX_SEEK_END SEEK_END
#define LINUX_SEEK_CUR SEEK_CUR

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

#define linux_read(fd, buff, size)                          linux_syscall3(LINUX_SYS_READ,            U64(fd), U64(buff), U64(size))
#define linux_write(fd, buff, size)                         linux_syscall3(LINUX_SYS_WRITE,           U64(fd), U64(buff), U64(size))
#define linux_open(path, flags)                             linux_syscall2(LINUX_SYS_OPEN,            U64(path), U64(flags))
#define linux_open_mode(path, flags, mode)                  linux_syscall3(LINUX_SYS_OPEN,            U64(path), U64(flags), U64(mode))
#define linux_close(fd)                                     linux_syscall1(LINUX_SYS_CLOSE,           U64(fd))

#define linux_mkdir(path, mode)                             linux_syscall2(LINUX_SYS_MKDIR,           U64(path), U64(mode))
#define linux_lseek(fd, offset, whence)                     linux_syscall3(LINUX_SYS_LSEEK,           U64(fd), U64(offset), U64(whence))
#define linux_mmap(addr, length, prot, flags, fd, offset)   PTR(linux_syscall6(LINUX_SYS_MMAP,        U64(addr), U64(length), U64(prot), U64(flags), U64(fd), U64(offset)))

#define linux_socket(family, domain, type)                  linux_syscall3(LINUX_SYS_SOCKET,          U64(family), U64(domain), U64(type))
#define linux_connect(sock, addr, len)                      linux_syscall3(LINUX_SYS_CONNECT,         U64(sock), U64(addr), U64(len))
#define linux_accept(sock, addr, len)                       linux_syscall3(LINUX_SYS_ACCEPT,          U64(sock), U64(addr), U64(len))
#define linux_send(sock, buff, size, flags)                 linux_syscall6(LINUX_SYS_SENDTO,          U64(sock), U64(buff), U64(size), U64(flags), 0, 0)
#define linux_recv(sock, buff, size, flags)                 linux_syscall6(LINUX_SYS_RECVFROM,        U64(sock), U64(buff), U64(size), U64(flags), 0, 0)

#define linux_shutdown(sock, how)                           linux_syscall2(LINUX_SYS_SHUTDOWN,        U64(sock), U64(how))
#define linux_bind(sock, addr, size)                        linux_syscall3(LINUX_SYS_BIND,            U64(sock), U64(addr), U64(size))
#define linux_listen(sock, len)                             linux_syscall2(LINUX_SYS_LISTEN,          U64(sock), U64(len))

#define linux_setsockopt(sock, level, opt, optval, len)     linux_syscall5(LINUX_SYS_SETSOCKOPT,      U64(sock), U64(level), U64(opt), U64(optval), U64(len))
#define linux_getsockopt(sock, level, opt, optval, len)     linux_syscall5(LINUX_SYS_GETSOCKOPT,      U64(sock), U64(level), U64(opt), U64(optval), U64(len))

#define linux_getrlimit(resource, rlim)                     linux_syscall2(LINUX_SYS_GETRLIMIT,       U64(resource), U64(rlim))
#define linux_clone3(clone_args, size)                      linux_syscall2(LINUX_SYS_CLONE3,          clone_args, size)
#define linux_waitpid(pid, status, options)                 linux_syscall3(LINUX_SYS_WAIT4,           U64(pid), U64(status), U64(options))

#define linux_clock_gettime(clk_id, tp)                     linux_syscall2(LINUX_SYS_CLOCK_GETTIME,   U64(clk_id), U64(tp))

#define linux_gettid()                                      linux_syscall0(LINUX_SYS_GETTID)
#define linux_getdents64(fd, dirp, count)                   linux_syscall3(LINUX_SYS_GETDENTS64,      U64(fd), U64(dirp), U64(count))

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
    LINUX_CSIGNAL = 0x000000ff,
    LINUX_CLONE_VM = 0x00000100,
    LINUX_CLONE_FS = 0x00000200,
    LINUX_CLONE_FILES = 0x00000400,
    LINUX_CLONE_SIGHAND = 0x00000800,
    LINUX_CLONE_PIDFD = 0x00001000,
    LINUX_CLONE_PTRACE = 0x00002000,
    LINUX_CLONE_VFORK = 0x00004000,
    LINUX_CLONE_PARENT = 0x00008000,
    LINUX_CLONE_THREAD = 0x00010000,
    LINUX_CLONE_NEWNS = 0x00020000,
    LINUX_CLONE_SYSVSEM = 0x00040000,
    LINUX_CLONE_SETTLS = 0x00080000,
    LINUX_CLONE_PARENT_SETTID = 0x00100000,
    LINUX_CLONE_CHILD_CLEARTID = 0x00200000,
    LINUX_CLONE_DETACHED = 0x00400000,
    LINUX_CLONE_UNTRACED = 0x00800000,
    LINUX_CLONE_CHILD_SETTID = 0x01000000,
    LINUX_CLONE_NEWCGROUP = 0x02000000,
    LINUX_CLONE_NEWUTS = 0x04000000,
    LINUX_CLONE_NEWIPC = 0x08000000,
    LINUX_CLONE_NEWUSER = 0x10000000,
    LINUX_CLONE_NEWPID = 0x20000000,
    LINUX_CLONE_NEWNET = 0x40000000,
    LINUX_CLONE_IO = 0x80000000,
};

enum linux_signals {
    LINUX_SIGHUP = 1,
    LINUX_SIGINT = 2,
    LINUX_SIGQUIT = 3,
    LINUX_SIGILL = 4,
    LINUX_SIGTRAP = 5,
    LINUX_SIGABRT = 6,
    LINUX_SIGBUS = 7,
    LINUX_SIGFPE = 8,
    LINUX_SIGKILL = 9,
    LINUX_SIGUSR1 = 10,
    LINUX_SIGSEGV = 11,
    LINUX_SIGUSR2 = 12,
    LINUX_SIGPIPE = 13,
    LINUX_SIGALRM = 14,
    LINUX_SIGTERM = 15,
    LINUX_SIGSTKFLT = 16,
    LINUX_SIGCHLD = 17,
    LINUX_SIGCONT = 18,
    LINUX_SIGSTOP = 19,
    LINUX_SIGTSTP = 20,
    LINUX_SIGTTIN = 21,
    LINUX_SIGTTOU = 22,
    LINUX_SIGURG = 23,
    LINUX_SIGXCPU = 24,
    LINUX_SIGXFSZ = 25,
    LINUX_SIGVTALRM = 26,
    LINUX_SIGPROF = 27,
    LINUX_SIGWINCH = 28,
    LINUX_SIGIO = 29,
    LINUX_SIGLOST = 29,
    LINUX_SIGPWR = 30,
    LINUX_SIGSYS = 31,
#define SIGIOT SIGABRT
#define SIGPOLL SIGIO
#define SIGUNUSED SIGSYS
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
    LINUX_SOCKET_AF_UNSPEC = 0,
    LINUX_SOCKET_AF_LOCAL = 1,
    LINUX_SOCKET_AF_INET = 2,
    LINUX_SOCKET_AF_INET6 = 10,
};

enum linux_socket_protocol_types {
    LINUX_SOCKET_PROTOCOL_TYPE_TCP = 6,
    LINUX_SOCKET_PROTOCOL_TYPE_UDP = 17,
};

enum linux_socket_types {
    LINUX_SOCKET_TYPE_STREAM = 1,
    LINUX_SOCKET_TYPE_DGRAM = 2,
    LINUX_SOCKET_TYPE_RAW = 3,
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
