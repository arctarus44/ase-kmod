#include "ase_kmod.h"

MODULE_AUTHOR("Jean-Serge Monbailly - Arthur Dewarumez. Les RoxXoR !");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module which monitors the execution time of a task on the different cores of a processor.");

static struct proc_dir_entry *proc_folder;

static int ase_cmd_show(struct seq_file *m, void *v){
	seq_printf(m, "lolilol\n");
	return 0;
}

static const struct file_operations ase_fops = {
	.owner = THIS_MODULE,
	.open = ase_cmd_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.write = NULL,
};

static int ase_cmd_open(struct inode *inode, struct file *file){
	return single_open(file, ase_cmd_show, NULL);
}


int ase_kmod_init(void){
	proc_folder = proc_mkdir(PROC_DIR, NULL);
	printk(KERN_EMERG MOD_NAME LOG_INIT);
	proc_create(PROC_ENTRY, 0, NULL, &ase_fops);
	return 0;
}

void ase_kmod_cleanup(void){
	remove_proc_entry(PROC_DIR, NULL);
	remove_proc_entry(PROC_ENTRY, NULL);
	printk(KERN_EMERG MOD_NAME LOG_CLEAN);
}

module_init(ase_kmod_init);
module_exit(ase_kmod_cleanup);
