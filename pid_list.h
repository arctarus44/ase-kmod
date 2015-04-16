#ifndef H_PID_LIST
#define H_PID_LIST

/* #include <linux/pid.h> */
struct pid;

/*
 * Ce fichier permet de manipuler facilement la liste de pid
 * à stocker.
 */

void add_pid(struct pid *pid_s);

#endif
