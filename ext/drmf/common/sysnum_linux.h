/* **********************************************************
 * Copyright (c) 2013 Google, Inc. All rights reserved.
 * Copyright (c) 2009 VMware, Inc. All rights reserved.
 * **********************************************************/

/* Dr. Memory: the memory debugger
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License, and no later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _SYSCALL_NUMS_H_
#define _SYSCALL_NUMS_H_ 1

/* Adapted from /usr/include/asm/unistd_64.h and
 * /usr/include/asm/unistd_32.h on Fedora 10
 */

#ifdef X64

#    define __NR_read 0
#    define __NR_write 1
#    define __NR_open 2
#    define __NR_close 3
#    define __NR_stat 4
#    define __NR_fstat 5
#    define __NR_lstat 6
#    define __NR_poll 7
#    define __NR_lseek 8
#    define __NR_mmap 9
#    define __NR_mprotect 10
#    define __NR_munmap 11
#    define __NR_brk 12
#    define __NR_rt_sigaction 13
#    define __NR_rt_sigprocmask 14
#    define __NR_rt_sigreturn 15
#    define __NR_ioctl 16
#    define __NR_pread64 17
#    define __NR_pwrite64 18
#    define __NR_readv 19
#    define __NR_writev 20
#    define __NR_access 21
#    define __NR_pipe 22
#    define __NR_select 23
#    define __NR_sched_yield 24
#    define __NR_mremap 25
#    define __NR_msync 26
#    define __NR_mincore 27
#    define __NR_madvise 28
#    define __NR_shmget 29
#    define __NR_shmat 30
#    define __NR_shmctl 31
#    define __NR_dup 32
#    define __NR_dup2 33
#    define __NR_pause 34
#    define __NR_nanosleep 35
#    define __NR_getitimer 36
#    define __NR_alarm 37
#    define __NR_setitimer 38
#    define __NR_getpid 39
#    define __NR_sendfile 40
#    define __NR_socket 41
#    define __NR_connect 42
#    define __NR_accept 43
#    define __NR_sendto 44
#    define __NR_recvfrom 45
#    define __NR_sendmsg 46
#    define __NR_recvmsg 47
#    define __NR_shutdown 48
#    define __NR_bind 49
#    define __NR_listen 50
#    define __NR_getsockname 51
#    define __NR_getpeername 52
#    define __NR_socketpair 53
#    define __NR_setsockopt 54
#    define __NR_getsockopt 55
#    define __NR_clone 56
#    define __NR_fork 57
#    define __NR_vfork 58
#    define __NR_execve 59
#    define __NR_exit 60
#    define __NR_wait4 61
#    define __NR_kill 62
#    define __NR_uname 63
#    define __NR_semget 64
#    define __NR_semop 65
#    define __NR_semctl 66
#    define __NR_shmdt 67
#    define __NR_msgget 68
#    define __NR_msgsnd 69
#    define __NR_msgrcv 70
#    define __NR_msgctl 71
#    define __NR_fcntl 72
#    define __NR_flock 73
#    define __NR_fsync 74
#    define __NR_fdatasync 75
#    define __NR_truncate 76
#    define __NR_ftruncate 77
#    define __NR_getdents 78
#    define __NR_getcwd 79
#    define __NR_chdir 80
#    define __NR_fchdir 81
#    define __NR_rename 82
#    define __NR_mkdir 83
#    define __NR_rmdir 84
#    define __NR_creat 85
#    define __NR_link 86
#    define __NR_unlink 87
#    define __NR_symlink 88
#    define __NR_readlink 89
#    define __NR_chmod 90
#    define __NR_fchmod 91
#    define __NR_chown 92
#    define __NR_fchown 93
#    define __NR_lchown 94
#    define __NR_umask 95
#    define __NR_gettimeofday 96
#    define __NR_getrlimit 97
#    define __NR_getrusage 98
#    define __NR_sysinfo 99
#    define __NR_times 100
#    define __NR_ptrace 101
#    define __NR_getuid 102
#    define __NR_syslog 103
/* at the very end the stuff that never runs during the benchmarks */
#    define __NR_getgid 104
#    define __NR_setuid 105
#    define __NR_setgid 106
#    define __NR_geteuid 107
#    define __NR_getegid 108
#    define __NR_setpgid 109
#    define __NR_getppid 110
#    define __NR_getpgrp 111
#    define __NR_setsid 112
#    define __NR_setreuid 113
#    define __NR_setregid 114
#    define __NR_getgroups 115
#    define __NR_setgroups 116
#    define __NR_setresuid 117
#    define __NR_getresuid 118
#    define __NR_setresgid 119
#    define __NR_getresgid 120
#    define __NR_getpgid 121
#    define __NR_setfsuid 122
#    define __NR_setfsgid 123
#    define __NR_getsid 124
#    define __NR_capget 125
#    define __NR_capset 126
#    define __NR_rt_sigpending 127
#    define __NR_rt_sigtimedwait 128
#    define __NR_rt_sigqueueinfo 129
#    define __NR_rt_sigsuspend 130
#    define __NR_sigaltstack 131
#    define __NR_utime 132
#    define __NR_mknod 133
/* Only needed for a.out */
#    define __NR_uselib 134
#    define __NR_personality 135
#    define __NR_ustat 136
#    define __NR_statfs 137
#    define __NR_fstatfs 138
#    define __NR_sysfs 139
#    define __NR_getpriority 140
#    define __NR_setpriority 141
#    define __NR_sched_setparam 142
#    define __NR_sched_getparam 143
#    define __NR_sched_setscheduler 144
#    define __NR_sched_getscheduler 145
#    define __NR_sched_get_priority_max 146
#    define __NR_sched_get_priority_min 147
#    define __NR_sched_rr_get_interval 148
#    define __NR_mlock 149
#    define __NR_munlock 150
#    define __NR_mlockall 151
#    define __NR_munlockall 152
#    define __NR_vhangup 153
#    define __NR_modify_ldt 154
#    define __NR_pivot_root 155
#    define __NR__sysctl 156
#    define __NR_prctl 157
#    define __NR_arch_prctl 158
#    define __NR_adjtimex 159
#    define __NR_setrlimit 160
#    define __NR_chroot 161
#    define __NR_sync 162
#    define __NR_acct 163
#    define __NR_settimeofday 164
#    define __NR_mount 165
#    define __NR_umount2 166
#    define __NR_swapon 167
#    define __NR_swapoff 168
#    define __NR_reboot 169
#    define __NR_sethostname 170
#    define __NR_setdomainname 171
#    define __NR_iopl 172
#    define __NR_ioperm 173
#    define __NR_create_module 174
#    define __NR_init_module 175
#    define __NR_delete_module 176
#    define __NR_get_kernel_syms 177
#    define __NR_query_module 178
#    define __NR_quotactl 179
#    define __NR_nfsservctl 180
/* reserved for LiS/STREAMS */
#    define __NR_getpmsg 181
#    define __NR_putpmsg 182
/* reserved for AFS */
#    define __NR_afs_syscall 183
/* reserved for tux */
#    define __NR_tuxcall 184
#    define __NR_security 185
#    define __NR_gettid 186
#    define __NR_readahead 187
#    define __NR_setxattr 188
#    define __NR_lsetxattr 189
#    define __NR_fsetxattr 190
#    define __NR_getxattr 191
#    define __NR_lgetxattr 192
#    define __NR_fgetxattr 193
#    define __NR_listxattr 194
#    define __NR_llistxattr 195
#    define __NR_flistxattr 196
#    define __NR_removexattr 197
#    define __NR_lremovexattr 198
#    define __NR_fremovexattr 199
#    define __NR_tkill 200
#    define __NR_time 201
#    define __NR_futex 202
#    define __NR_sched_setaffinity 203
#    define __NR_sched_getaffinity 204
#    define __NR_set_thread_area 205
#    define __NR_io_setup 206
#    define __NR_io_destroy 207
#    define __NR_io_getevents 208
#    define __NR_io_submit 209
#    define __NR_io_cancel 210
#    define __NR_get_thread_area 211
#    define __NR_lookup_dcookie 212
#    define __NR_epoll_create 213
#    define __NR_epoll_ctl_old 214
#    define __NR_epoll_wait_old 215
#    define __NR_remap_file_pages 216
#    define __NR_getdents64 217
#    define __NR_set_tid_address 218
#    define __NR_restart_syscall 219
#    define __NR_semtimedop 220
#    define __NR_fadvise64 221
#    define __NR_timer_create 222
#    define __NR_timer_settime 223
#    define __NR_timer_gettime 224
#    define __NR_timer_getoverrun 225
#    define __NR_timer_delete 226
#    define __NR_clock_settime 227
#    define __NR_clock_gettime 228
#    define __NR_clock_getres 229
#    define __NR_clock_nanosleep 230
#    define __NR_exit_group 231
#    define __NR_epoll_wait 232
#    define __NR_epoll_ctl 233
#    define __NR_tgkill 234
#    define __NR_utimes 235
#    define __NR_vserver 236
#    define __NR_mbind 237
#    define __NR_set_mempolicy 238
#    define __NR_get_mempolicy 239
#    define __NR_mq_open 240
#    define __NR_mq_unlink 241
#    define __NR_mq_timedsend 242
#    define __NR_mq_timedreceive 243
#    define __NR_mq_notify 244
#    define __NR_mq_getsetattr 245
#    define __NR_kexec_load 246
#    define __NR_waitid 247
#    define __NR_add_key 248
#    define __NR_request_key 249
#    define __NR_keyctl 250
#    define __NR_ioprio_set 251
#    define __NR_ioprio_get 252
#    define __NR_inotify_init 253
#    define __NR_inotify_add_watch 254
#    define __NR_inotify_rm_watch 255
#    define __NR_migrate_pages 256
#    define __NR_openat 257
#    define __NR_mkdirat 258
#    define __NR_mknodat 259
#    define __NR_fchownat 260
#    define __NR_futimesat 261
#    define __NR_newfstatat 262
#    define __NR_unlinkat 263
#    define __NR_renameat 264
#    define __NR_linkat 265
#    define __NR_symlinkat 266
#    define __NR_readlinkat 267
#    define __NR_fchmodat 268
#    define __NR_faccessat 269
#    define __NR_pselect6 270
#    define __NR_ppoll 271
#    define __NR_unshare 272
#    define __NR_set_robust_list 273
#    define __NR_get_robust_list 274
#    define __NR_splice 275
#    define __NR_tee 276
#    define __NR_sync_file_range 277
#    define __NR_vmsplice 278
#    define __NR_move_pages 279
#    define __NR_utimensat 280
#    define __IGNORE_getcpu /* implemented as a vsyscall */
#    define __NR_epoll_pwait 281
#    define __NR_signalfd 282
#    define __NR_timerfd_create 283
#    define __NR_eventfd 284
#    define __NR_fallocate 285
#    define __NR_timerfd_settime 286
#    define __NR_timerfd_gettime 287
#    define __NR_paccept 288
#    define __NR_signalfd4 289
#    define __NR_eventfd2 290
#    define __NR_epoll_create1 291
#    define __NR_dup3 292
#    define __NR_pipe2 293
#    define __NR_inotify_init1 294
#    define __NR_process_vm_readv 310
#    define __NR_process_vm_writev 311
#    define __NR_finit_module 313

#else /* X64 */

#    define __NR_restart_syscall 0
#    define __NR_exit 1
#    define __NR_fork 2
#    define __NR_read 3
#    define __NR_write 4
#    define __NR_open 5
#    define __NR_close 6
#    define __NR_waitpid 7
#    define __NR_creat 8
#    define __NR_link 9
#    define __NR_unlink 10
#    define __NR_execve 11
#    define __NR_chdir 12
#    define __NR_time 13
#    define __NR_mknod 14
#    define __NR_chmod 15
#    define __NR_lchown 16
#    define __NR_break 17
#    define __NR_oldstat 18
#    define __NR_lseek 19
#    define __NR_getpid 20
#    define __NR_mount 21
#    define __NR_umount 22
#    define __NR_setuid 23
#    define __NR_getuid 24
#    define __NR_stime 25
#    define __NR_ptrace 26
#    define __NR_alarm 27
#    define __NR_oldfstat 28
#    define __NR_pause 29
#    define __NR_utime 30
#    define __NR_stty 31
#    define __NR_gtty 32
#    define __NR_access 33
#    define __NR_nice 34
#    define __NR_ftime 35
#    define __NR_sync 36
#    define __NR_kill 37
#    define __NR_rename 38
#    define __NR_mkdir 39
#    define __NR_rmdir 40
#    define __NR_dup 41
#    define __NR_pipe 42
#    define __NR_times 43
#    define __NR_prof 44
#    define __NR_brk 45
#    define __NR_setgid 46
#    define __NR_getgid 47
#    define __NR_signal 48
#    define __NR_geteuid 49
#    define __NR_getegid 50
#    define __NR_acct 51
#    define __NR_umount2 52
#    define __NR_lock 53
#    define __NR_ioctl 54
#    define __NR_fcntl 55
#    define __NR_mpx 56
#    define __NR_setpgid 57
#    define __NR_ulimit 58
#    define __NR_oldolduname 59
#    define __NR_umask 60
#    define __NR_chroot 61
#    define __NR_ustat 62
#    define __NR_dup2 63
#    define __NR_getppid 64
#    define __NR_getpgrp 65
#    define __NR_setsid 66
#    define __NR_sigaction 67
#    define __NR_sgetmask 68
#    define __NR_ssetmask 69
#    define __NR_setreuid 70
#    define __NR_setregid 71
#    define __NR_sigsuspend 72
#    define __NR_sigpending 73
#    define __NR_sethostname 74
#    define __NR_setrlimit 75
#    define __NR_getrlimit 76 /* Back compatible 2Gig limited rlimit */
#    define __NR_getrusage 77
#    define __NR_gettimeofday 78
#    define __NR_settimeofday 79
#    define __NR_getgroups 80
#    define __NR_setgroups 81
#    define __NR_select 82
#    define __NR_symlink 83
#    define __NR_oldlstat 84
#    define __NR_readlink 85
#    define __NR_uselib 86
#    define __NR_swapon 87
#    define __NR_reboot 88
#    define __NR_readdir 89
#    define __NR_mmap 90
#    define __NR_munmap 91
#    define __NR_truncate 92
#    define __NR_ftruncate 93
#    define __NR_fchmod 94
#    define __NR_fchown 95
#    define __NR_getpriority 96
#    define __NR_setpriority 97
#    define __NR_profil 98
#    define __NR_statfs 99
#    define __NR_fstatfs 100
#    define __NR_ioperm 101
#    define __NR_socketcall 102
#    define __NR_syslog 103
#    define __NR_setitimer 104
#    define __NR_getitimer 105
#    define __NR_stat 106
#    define __NR_lstat 107
#    define __NR_fstat 108
#    define __NR_olduname 109
#    define __NR_iopl 110
#    define __NR_vhangup 111
#    define __NR_idle 112
#    define __NR_vm86old 113
#    define __NR_wait4 114
#    define __NR_swapoff 115
#    define __NR_sysinfo 116
#    define __NR_ipc 117
#    define __NR_fsync 118
#    define __NR_sigreturn 119
#    define __NR_clone 120
#    define __NR_setdomainname 121
#    define __NR_uname 122
#    define __NR_modify_ldt 123
#    define __NR_adjtimex 124
#    define __NR_mprotect 125
#    define __NR_sigprocmask 126
#    define __NR_create_module 127
#    define __NR_init_module 128
#    define __NR_delete_module 129
#    define __NR_get_kernel_syms 130
#    define __NR_quotactl 131
#    define __NR_getpgid 132
#    define __NR_fchdir 133
#    define __NR_bdflush 134
#    define __NR_sysfs 135
#    define __NR_personality 136
#    define __NR_afs_syscall 137 /* Syscall for Andrew File System */
#    define __NR_setfsuid 138
#    define __NR_setfsgid 139
#    define __NR__llseek 140
#    define __NR_getdents 141
#    define __NR__newselect 142
#    define __NR_flock 143
#    define __NR_msync 144
#    define __NR_readv 145
#    define __NR_writev 146
#    define __NR_getsid 147
#    define __NR_fdatasync 148
#    define __NR__sysctl 149
#    define __NR_mlock 150
#    define __NR_munlock 151
#    define __NR_mlockall 152
#    define __NR_munlockall 153
#    define __NR_sched_setparam 154
#    define __NR_sched_getparam 155
#    define __NR_sched_setscheduler 156
#    define __NR_sched_getscheduler 157
#    define __NR_sched_yield 158
#    define __NR_sched_get_priority_max 159
#    define __NR_sched_get_priority_min 160
#    define __NR_sched_rr_get_interval 161
#    define __NR_nanosleep 162
#    define __NR_mremap 163
#    define __NR_setresuid 164
#    define __NR_getresuid 165
#    define __NR_vm86 166
#    define __NR_query_module 167
#    define __NR_poll 168
#    define __NR_nfsservctl 169
#    define __NR_setresgid 170
#    define __NR_getresgid 171
#    define __NR_prctl 172
#    define __NR_rt_sigreturn 173
#    define __NR_rt_sigaction 174
#    define __NR_rt_sigprocmask 175
#    define __NR_rt_sigpending 176
#    define __NR_rt_sigtimedwait 177
#    define __NR_rt_sigqueueinfo 178
#    define __NR_rt_sigsuspend 179
#    define __NR_pread64 180
#    define __NR_pwrite64 181
#    define __NR_chown 182
#    define __NR_getcwd 183
#    define __NR_capget 184
#    define __NR_capset 185
#    define __NR_sigaltstack 186
#    define __NR_sendfile 187
#    define __NR_getpmsg 188 /* some people actually want streams */
#    define __NR_putpmsg 189 /* some people actually want streams */
#    define __NR_vfork 190
#    define __NR_ugetrlimit 191 /* SuS compliant getrlimit */
#    define __NR_mmap2 192
#    define __NR_truncate64 193
#    define __NR_ftruncate64 194
#    define __NR_stat64 195
#    define __NR_lstat64 196
#    define __NR_fstat64 197
#    define __NR_lchown32 198
#    define __NR_getuid32 199
#    define __NR_getgid32 200
#    define __NR_geteuid32 201
#    define __NR_getegid32 202
#    define __NR_setreuid32 203
#    define __NR_setregid32 204
#    define __NR_getgroups32 205
#    define __NR_setgroups32 206
#    define __NR_fchown32 207
#    define __NR_setresuid32 208
#    define __NR_getresuid32 209
#    define __NR_setresgid32 210
#    define __NR_getresgid32 211
#    define __NR_chown32 212
#    define __NR_setuid32 213
#    define __NR_setgid32 214
#    define __NR_setfsuid32 215
#    define __NR_setfsgid32 216
#    define __NR_pivot_root 217
#    define __NR_mincore 218
#    define __NR_madvise 219
#    define __NR_madvise1 219 /* delete when C lib stub is removed */
#    define __NR_getdents64 220
#    define __NR_fcntl64 221
/* 223 is unused */
#    define __NR_gettid 224
#    define __NR_readahead 225
#    define __NR_setxattr 226
#    define __NR_lsetxattr 227
#    define __NR_fsetxattr 228
#    define __NR_getxattr 229
#    define __NR_lgetxattr 230
#    define __NR_fgetxattr 231
#    define __NR_listxattr 232
#    define __NR_llistxattr 233
#    define __NR_flistxattr 234
#    define __NR_removexattr 235
#    define __NR_lremovexattr 236
#    define __NR_fremovexattr 237
#    define __NR_tkill 238
#    define __NR_sendfile64 239
#    define __NR_futex 240
#    define __NR_sched_setaffinity 241
#    define __NR_sched_getaffinity 242
#    define __NR_set_thread_area 243
#    define __NR_get_thread_area 244
#    define __NR_io_setup 245
#    define __NR_io_destroy 246
#    define __NR_io_getevents 247
#    define __NR_io_submit 248
#    define __NR_io_cancel 249
#    define __NR_fadvise64 250
/* 251 is available for reuse (was briefly sys_set_zone_reclaim) */
#    define __NR_exit_group 252
#    define __NR_lookup_dcookie 253
#    define __NR_epoll_create 254
#    define __NR_epoll_ctl 255
#    define __NR_epoll_wait 256
#    define __NR_remap_file_pages 257
#    define __NR_set_tid_address 258
#    define __NR_timer_create 259
#    define __NR_timer_settime (__NR_timer_create + 1)
#    define __NR_timer_gettime (__NR_timer_create + 2)
#    define __NR_timer_getoverrun (__NR_timer_create + 3)
#    define __NR_timer_delete (__NR_timer_create + 4)
#    define __NR_clock_settime (__NR_timer_create + 5)
#    define __NR_clock_gettime (__NR_timer_create + 6)
#    define __NR_clock_getres (__NR_timer_create + 7)
#    define __NR_clock_nanosleep (__NR_timer_create + 8)
#    define __NR_statfs64 268
#    define __NR_fstatfs64 269
#    define __NR_tgkill 270
#    define __NR_utimes 271
#    define __NR_fadvise64_64 272
#    define __NR_vserver 273
#    define __NR_mbind 274
#    define __NR_get_mempolicy 275
#    define __NR_set_mempolicy 276
#    define __NR_mq_open 277
#    define __NR_mq_unlink (__NR_mq_open + 1)
#    define __NR_mq_timedsend (__NR_mq_open + 2)
#    define __NR_mq_timedreceive (__NR_mq_open + 3)
#    define __NR_mq_notify (__NR_mq_open + 4)
#    define __NR_mq_getsetattr (__NR_mq_open + 5)
#    define __NR_kexec_load 283
#    define __NR_waitid 284
/* # define __NR_sys_setaltroot  285 */
#    define __NR_add_key 286
#    define __NR_request_key 287
#    define __NR_keyctl 288
#    define __NR_ioprio_set 289
#    define __NR_ioprio_get 290
#    define __NR_inotify_init 291
#    define __NR_inotify_add_watch 292
#    define __NR_inotify_rm_watch 293
#    define __NR_migrate_pages 294
#    define __NR_openat 295
#    define __NR_mkdirat 296
#    define __NR_mknodat 297
#    define __NR_fchownat 298
#    define __NR_futimesat 299
#    define __NR_fstatat64 300
#    define __NR_unlinkat 301
#    define __NR_renameat 302
#    define __NR_linkat 303
#    define __NR_symlinkat 304
#    define __NR_readlinkat 305
#    define __NR_fchmodat 306
#    define __NR_faccessat 307
#    define __NR_pselect6 308
#    define __NR_ppoll 309
#    define __NR_unshare 310
#    define __NR_set_robust_list 311
#    define __NR_get_robust_list 312
#    define __NR_splice 313
#    define __NR_sync_file_range 314
#    define __NR_tee 315
#    define __NR_vmsplice 316
#    define __NR_move_pages 317
#    define __NR_getcpu 318
#    define __NR_epoll_pwait 319
#    define __NR_utimensat 320
#    define __NR_signalfd 321
#    define __NR_timerfd_create 322
#    define __NR_eventfd 323
#    define __NR_fallocate 324
#    define __NR_timerfd_settime 325
#    define __NR_timerfd_gettime 326
#    define __NR_signalfd4 327
#    define __NR_eventfd2 328
#    define __NR_epoll_create1 329
#    define __NR_dup3 330
#    define __NR_pipe2 331
#    define __NR_inotify_init1 332
#    define __NR_process_vm_readv 347
#    define __NR_process_vm_writev 348
#    define __NR_finit_module 350

#endif /* X64 */

/* From Fedora 10's /usr/include/bits/syscall.h */
#define SYS__sysctl __NR__sysctl
#define SYS_access __NR_access
#define SYS_acct __NR_acct
#define SYS_add_key __NR_add_key
#define SYS_adjtimex __NR_adjtimex
#define SYS_afs_syscall __NR_afs_syscall
#define SYS_alarm __NR_alarm
#define SYS_brk __NR_brk
#define SYS_capget __NR_capget
#define SYS_capset __NR_capset
#define SYS_chdir __NR_chdir
#define SYS_chmod __NR_chmod
#define SYS_chown __NR_chown
#define SYS_chroot __NR_chroot
#define SYS_clock_getres __NR_clock_getres
#define SYS_clock_gettime __NR_clock_gettime
#define SYS_clock_nanosleep __NR_clock_nanosleep
#define SYS_clock_settime __NR_clock_settime
#define SYS_clone __NR_clone
#define SYS_close __NR_close
#define SYS_creat __NR_creat
#define SYS_create_module __NR_create_module
#define SYS_delete_module __NR_delete_module
#define SYS_dup __NR_dup
#define SYS_dup2 __NR_dup2
#define SYS_dup3 __NR_dup3
#define SYS_epoll_create __NR_epoll_create
#define SYS_epoll_create1 __NR_epoll_create1
#define SYS_epoll_ctl __NR_epoll_ctl
#define SYS_epoll_pwait __NR_epoll_pwait
#define SYS_epoll_wait __NR_epoll_wait
#define SYS_eventfd __NR_eventfd
#define SYS_eventfd2 __NR_eventfd2
#define SYS_execve __NR_execve
#define SYS_exit __NR_exit
#define SYS_exit_group __NR_exit_group
#define SYS_faccessat __NR_faccessat
#define SYS_fadvise64 __NR_fadvise64
#define SYS_fallocate __NR_fallocate
#define SYS_fchdir __NR_fchdir
#define SYS_fchmod __NR_fchmod
#define SYS_fchmodat __NR_fchmodat
#define SYS_fchown __NR_fchown
#define SYS_fchownat __NR_fchownat
#define SYS_fcntl __NR_fcntl
#define SYS_fdatasync __NR_fdatasync
#define SYS_fgetxattr __NR_fgetxattr
#define SYS_flistxattr __NR_flistxattr
#define SYS_flock __NR_flock
#define SYS_fork __NR_fork
#define SYS_fremovexattr __NR_fremovexattr
#define SYS_fsetxattr __NR_fsetxattr
#define SYS_fstat __NR_fstat
#define SYS_fstatfs __NR_fstatfs
#define SYS_fsync __NR_fsync
#define SYS_ftruncate __NR_ftruncate
#define SYS_futex __NR_futex
#define SYS_futimesat __NR_futimesat
#define SYS_get_kernel_syms __NR_get_kernel_syms
#define SYS_get_mempolicy __NR_get_mempolicy
#define SYS_get_robust_list __NR_get_robust_list
#define SYS_get_thread_area __NR_get_thread_area
#define SYS_getcwd __NR_getcwd
#define SYS_getdents __NR_getdents
#define SYS_getdents64 __NR_getdents64
#define SYS_getegid __NR_getegid
#define SYS_geteuid __NR_geteuid
#define SYS_getgid __NR_getgid
#define SYS_getgroups __NR_getgroups
#define SYS_getitimer __NR_getitimer
#define SYS_getpgid __NR_getpgid
#define SYS_getpgrp __NR_getpgrp
#define SYS_getpid __NR_getpid
#define SYS_getpmsg __NR_getpmsg
#define SYS_getppid __NR_getppid
#define SYS_getpriority __NR_getpriority
#define SYS_getresgid __NR_getresgid
#define SYS_getresuid __NR_getresuid
#define SYS_getrlimit __NR_getrlimit
#define SYS_getrusage __NR_getrusage
#define SYS_getsid __NR_getsid
#define SYS_gettid __NR_gettid
#define SYS_gettimeofday __NR_gettimeofday
#define SYS_getuid __NR_getuid
#define SYS_getxattr __NR_getxattr
#define SYS_init_module __NR_init_module
#define SYS_inotify_add_watch __NR_inotify_add_watch
#define SYS_inotify_init __NR_inotify_init
#define SYS_inotify_init1 __NR_inotify_init1
#define SYS_inotify_rm_watch __NR_inotify_rm_watch
#define SYS_io_cancel __NR_io_cancel
#define SYS_io_destroy __NR_io_destroy
#define SYS_io_getevents __NR_io_getevents
#define SYS_io_setup __NR_io_setup
#define SYS_io_submit __NR_io_submit
#define SYS_ioctl __NR_ioctl
#define SYS_ioperm __NR_ioperm
#define SYS_iopl __NR_iopl
#define SYS_ioprio_get __NR_ioprio_get
#define SYS_ioprio_set __NR_ioprio_set
#define SYS_kexec_load __NR_kexec_load
#define SYS_keyctl __NR_keyctl
#define SYS_kill __NR_kill
#define SYS_lchown __NR_lchown
#define SYS_lgetxattr __NR_lgetxattr
#define SYS_link __NR_link
#define SYS_linkat __NR_linkat
#define SYS_listxattr __NR_listxattr
#define SYS_llistxattr __NR_llistxattr
#define SYS_lookup_dcookie __NR_lookup_dcookie
#define SYS_lremovexattr __NR_lremovexattr
#define SYS_lseek __NR_lseek
#define SYS_lsetxattr __NR_lsetxattr
#define SYS_lstat __NR_lstat
#define SYS_madvise __NR_madvise
#define SYS_mbind __NR_mbind
#define SYS_migrate_pages __NR_migrate_pages
#define SYS_mincore __NR_mincore
#define SYS_mkdir __NR_mkdir
#define SYS_mkdirat __NR_mkdirat
#define SYS_mknod __NR_mknod
#define SYS_mknodat __NR_mknodat
#define SYS_mlock __NR_mlock
#define SYS_mlockall __NR_mlockall
#define SYS_mmap __NR_mmap
#define SYS_modify_ldt __NR_modify_ldt
#define SYS_mount __NR_mount
#define SYS_move_pages __NR_move_pages
#define SYS_mprotect __NR_mprotect
#define SYS_mq_getsetattr __NR_mq_getsetattr
#define SYS_mq_notify __NR_mq_notify
#define SYS_mq_open __NR_mq_open
#define SYS_mq_timedreceive __NR_mq_timedreceive
#define SYS_mq_timedsend __NR_mq_timedsend
#define SYS_mq_unlink __NR_mq_unlink
#define SYS_mremap __NR_mremap
#define SYS_msync __NR_msync
#define SYS_munlock __NR_munlock
#define SYS_munlockall __NR_munlockall
#define SYS_munmap __NR_munmap
#define SYS_nanosleep __NR_nanosleep
#define SYS_nfsservctl __NR_nfsservctl
#define SYS_open __NR_open
#define SYS_openat __NR_openat
#define SYS_pause __NR_pause
#define SYS_personality __NR_personality
#define SYS_pipe __NR_pipe
#define SYS_pipe2 __NR_pipe2
#define SYS_pivot_root __NR_pivot_root
#define SYS_poll __NR_poll
#define SYS_ppoll __NR_ppoll
#define SYS_prctl __NR_prctl
#define SYS_pread64 __NR_pread64
#define SYS_pselect6 __NR_pselect6
#define SYS_ptrace __NR_ptrace
#define SYS_putpmsg __NR_putpmsg
#define SYS_pwrite64 __NR_pwrite64
#define SYS_query_module __NR_query_module
#define SYS_quotactl __NR_quotactl
#define SYS_read __NR_read
#define SYS_readahead __NR_readahead
#define SYS_readlink __NR_readlink
#define SYS_readlinkat __NR_readlinkat
#define SYS_readv __NR_readv
#define SYS_reboot __NR_reboot
#define SYS_remap_file_pages __NR_remap_file_pages
#define SYS_removexattr __NR_removexattr
#define SYS_rename __NR_rename
#define SYS_renameat __NR_renameat
#define SYS_request_key __NR_request_key
#define SYS_restart_syscall __NR_restart_syscall
#define SYS_rmdir __NR_rmdir
#define SYS_rt_sigaction __NR_rt_sigaction
#define SYS_rt_sigpending __NR_rt_sigpending
#define SYS_rt_sigprocmask __NR_rt_sigprocmask
#define SYS_rt_sigqueueinfo __NR_rt_sigqueueinfo
#define SYS_rt_sigreturn __NR_rt_sigreturn
#define SYS_rt_sigsuspend __NR_rt_sigsuspend
#define SYS_rt_sigtimedwait __NR_rt_sigtimedwait
#define SYS_sched_get_priority_max __NR_sched_get_priority_max
#define SYS_sched_get_priority_min __NR_sched_get_priority_min
#define SYS_sched_getaffinity __NR_sched_getaffinity
#define SYS_sched_getparam __NR_sched_getparam
#define SYS_sched_getscheduler __NR_sched_getscheduler
#define SYS_sched_rr_get_interval __NR_sched_rr_get_interval
#define SYS_sched_setaffinity __NR_sched_setaffinity
#define SYS_sched_setparam __NR_sched_setparam
#define SYS_sched_setscheduler __NR_sched_setscheduler
#define SYS_sched_yield __NR_sched_yield
#define SYS_select __NR_select
#define SYS_sendfile __NR_sendfile
#define SYS_set_mempolicy __NR_set_mempolicy
#define SYS_set_robust_list __NR_set_robust_list
#define SYS_set_thread_area __NR_set_thread_area
#define SYS_set_tid_address __NR_set_tid_address
#define SYS_setdomainname __NR_setdomainname
#define SYS_setfsgid __NR_setfsgid
#define SYS_setfsuid __NR_setfsuid
#define SYS_setgid __NR_setgid
#define SYS_setgroups __NR_setgroups
#define SYS_sethostname __NR_sethostname
#define SYS_setitimer __NR_setitimer
#define SYS_setpgid __NR_setpgid
#define SYS_setpriority __NR_setpriority
#define SYS_setregid __NR_setregid
#define SYS_setresgid __NR_setresgid
#define SYS_setresuid __NR_setresuid
#define SYS_setreuid __NR_setreuid
#define SYS_setrlimit __NR_setrlimit
#define SYS_setsid __NR_setsid
#define SYS_settimeofday __NR_settimeofday
#define SYS_setuid __NR_setuid
#define SYS_setxattr __NR_setxattr
#define SYS_sigaltstack __NR_sigaltstack
#define SYS_signalfd __NR_signalfd
#define SYS_signalfd4 __NR_signalfd4
#define SYS_splice __NR_splice
#define SYS_stat __NR_stat
#define SYS_statfs __NR_statfs
#define SYS_swapoff __NR_swapoff
#define SYS_swapon __NR_swapon
#define SYS_symlink __NR_symlink
#define SYS_symlinkat __NR_symlinkat
#define SYS_sync __NR_sync
#define SYS_sync_file_range __NR_sync_file_range
#define SYS_sysfs __NR_sysfs
#define SYS_sysinfo __NR_sysinfo
#define SYS_syslog __NR_syslog
#define SYS_tee __NR_tee
#define SYS_tgkill __NR_tgkill
#define SYS_time __NR_time
#define SYS_timer_create __NR_timer_create
#define SYS_timer_delete __NR_timer_delete
#define SYS_timer_getoverrun __NR_timer_getoverrun
#define SYS_timer_gettime __NR_timer_gettime
#define SYS_timer_settime __NR_timer_settime
#define SYS_timerfd_create __NR_timerfd_create
#define SYS_timerfd_gettime __NR_timerfd_gettime
#define SYS_timerfd_settime __NR_timerfd_settime
#define SYS_times __NR_times
#define SYS_tkill __NR_tkill
#define SYS_truncate __NR_truncate
#define SYS_umask __NR_umask
#define SYS_umount2 __NR_umount2
#define SYS_uname __NR_uname
#define SYS_unlink __NR_unlink
#define SYS_unlinkat __NR_unlinkat
#define SYS_unshare __NR_unshare
#define SYS_uselib __NR_uselib
#define SYS_ustat __NR_ustat
#define SYS_utime __NR_utime
#define SYS_utimensat __NR_utimensat
#define SYS_utimes __NR_utimes
#define SYS_vfork __NR_vfork
#define SYS_vhangup __NR_vhangup
#define SYS_vmsplice __NR_vmsplice
#define SYS_vserver __NR_vserver
#define SYS_wait4 __NR_wait4
#define SYS_waitid __NR_waitid
#define SYS_write __NR_write
#define SYS_writev __NR_writev
#define SYS_process_vm_readv __NR_process_vm_readv
#define SYS_process_vm_writev __NR_process_vm_writev
#define SYS_finit_module __NR_finit_module

#ifdef X64
#    define SYS_accept __NR_accept
#    define SYS_arch_prctl __NR_arch_prctl
#    define SYS_bind __NR_bind
#    define SYS_connect __NR_connect
#    define SYS_epoll_ctl_old __NR_epoll_ctl_old
#    define SYS_epoll_wait_old __NR_epoll_wait_old
#    define SYS_getpeername __NR_getpeername
#    define SYS_getsockname __NR_getsockname
#    define SYS_getsockopt __NR_getsockopt
#    define SYS_listen __NR_listen
#    define SYS_msgctl __NR_msgctl
#    define SYS_msgget __NR_msgget
#    define SYS_msgrcv __NR_msgrcv
#    define SYS_msgsnd __NR_msgsnd
#    define SYS_newfstatat __NR_newfstatat
#    define SYS_paccept __NR_paccept
#    define SYS_recvfrom __NR_recvfrom
#    define SYS_recvmsg __NR_recvmsg
#    define SYS_security __NR_security
#    define SYS_semctl __NR_semctl
#    define SYS_semget __NR_semget
#    define SYS_semop __NR_semop
#    define SYS_semtimedop __NR_semtimedop
#    define SYS_sendmsg __NR_sendmsg
#    define SYS_sendto __NR_sendto
#    define SYS_setsockopt __NR_setsockopt
#    define SYS_shmat __NR_shmat
#    define SYS_shmctl __NR_shmctl
#    define SYS_shmdt __NR_shmdt
#    define SYS_shmget __NR_shmget
#    define SYS_shutdown __NR_shutdown
#    define SYS_socket __NR_socket
#    define SYS_socketpair __NR_socketpair
#    define SYS_tuxcall __NR_tuxcall
#else
#    define SYS__llseek __NR__llseek
#    define SYS__newselect __NR__newselect
#    define SYS_bdflush __NR_bdflush
#    define SYS_break __NR_break
#    define SYS_chown32 __NR_chown32
#    define SYS_fadvise64_64 __NR_fadvise64_64
#    define SYS_fchown32 __NR_fchown32
#    define SYS_fcntl64 __NR_fcntl64
#    define SYS_fstat64 __NR_fstat64
#    define SYS_fstatat64 __NR_fstatat64
#    define SYS_fstatfs64 __NR_fstatfs64
#    define SYS_ftime __NR_ftime
#    define SYS_ftruncate64 __NR_ftruncate64
#    define SYS_getcpu __NR_getcpu
#    define SYS_getegid32 __NR_getegid32
#    define SYS_geteuid32 __NR_geteuid32
#    define SYS_getgid32 __NR_getgid32
#    define SYS_getgroups32 __NR_getgroups32
#    define SYS_getresgid32 __NR_getresgid32
#    define SYS_getresuid32 __NR_getresuid32
#    define SYS_getuid32 __NR_getuid32
#    define SYS_gtty __NR_gtty
#    define SYS_idle __NR_idle
#    define SYS_ipc __NR_ipc
#    define SYS_lchown32 __NR_lchown32
#    define SYS_lock __NR_lock
#    define SYS_lstat64 __NR_lstat64
#    define SYS_madvise1 __NR_madvise1
#    define SYS_mmap2 __NR_mmap2
#    define SYS_mpx __NR_mpx
#    define SYS_nice __NR_nice
#    define SYS_oldfstat __NR_oldfstat
#    define SYS_oldlstat __NR_oldlstat
#    define SYS_oldolduname __NR_oldolduname
#    define SYS_oldstat __NR_oldstat
#    define SYS_olduname __NR_olduname
#    define SYS_prof __NR_prof
#    define SYS_profil __NR_profil
#    define SYS_readdir __NR_readdir
#    define SYS_sendfile64 __NR_sendfile64
#    define SYS_setfsgid32 __NR_setfsgid32
#    define SYS_setfsuid32 __NR_setfsuid32
#    define SYS_setgid32 __NR_setgid32
#    define SYS_setgroups32 __NR_setgroups32
#    define SYS_setregid32 __NR_setregid32
#    define SYS_setresgid32 __NR_setresgid32
#    define SYS_setresuid32 __NR_setresuid32
#    define SYS_setreuid32 __NR_setreuid32
#    define SYS_setuid32 __NR_setuid32
#    define SYS_sgetmask __NR_sgetmask
#    define SYS_sigaction __NR_sigaction
#    define SYS_signal __NR_signal
#    define SYS_sigpending __NR_sigpending
#    define SYS_sigprocmask __NR_sigprocmask
#    define SYS_sigreturn __NR_sigreturn
#    define SYS_sigsuspend __NR_sigsuspend
#    define SYS_socketcall __NR_socketcall
#    define SYS_ssetmask __NR_ssetmask
#    define SYS_stat64 __NR_stat64
#    define SYS_statfs64 __NR_statfs64
#    define SYS_stime __NR_stime
#    define SYS_stty __NR_stty
#    define SYS_truncate64 __NR_truncate64
#    define SYS_ugetrlimit __NR_ugetrlimit
#    define SYS_ulimit __NR_ulimit
#    define SYS_umount __NR_umount
#    define SYS_vm86 __NR_vm86
#    define SYS_vm86old __NR_vm86old
#    define SYS_waitpid __NR_waitpid
#endif

#endif /* _SYSCALL_NUMS_H_ */
