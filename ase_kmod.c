#include "ase_kmod.h"

MODULE_AUTHOR("Jean-Serge Monbailly - Arthur Dewarumez. Les RoxXoR !");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module which monitors the execution time of a task on the different cores of a processor.");

static struct proc_dir_entry *proc_folder;
int test(struct file *file, const char *data, size_t size, loff_t *offset){
  printk(KERN_EMERG MOD_NAME "Passage dans la fonction de write.\n");    

  return size;
}

static const struct file_operations ase_fops = {
	.owner = THIS_MODULE,
	/* .open = NULL, */
	/* .read = seq_read, */
	/* .llseek = seq_lseek, */
	/* .release = single_release, */
	.write = test,
};


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
