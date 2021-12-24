/* 
 * exit.c
 *
 */

#include <types.h>
#include <curthread.h>
#include <thread.h>
#include <lib.h>

int sys_exit(int code){
    // Mark the Thread_Supp's has_exit.
    //    Mark all children's ppid to -1.
    // Check if the curthread has a parent.
    //     If has a parent, decrement parent's children and remove current thread from parent.
    //         Remove parent from process table, if curr_thread is the last child and parent has exited.
    // Remove curthread from process table, if curthread doesn't have any children.

    // Thread data struct representation in process table.
    struct thread_supp *temp = table_findProcess(process_table, curthread->pid);

    // Assign the exit code inside the process id.
    temp -> exit_code = code;

    // Marks the process as exit in the process table.
    // Also marks all children ppid to -1.
    table_exit(process_table, curthread->pid);

    // If thread has a parent, remove child from
    // the parent's child list.
    int child_ppid = curthread -> ppid;
    if(child_ppid != -1){
        //*(process_table[cur_thread -> ppid]).children[cur_thread->pid] = 0;
        //struct thread *parentProcess = table_findProcess(process_table, t_ppid);
        //(*parentProcess).children[curthread->pid] = 0;
        //parentProcess -> num_children --;

        struct thread_supp *parent_supp= table_findProcess(process_table, child_ppid);
        (*parent_supp).children[temp->pid] = 0;
        parent_supp -> num_children --;

        // If the parent supp doesn't have a child, remove it from process table.
        if(parent_supp -> num_children == 0 && parent_supp -> has_exit == 1){
            table_index_remove(process_table, parent_supp -> pid);
        }
    }

    // Remove the process from thread, if it doesn't have children.
    // Assumption: If thread has children, it will be removed through its child threads.
    if(temp -> num_children == 0){
        table_index_remove(process_table, curthread->pid);
    }

    thread_exit();

    return 0;
}
