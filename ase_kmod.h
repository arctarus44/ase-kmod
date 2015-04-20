#ifndef H_ASE_KMOD
#define H_ASE_KMOD

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define MOD_NAME "ASE_KMOD :"
#define MOD_BUF_LEN 64

#define PROC_DIR "ase"
#define PROC_ENTRY "ase_cmd"
#define MAX_PID_HANDLE 1024


#define LOG_INIT " Initialisation... Done!\n"
#define LOG_CLEAN " Cleanup module... Done!\n"
#define LOG_INIT_TRACK " Going into the init_track_pid function.\n"
#define LOG_ADD_PID " Adding a new PID into the module.\n"

#define ERR_INIT_OVERFLOW " Overflow during conversion.\n"
#define ERR_INIT_NOT_INT " Error during string to int conversion.\n"
#define ERR_INIT_NOT_PID " The value does not match any PID.\n"

#define PID_FILE_HEADER "**** PID %d stats ****\n"



#endif
