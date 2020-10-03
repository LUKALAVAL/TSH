#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "types/posix_header.h"
#include "list_file.h"



int ls(const char * path_tar, const char * path_loc)
{
	/*
	- !! REMARQUE IMPORTANTE !! on considère que les path fournis existent
	- ls(...) : renvoie 0 si tout s'est bien passé et -1 sinon
	- path_tar : chemin du tar (exemple "chemin/.../test.tar")
	- path_loc : chemin de la localisation de l'utilisateur à partir du tar avec un '/' à la fin (exemple "dos1/dos2/dos3/")
	*/

	struct posix_header p;
	int n, i, j;
	char name[100];
	char buf[BLOCK_SIZE];
	char message[500] = ""; //surement à améliorer

	int fd = open(path_tar, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	while((n = read(fd, buf, BLOCK_SIZE)) > 0)
	{
		if(*(buf) == '\0')
		{
			/* fin du tar */
			close(fd);
			if (strlen(message) != 0)
			{
				strcat(message, "\n");
				/* écrit le message, un seul appel de write */
				n = write(STDOUT_FILENO, message, 500);
				if (n < 0)
				{
					perror("write");
					return -1;
				}
			}
			return 0;
		}

		/* récupère le nom complet du fichier */
		for (i = 0; i < sizeof(p.name); i++)
		{
			p.name[i] = buf[i];
		}
		/* récupère la taille du fichier */
		for (i = 0; i < sizeof(p.size); i++)
		{
			p.size[i] = buf[124+i];
		}

		/* vérifie que le fichier est à afficher et ajoute son nom exact au message si il l'est */
		if((strncmp(path_loc, p.name, (strlen(path_loc)-1)) == 0) || (strcmp(path_loc,"") == 0))
		{
			/* exclu le nom de l'emplacement (dossier) dans lequel on est cf localisation */
			if(strcmp(path_loc,p.name) != 0)
			{
				for(i = 0,j = strlen(path_loc); j < sizeof(p.name)-strlen(path_loc) ; i++, j++)
				{
					name[i] = p.name[j];
				}
				/* name = nom_exact ou nom_exact/ ou encore nom_exact/... */

				int size_name = strlen(name);
				strtok(name, "/");
				/* name = nom_exact ou nom_exact/ */
				/* ce if permet d'exclure les name = nom_exact/... */
				if(size_name <=  strlen(name) + 1)
				{
					/* ajoute le nom exact du fichier+"\t" au message à renvoyer */
					strcat(strcat(message, name), "\t");
				}
			}
		}

		/* passe à l'en-tête suivant */
		for (i = 0; i < ((atoi(p.size) + BLOCK_SIZE -1)/BLOCK_SIZE); i++)
		{
			/* saute les contenus des fichiers */
			n = read(fd, buf, BLOCK_SIZE);
			if (n < 0)
			{
				perror("read 2");
				return -1;
			}
		}
		/* prêt à lire l'en-tête suivant */
	}
	perror("read 1");
	return -1;

}
