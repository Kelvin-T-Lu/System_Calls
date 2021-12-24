/*
 * getppid.c
 *
 */

#include <types.h>
#include <curthread.h>
#include <thread.h>

int sys_getppid(){

   if(curthread) {
      int ppid = curthread->ppid;
      return ppid;
   }
   return -1;
}
