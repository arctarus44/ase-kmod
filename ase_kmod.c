#include "ase_kmod.h"

MODULE_AUTHOR("Jean-Serge Monbailly - Arthur Dewarumez. Les RoxXoR !");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module which monitors the execution time of a task on the different cores of a processor.");

/**************/
/* Prototypes */
/**************/

static int ase_cmd_open(struct inode *inode, struct file *file);
static int ase_cmd_show(struct seq_file *m, void *v);
static int init_track_pid(struct file *file, const char *data, size_t size, loff_t *offset);


/**********************/
/* Variables globales */
/**********************/

static const struct file_operations ase_fops = {
	.owner = THIS_MODULE,
	.open = ase_cmd_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.write = init_track_pid,
};

static struct proc_dir_entry *proc_folder;
static int pid_count = 0;
static struct pid *pid_array[MAX_PID_HANDLE];


/*************/
/* Fonctions */
/*************/

static int ase_cmd_show(struct seq_file *m, void *v){
	seq_printf(m, "lolilol\n");
	return 0;
}

static int ase_cmd_open(struct inode *inode, struct file *file){
	return single_open(file, ase_cmd_show, NULL);
}

static void add_pid_action(const char *pid_str){
	long pid;
	struct pid *pid_struct;

	printk(KERN_EMERG MOD_NAME " Entering the add_pid_action function.\n");
	switch(kstrtol(pid_str, 10, &pid)){
	case -ERANGE:
		printk(KERN_EMERG MOD_NAME ERR_INIT_OVERFLOW);
		return ;
	case -EINVAL:
		printk(KERN_EMERG MOD_NAME ERR_INIT_NOT_INT);
		return ;
	}
	if((pid_struct = find_get_pid(pid)) != NULL){
		printk(KERN_EMERG MOD_NAME LOG_ADD_PID);
		proc_create(pid_str, 0644, proc_folder, &ase_fops);
		pid_array[pid_count] = pid_struct;
		pid_count++;
	}
	else{
		printk(KERN_EMERG MOD_NAME ERR_INIT_NOT_PID);
	}
}

static int init_track_pid(struct file *file, const char __user *buff, size_t size, loff_t *data){
	char *tmp = kmalloc(sizeof(char) * size, GFP_KERNEL);
	int i;
	printk(KERN_EMERG MOD_NAME LOG_INIT_TRACK);
	if (size > (MOD_BUF_LEN - 1)) {
		printk(KERN_EMERG MOD_NAME " Message trop grand.\n");
		return -EINVAL;
	}
	for(i = 0; i < size-1; i++){
		tmp[i] = buff[i];
	}
	tmp[i] = '\0';

	add_pid_action(tmp);

	return size;
}

int ase_kmod_init(void){
	proc_folder = proc_mkdir(PROC_DIR, NULL);
	printk(KERN_EMERG MOD_NAME LOG_INIT);
	proc_create(PROC_ENTRY, 0644, NULL, &ase_fops);
	return 0;
}

void ase_kmod_cleanup(void){
	remove_proc_entry(PROC_DIR, NULL);
	remove_proc_entry(PROC_ENTRY, NULL);
	printk(KERN_EMERG MOD_NAME LOG_CLEAN);
}

module_init(ase_kmod_init);
module_exit(ase_kmod_cleanup);
