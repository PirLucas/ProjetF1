#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "structs.h"
#include "conv.h"
#include "fct_tour.h"
#include "tri.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>


void affichage(struct voiture * v,int nbr,struct classement * clas){
	
	//difference

	printf("voiture n° |	 S1	|     S2     |     S3     |    tour    |\n");
	printf("___________|____________|____________|____________|____________|\n");
	for (int i = 0; i < nbr; i++){
		printf("%-11i|%-12.3f|%-12.3f|%-12.3f|%-12s|\n", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3, minSec(v[i].dern_tour));
	}
	printf("___________|____________|____________|____________|____________|\n");
	printf("\n");
	
	//si on doit voir les chiffres se mettre a jour, il faut pas que les voitures tournent trop vite?
	for (int i = 0; i < nbr; i++){
		printf("meilleur tour n°%i: %s (+%.3f)\n", v[i].id, minSec(v[i].m_tour), diff(v[i], v[i-1]));
	}	
	printf("\n");
	
	
	printf("meilleur S1: n°%i avec %.3f\n",clas->S1_id, clas->best_S1);
	printf("meilleur S2: n°%i avec %.3f\n",clas->S2_id, clas->best_S2);
	printf("meilleur S3: n°%i avec %.3f\n",clas->S3_id, clas->best_S3);
}


int main(){
	struct classement * clas;
	//int liste[] = {44,63,1,11,55,16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	int liste[]= {44,63,1,11,55};
	int nbr = sizeof(liste)/sizeof(int);//pour obtenir la taille d'une liste, on divise sa taille par la taille du type d'element qu'elle contiens
	struct voiture * shCoureurs;
	
	
	
	srandom(time(NULL));//assure que le random change avec l'heure systeme
	int duree = 100;
	int pid;
	
	int shmId = shmget(57, nbr * sizeof(struct voiture), IPC_CREAT | 0666); // le | est un "ou". donc soit on crée, soit si il existe, on ouvre en 666
	int shmIdClas = shmget(60, sizeof(struct classement), IPC_CREAT | 0666);
	if (shmId == -1) {
		perror("Shared memory");
		return (-1);
	}
	if (shmIdClas == -1) {
		perror("Shared memory");
		return (-1);
	}
	
	shCoureurs = shmat(shmId, NULL,0);
	clas = shmat(shmIdClas,0,0);
	if (shCoureurs == (void *) -1) {
		perror("Shared memory attach");
		return -2;
	}
	if (clas == (void *) -1) {
		perror("Shared memory attach");
		return -2;
	}
	clas->best_S1 = 3600;
	clas->best_S2 = 3600;
	clas->best_S3 = 3600;
	for (int i = 0; i < nbr; i++){
		shCoureurs[i].tps_S1 = 3600;//initie les temps au maximum possible
		shCoureurs[i].tps_S2 = 3600;
		shCoureurs[i].tps_S3 = 3600;
		shCoureurs[i].id = liste[i];
		shCoureurs[i].tour_fait = 0;
		shCoureurs[i].total=0;
		shCoureurs[i].m_tour = 3600;
		shCoureurs[i].position=i+1;
	};
	
	for (int i = 0; i < nbr; i++){
		//tourne(&coureurs[i], duree); //peut-etre pas nécessaire
		//tour(&coureurs[i]);
		//tour(&coureurs[i]);
		//tour(&coureurs[i]);
		if ((pid = fork()) == -1){
		perror("fork");
		exit(-1);
		}
		else if(pid == 0){
			if (i == nbr-1){
				clas->start = 1;
			}
			//printf("%i",i);
			while (clas->start != 1){
			}
			//printf("%i",i);
			tour(&shCoureurs[i], clas);
			tour(&shCoureurs[i], clas);
			tour(&shCoureurs[i], clas);
			exit(-1);
		}
		else{
			if ((i == nbr-1) && (clas->start==1)){
				usleep(100000);
				printf("\n");
				affichage(tri(shCoureurs,sizeof(struct voiture), nbr), nbr, clas);
				printf("\n");
			}
		}
	};
	

	shmdt(shCoureurs);
}
