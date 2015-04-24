#include "pid_list.h"

/* Définition de la structure */
struct ase_pid
{
	int pid;
	struct pid *pid_s;
	struct task_struct * task;
	struct ase_pid * next;
};

/* Début de la liste */
struct ase_pid *head = NULL;

/*
 * Créé une nouvelle structure ase_pid
 * à partir de la structure pid passée en paramètre
 */
static struct ase_pid * create_struct(struct pid * pid_s)
{
	struct ase_pid * new;

	new = (struct ase_pid *) kmalloc(sizeof(struct ase_pid), GFP_KERNEL);
	new->pid_s = pid_s;
	new->task = pid_task(pid_s, PIDTYPE_PID);
	new->pid = pid_s->numbers[0].nr;
	new->next = NULL;

	return new;
}

/*
 * Ajoute un nouvel élément à la liste.
 * On suppose que le pid n'est pas déjà dans la liste.
 */
void add_pid(struct pid * pid_s)
{
	struct ase_pid * add;
	struct ase_pid * tmp;

	add = create_struct(pid_s);

	if(head == NULL)
		{
			printk(KERN_EMERG "LIST " "Ajout en tête\n");
			head = add;
			return;
		}

	tmp = head;
	while(tmp->next != NULL)
		{
			printk(KERN_EMERG "LIST " "Pas la fin\n");
			tmp = tmp->next;
		}

	tmp->next = add;
}

struct ase_pid * get_struct(int pid)
{
	struct ase_pid * tmp = head;

	while(tmp != NULL)
		{
			if(tmp->pid == pid){
				printk(KERN_EMERG "LIST " "I find it\n");
				return tmp;
			}
			tmp = tmp->next;
			printk(KERN_EMERG "LIST " "I don't find it\n");
		}

	return NULL;
}

/*
 * Retourne le stime correspondant au pid indiqué.
 * On suppose ce pid suivi et présent dans le buffer.
 */
long get_stime(int pid)
{
	struct ase_pid * tmp = get_struct(pid);

	if(tmp == NULL)
		{
			/* Ne devrait pas arriver */
			return -1;
		}

	return tmp->task->stime;
}


/*
 * Retourne le utime correspondant au pid indiqué.
 * On suppose ce pid suivi et présent dans le buffer.
 */
long get_utime(int pid)
{
	struct ase_pid * tmp = get_struct(pid);

	if(tmp == NULL)
		{
			/* Ne devrait pas arriver */
			return -1;
		}

	return tmp->task->utime;
}
