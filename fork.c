/*
 * fork.c
 *
 */
#include <types.h>
#include <machine/trapframe.h>
#include <curthread.h>
#include <kern/errno.h>
#include <thread.h>
#include <addrspace.h>
#include <array.h>
#include <lib.h>
#include <machine/spl.h>


pid_t sys_fork(struct trapframe *parent_tf, int *ret){
    /*
    Check for errors.
    Create a pid when creating a new process. Add it to your process table (thread_fork)
    Copy the parent’s address space
    Call thread_fork(), the child will start executing ‘md_forkentry’
         Pass in parent_tf and the child copied addrspace.
     */

    int spl = splhigh();

    /* Checking if too many processes exists. */
    int i, bool = 0;
    for (i = 0; i<TABLESIZE; i++){
        if(!process_table[i]) {
            bool = 1;
            break;
        }
    }
    if(bool == 0){ /* Error checking */ return EAGAIN;  }

    struct thread *child_thread = kmalloc(sizeof (struct thread));
    struct trapframe *child_tf = kmalloc(sizeof(struct trapframe));

    /* Failed malloc. */
    if(child_tf==NULL){
        return -1;
    }
    /* Copy the parent trap frame into the child. */
    //memcpy(child_tf, parent_tf, sizeof(struct trapframe));

    /* Copying the parent's address space */
    struct addrspace *child_addr;
    if (as_copy(curthread -> t_vmspace, &child_addr)){
        return -1; /* Error copying the virtual space. */
    }

     /* Error from thread_fork
      */

    void **args = kmalloc(sizeof(void *) *2 );
    if(args==NULL){ // Args did not get intialized.
        return -1;
    }

    args[0] = parent_tf;
    args[1] = child_addr; //curthread -> t_vmspace;

    // kprintf("Parent tf: %p Currentthread: %p ", &(parent_tf-> tf_vaddr), curthread-> t_vmspace );
    if(thread_fork("child", (void *) args , 0 , (void *) md_forkentry, &child_thread)){
        return -1; /* Fork error */
    }

//    kprintf("Post MD\n");
    *ret = child_thread -> pid;

    splx(spl);
    return 0;
}
