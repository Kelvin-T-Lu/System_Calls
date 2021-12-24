/*
 * getpid.c
 *
 */

#include <curthread.h>
#include <types.h>
#include <thread.h>

int sys_getpid(){
    if(curthread) {
        int pid = curthread -> pid;
        return pid;
    }
    return -1;
//    return -1;
//    return -1; //curthread->pid;
  //return 1;
}
