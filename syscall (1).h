#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);

int sys_getpid();

int sys_getppid();

pid_t sys_fork(struct trapframe *parent_tf, int *ret);

int sys_exit(int code);

int sys_waitpid(pid_t pid, userptr_t status, int options, int *ret);

#endif /* _SYSCALL_H_ */
