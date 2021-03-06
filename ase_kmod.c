#include "ase_kmod.h"
#include "pid_list.h"

// KERN_DEBUG ne sert à rien, la machine n'est pas configurée pour l'afficher
#define debug(message)				\
	printk(KERN_EMERG MOD_NAME message)		\

MODULE_AUTHOR("Jean-Serge Monbailly - Arthur Dewarumez. Les RoxXoR !");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module which monitors the execution time of a task on the different cores of a processor.");

/**************/
/* Prototypes */
/**************/
static int ase_pid_open(struct inode *inode, struct file *file);
static int ase_pid_show(struct seq_file *m, void *v);
static int init_track_pid(struct file *file, const char *data, size_t size, loff_t *offset);

/**********************/
/* Variables globales */
/**********************/
int current_pid; /* Utiliser pour l'ajout de pid dans la liste */
static struct proc_dir_entry *proc_folder;

/*
 * Structure définisant les actions sur le fichier PROC_ENTRY du répertoire.
 * Les fichiers du répertoire ase doivent seulement être lu.
 */
static const struct file_operations ase_pid = {
	.owner = THIS_MODULE,
	.open = ase_pid_open,
	.read = seq_read,
};

/*
 * Structure définisant les actions sur le fichier ase_cmd.
 * Ce fichier doit seulement être écrit..
 */
static const struct file_operations ase_cmd = {
	.owner = THIS_MODULE,
	.write = init_track_pid,
};


/*************/
/* Fonctions */
/*************/

/*
 * Fonction déterminant l'affichage d'un fichier du répertoire ase/
 */
static int ase_pid_show(struct seq_file *m, void *v)
{
	int stime, utime;
	printk(KERN_ERR MOD_NAME "************* CRT_PID %d\n", current_pid);
	stime = get_stime(current_pid);
	utime = get_utime(current_pid);

#ifdef DEBUG
	debug(" entering ase_pid_show\n");
#endif

	/* Le seq_printf ne doit pas servir étant donné que le fichier proc n'existe pas vraiment */
	seq_printf(m, PID_FILE_HEADER, current_pid);
	seq_printf(m, "temps en espace utilisateur : %d\n", utime);
	seq_printf(m, "temps en espace noyau : %d\n", stime);
	seq_printf(m, "temps total d'éxécution : %d\n", utime + stime);
	return 0;
}

/*
 * Fonction associé à l'affichage d'un fichier du répertoire /proc/ase/
 * Doit afficher des informations sur le processus qu'il représente.
 */
static int ase_pid_open(struct inode *inode, struct file *file){
	long tmp;
	/* On récupère le pid du fichier sur lequel on travaille */
	switch(kstrtol(file->f_path.dentry->d_iname, 10, &tmp)){
	case -ERANGE:
	printk(KERN_ERR MOD_NAME ERR_INIT_OVERFLOW);
	return -ERANGE;
	case -EINVAL:
	printk(KERN_ERR MOD_NAME ERR_INIT_NOT_INT);
	return -EINVAL;
	}

	current_pid = (int) tmp;
	return single_open(file, ase_pid_show, NULL);
}


/*
 * Vérifie que le pid passé est bien un nombre et recherche un processus correspondant.
 * Si trouvé, créé un fichier du nom de ce PID dans le répertoire /proc/ase/
 * après avoir vérifié si celui-ci n'existe pas déjà.
 */
static void add_pid_action(const char *pid_str){
	long pid;
	struct pid *pid_struct;

#ifdef DEBUG
	debug(" Entering the add_pid_action function.\n");
#endif

	/* On vérifie que la valeur écrite est bien un nombre */
	switch(kstrtol(pid_str, 10, &pid)){
	case -ERANGE:
	printk(KERN_ERR MOD_NAME ERR_INIT_OVERFLOW);
	return ;
	case -EINVAL:
	printk(KERN_ERR MOD_NAME ERR_INIT_NOT_INT);
	return ;
	}

	/* On cherche un processus correspondant à ce PID */
	if((pid_struct = find_get_pid(pid)) != NULL){
#ifdef DEBUG
	debug(LOG_ADD_PID);
#endif
	// On ajoute ce PID à la liste
	add_pid(pid_struct);
	proc_create(pid_str, 0644, proc_folder, &ase_pid);
	}
	else{
	printk(KERN_ERR MOD_NAME ERR_INIT_NOT_PID);
	}
}

/*
 * Cette fonction est appelée lors d'une écriture dans le fichier ase_cmd.
 * Cette fonction récupère le texte écrit et appelle la fonction add_pid_action
 * pour la traiter
 */
static int init_track_pid(struct file *file, const char __user *buff, size_t size, loff_t *data){
	char *tmp = kmalloc(sizeof(char) * size, GFP_KERNEL);
	int i;

#ifdef DEBUG
	debug(LOG_INIT_TRACK);
#endif

	if(size > (MOD_BUF_LEN - 1)){
	printk(KERN_ERR MOD_NAME " Message trop grand.\n");
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
#ifdef DEBUG
	debug(LOG_INIT);
#endif

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

#ifdef DEBUG
	debug(LOG_CLEAN);
#endif
}

module_init(ase_kmod_init);
module_exit(ase_kmod_cleanup);
