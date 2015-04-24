# ase-kmod

Arthur Dewarumez
Jean-Serge Monbailly


Création d'un module pour le noyau linux. Celui ci devra surveiller le temps d'exécution d'une tâche sur les différents cœur d'un processeur.

Le temps d'exécution affiché correspond au temps processeur total alloué pour un processus dans l'espace utilisateur (utime) ainsi que dans 
l'espace machine (stime).


Fonctionnalités :
=================
- A l'installation du module, le fichier /proc/ase-cmd et le répertoire /proc/ase/ sont créés
- Ces fichiers sont supprimés à la désinstallation du module.

- Lors de l'écriture dans le fichier ase-cmd :
	- On vérifie que le texte écrit est un nombre
	- On vérifie que ce nombre représente un pid actif
	- On créé un fichier dans le répertoire /proc/ase ayant comme nom le pid en question

- Lors de la lecture d'un des fichiers du répertoire :
	- On récupère la structure correspondante
	- On affiche les temps d'exécutions de ce processus (champs utime et stime de la task_struct)


Notes :
=======

Liste chaînée
-------------
Les structures sont stockées sous forme d'une liste chainée (fichier pid-list.[h,c]).
Cette liste est gérée par les fichiers pid-list qui permettent de la manipuler par la biais 
de simples fonctions.

La structure ase_pid est composée :
- du pid du processus concerné
- la structure pid de ce processus
- la task-struct associée

Ces champs permettent de faciliter l'accès aux champs et aux fonctions qui nous intéressent dans le programme.


Debug
-----
Afin de faciliter le debugging, nous avons ajouter une macro permettant de faciliter l'affichage d'informations
particulière (entrée dans une fonction ou autre) utilisant les printk.

Ces informations ne s'affichent que si la macro DEBUG est définie (voir Makefile).
