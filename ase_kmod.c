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

/*
 * Structure définisant les actions sur le fichier PROC_ENTRY du répertoire.
 * Le fichiers ase_cmd doit seulement être écrit.
 */
static const struct file_operations ase_pid = {
    .owner = THIS_MODULE,
    .open = ase_cmd_open,
    .read = seq_read,
};

/*
 * Structure définisant les actions sur les fichiers PID  du répertoire
 * PROC_DIR. Ces fichiers ne doivent être que lu.
 * TODO : modifier le champ read avec la bonne fonction.
 */
static const struct file_operations ase_cmd = {
    .owner = THIS_MODULE,
    .write = init_track_pid,
};

static struct proc_dir_entry *proc_folder;
static int pid_count = 0;
static struct pid *pid_array[MAX_PID_HANDLE];


/*************/
/* Fonctions */
/*************/

/*
 * A quoi ça sert?
 */
static int ase_cmd_show(struct seq_file *m, void *v){
    seq_printf(m, "lolilol\n");
    return 0;
}

/*
 * Fonction associé à l'affichage d'un fichier du répertoire /proc/ase/
 * Doit afficher des informations sur le processus qu'il représente.
 */
static int ase_cmd_open(struct inode *inode, struct file *file){
    /* Pourquoi ? */
    return single_open(file, ase_cmd_show, NULL);
}


/*
 * Vérifie que le pid passé est bien un nombre et recherche un processus correspondant.
 * Si trouvé, créé un fichier du nom de ce PID dans le répertoire /proc/ase/
 * après avoir vérifié si celui-ci n'existe pas déjà.
 */
static void add_pid_action(const char *pid_str){
    long pid;
    int i;
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

	/* On vérifie que le fichier n'existe pas avant de le créer. */
	for(i = 0 ; i < pid_count ; i++)
	    {
		/* Warning si il existe. */
		if((long int)(pid_array[i]->numbers[0].nr) == pid)
		    {
			printk(KERN_EMERG MOD_NAME " Proc file already exists.\n");
			return;
		    }
	    }
	proc_create(pid_str, 0644, proc_folder, &ase_pid);

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

/*
 * Initialisation du module, créé les fichiers/répertoires nécessaires
 */
int ase_kmod_init(void){
    printk(KERN_EMERG MOD_NAME LOG_INIT);

    proc_folder = proc_mkdir(PROC_DIR, NULL);
    proc_create(PROC_ENTRY, 0644, NULL, &ase_cmd);

    return 0;
}

/*
 * Retrait du module, suppression des fichiers correspondants.
 * Le répertoire est supprimé 'proprement'
 * i.e : en cascade
 */
void ase_kmod_cleanup(void){
    /* Supprime les fichiers en cascade */
    remove_proc_subtree(PROC_DIR, NULL);
    remove_proc_entry(PROC_ENTRY, NULL);

    printk(KERN_EMERG MOD_NAME LOG_CLEAN);
}

module_init(ase_kmod_init);
module_exit(ase_kmod_cleanup);
