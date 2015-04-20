#ifndef H_PID_LIST
#define H_PID_LIST

#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/sched.h>

/*
 * Ce fichier permet de manipuler facilement la liste de pid
 * à stocker.
 */

void add_pid(struct pid *pid_s);

struct ase_pid *get_struct(int pid);

long get_stime(int pid);

long get_utime(int pid);

#endif
