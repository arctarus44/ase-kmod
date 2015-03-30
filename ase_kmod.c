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



static int ase_cmd_show(struct seq_file *m, void *v){
	seq_printf(m, "lolilol\n");
	return 0;
}

static int ase_cmd_open(struct inode *inode, struct file *file){
	return single_open(file, ase_cmd_show, NULL);
}

static int init_track_pid(struct file *file, const char *data, size_t size, loff_t *offset){
	printk(KERN_EMERG MOD_NAME " passage dans la fonction write.\n");
	printk(KERN_EMERG MOD_NAME " data : %s\n", data);
	printk(KERN_EMERG MOD_NAME " size : %d\n", size);
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
