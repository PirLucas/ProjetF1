#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <fcntl.h>
#include "structs.h"
#include "conv.h"
#include "fct_tour.h"
#include "tri.h"
#include "fctRapport.h"

void affichage(struct voiture * v,int nbr,struct classement * clas){
	
	
	
	
	printf("voiture n° |	 S1	|     S2     |     S3     |dernier tour |meilleur tour|   nb   | différence |etat\n");
	printf("___________|____________|____________|____________|_____________|_____________|________|____________|\n");
	
	for (int i = 0; i < nbr; i++){
		char* etat;
		if (v[i].etat == 0){
			etat = "en_course";
		}
		else if (v[i].etat == 1){
			etat = "P";
		}
		else if (v[i].etat == 2){
			etat = "OUT";
		}
		printf("%-11i|%-12.3f|%-12.3f|%-12.3f|%-13s|%-13s|%-8i|+%-11.3f|%s", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3, minSec(v[i].dern_tour),minSec(v[i].m_tour),v[i].tour_fait,diff(v[i], v[i-1]), etat);
		printf("\n");
	}
	
	printf("___________|____________|____________|____________|_____________|_____________|________|____________|\n");
	printf("\n");
	
	printf("meilleur S1: n°%i avec %.3f\n",clas->S1_id, clas->best_S1);
	printf("meilleur S2: n°%i avec %.3f\n",clas->S2_id, clas->best_S2);
	printf("meilleur S3: n°%i avec %.3f\n",clas->S3_id, clas->best_S3);
	printf("meilleur tour: n°%i avec %s\n",clas->total_id, minSec(clas->best_total));
}




int main(int argc, char* argv[]){
	
	int liste[] = {44,63,1,11,55,16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	int nbr = sizeof(liste)/sizeof(int);//pour obtenir la taille d'une liste, on divise sa taille par la taille du type d'element qu'elle contiens
	int duree = 4;
	int pid;
	
	//création de la zone de memoire partagée pour les coureurs
	struct voiture * shCoureurs;
	int shmId = shmget(57, nbr * sizeof(struct voiture), IPC_CREAT | 0666); // le | est un "ou". donc soit on crée, soit si il existe, on ouvre en 666
	if (shmId == -1) {
		perror("Shared memory");
		return (-1);
	}
	shCoureurs = shmat(shmId, NULL,0);
	if (shCoureurs == (void *) -1) {
		perror("Shared memory attach");
		return -2;
	}
	
	//création de la zone de memoire partagée pour le classement
	struct classement * clas;
	int shmIdClas = shmget(60, sizeof(struct classement), IPC_CREAT | 0666);
	if (shmIdClas == -1) {
		perror("Shared memory");
		return (-1);
	}
	clas = shmat(shmIdClas,NULL,0);
	if (clas == (void *) -1) {
		perror("Shared memory attach");
		return -2;
	}
	
	
	srandom(time(NULL));//assure que le random change avec l'heure systeme
	
	
	if (argc == 1){
		printf("Entrez un argument");
		exit(-1);
	}
	
	if ((strcmp(argv[1], "normal") == 0)){
		//lance une course normale
		//initialisation du classement
		clas->best_S1 = 3600;
		clas->best_S2 = 3600;
		clas->best_S3 = 3600;
		clas->best_total = 3600;
		clas->start = 0;
		clas->time_start = 0;
		clas->current_time = 0;
		
		//initialisation des coureurs
		for (int i = 0; i < nbr; i++){
			shCoureurs[i].tps_S1 = 3600;//initie les temps au maximum possible
			shCoureurs[i].tps_S2 = 3600;
			shCoureurs[i].tps_S3 = 3600;
			shCoureurs[i].id = liste[i];
			shCoureurs[i].tour_fait = 0;
			shCoureurs[i].total=0;
			shCoureurs[i].m_tour = 3600;
			shCoureurs[i].position=i+1;
			shCoureurs[i].etat = 0;
		};
		
		struct voiture copy[nbr];
		memcpy(copy, shCoureurs, sizeof(struct voiture)*nbr);
		
		
		
		for (int i = 0; i < nbr; i++){
			if ((pid = fork()) == -1){
				perror("fork");
				exit(-1);
			}
			else if(pid == 0){
				shCoureurs = shmat(shmId, NULL,0);
				if (shCoureurs == (void *) -1) {
					perror("Shared memory attach");
					return -2;
				}
				clas = shmat(shmIdClas,NULL,0);
				if (clas == (void *) -1) {
					perror("Shared memory attach");
					return -2;
				}
				if (i == nbr-1){
					clas->start = 1;
					clas->time_start = time(NULL);
					clas->current_time = 0;
				}
				while (clas->start != 1){}
				//assure que la course commence qu'une fois que la derniere voiture est créée
				tourne(&shCoureurs[i], duree, clas);
				//tour(&shCoureurs[i], clas);
				//tour(&shCoureurs[i], clas);
				//tour(&shCoureurs[i], clas);
				shmdt(shCoureurs);
				shmdt(clas);
				exit(-1);
			}
			else{
				if (i == nbr-1){
					while (clas->start != 1){} //a quelle fréquence?
					//sleep(duree+2);
					printf("\n");
					while(clas->current_time<=duree){
						//if(clas->current_time % 2 == 0){
							sleep(1);
							system("clear");  
							affichage(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
							//printf("###%i/%i###", shCoureurs[0].id, copy[0].id);
							
						//}
					}
						
					printf("\n");
				}
			}
		};
		
		affichageTxt(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
	}
	else if ((strcmp(argv[1], "fichier") == 0)){
		//lance une course avec le fichier
		nbr = creeVoitures(shCoureurs, clas, "rapport.txt");
		struct voiture copy[nbr];
		memcpy(copy, shCoureurs, sizeof(struct voiture)*nbr);
		for (int i = 0; i < nbr; i++){
			if ((pid = fork()) == -1){
				perror("fork");
				exit(-1);
			}
			else if(pid == 0){
				shCoureurs = shmat(shmId, NULL,0);
				if (shCoureurs == (void *) -1) {
					perror("Shared memory attach");
					return -2;
				}
				clas = shmat(shmIdClas,NULL,0);
				if (clas == (void *) -1) {
					perror("Shared memory attach");
					return -2;
				}
				if (i == nbr-1){
					clas->start = 1;
					clas->time_start = time(NULL);
					clas->current_time = 0;
				}
				while (clas->start != 1){}
				//assure que la course commence qu'une fois que la derniere voiture est créée
				tourne(&shCoureurs[i], duree, clas);
				//tour(&shCoureurs[i], clas);
				//tour(&shCoureurs[i], clas);
				//tour(&shCoureurs[i], clas);
				shmdt(shCoureurs);
				shmdt(clas);
				exit(-1);
			}
			else{
				if (i == nbr-1){
					while (clas->start != 1){} //a quelle fréquence?
					//sleep(duree+2);
					printf("\n");
					while(clas->current_time<=duree){
						//if(clas->current_time % 2 == 0){
							sleep(1);
							system("clear");  
							affichage(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
							//printf("###%i/%i###", shCoureurs[0].id, copy[0].id);
							
						//}
					}
						
					printf("\n");
				}
			}
		};
		
		affichageTxt(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
	}
	
	
	
	
	
	//debutcourse(shCoureurs, clas, nbr)
	
	//printf("shc%s\n", shCoureurs[2].etat);
	
	//printf("cpy%s\n", copy[2].etat);
	//affichage(shCoureurs, nbr, clas);
	//affichage(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
	
	
	
	
	
	
	
	
	
	
	
	

	//détache les zones de MP
	shmdt(shCoureurs);
	shmdt(clas);
	
	//supprime les zones de MP
	shmctl(shmId, IPC_RMID, NULL);
	shmctl(shmIdClas, IPC_RMID, NULL);
}
