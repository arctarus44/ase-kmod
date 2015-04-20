# ase-kmod
Création d'un module pour le noyau linux. Celui ci devra surveiller le temps d'exécution d'une tâche sur les différents cœur d'un processeur.

Fonctionnalités :
=================
- A l'installation du module, le fichier /proc/ase-cmd et le répertoire /procase/ sont créés
- Ces fichiers sont supprimés à la désinstallation du module.

- Lors de l'écriture dans le fichier ase-cmd :
	- On vérifie que le texte écrit est un nombre
	- On vérifie que ce nombre représente un pid actif
	- On créé un fichier dans le répertoire /proc/ase ayant comme nom le pid en question

- Lors de la lecture d'un des fichiers du répertoire :
	- On récupère la structure correspondante
	- On affiche les temps d'exécutions de ce processus  


Notes :
=======
Les structures sont stockées sous forme d'une liste chainée (fichier pid-list.[h,c]).
