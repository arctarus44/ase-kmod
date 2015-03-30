#include "ase_kmod.h"

MODULE_AUTHOR("Jean-Serge Monbailly - Arthur Dewarumez. Les RoxXoR !");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module which monitors the execution time of a task on the different cores of a processor.");

static struct proc_dir_entry *proc_folder;
int my_write_function(struct file *file, const char *data, size_t size, loff_t *offset){
  printk(KERN_EMERG MOD_NAME " passage dans la fonction write.\n");    
  printk(KERN_EMERG MOD_NAME " data : %s\n", data);    
  printk(KERN_EMERG MOD_NAME " size : %d\n", size);    
  return size;
}

static ssize_t my_read_function(struct file *file, char *buf, size_t count, loff_t *ppos)
{
  printk(KERN_EMERG MOD_NAME " passage dans la fonction read.\n");    
  printk(KERN_EMERG MOD_NAME " donées lues : %s\n", buf);    
  return count;
}

static int my_open_function(struct inode *inode, struct file *file)
{
  printk(KERN_EMERG MOD_NAME " passage dans la fonction open.\n");    

    return 0;
}

static const struct file_operations ase_fops = {
	.owner = THIS_MODULE,
	.open = my_open_function,
	.read = my_read_function,
	/* .llseek = seq_lseek, */
	/* .release = single_release, */
	.write = my_write_function,
};


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
