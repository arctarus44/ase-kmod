#include <stdlib.h>

#include "pid_list.h"

struct ase_pid
{
    int pid;
    struct pid *pid_s;
    struct ase_pid *next;
};

struct ase_pid *head = NULL;

void add_pid(struct pid *pid_s)
{
    struct ase_pid *tmp = head;
    struct ase_pid *new;
    new->pid_s = pid_s;
    new->next = NULL;
    new->pid = pid_s->numbers[0].nr;

    if(head == NULL)
	{
	    head = new;
	    return;
	}

    
    while(tmp.next != NULL)
	{
	    tmp = tmp->next;
	}

}
