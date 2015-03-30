#ifndef H_ASE_KMOD
#define H_ASE_KMOD

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>

#define MOD_NAME "ASE_KMOD :"

#define PROC_DIR "ase"
#define PROC_ENTRY "ase_cmd"


#define LOG_INIT "Initialisation... Done!\n"
#define LOG_CLEAN "Cleanup module... Done!\n"

#endif
