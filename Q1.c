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
#include <sys/sem.h>
#include "structs.h"
#include "conv.h"
#include "fct_tour.h"
#include "tri.h"
#include "fctRapport.h"
#include "affichage.h"




int main(int argc, char* argv[]){
	
	int liste[] = {44,63,1,11,55,16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	int nbr = sizeof(liste)/sizeof(int);//pour obtenir la taille d'une liste, on divise sa taille par la taille du type d'element qu'elle contiens
	int duree = 4; //changer pour la durée d'un Q1
	int pid;
	
	//sem_t semVoitures;
	//sem_t semAffiche;
	
	
	//sem_t* listeSem = calloc(2, sizeof(sem_t));
	
	int semId = semget(54, 2, IPC_CREAT | 0666);
	
	struct sembuf semOp[6];
	struct sembuf semVoituresOpen; //semop[0]
	struct sembuf semVoituresWait; //semOp[1]
	struct sembuf semVoituresClose; //semOp[2]
	struct sembuf semAfficheOpen;
	struct sembuf semAfficheWait; //semOp[4]
	struct sembuf semAfficheClose; //semop[5]
	semOp[0] =semVoituresOpen;
	semOp[1] =semVoituresWait;
	semOp[2] =semVoituresClose;
	semOp[3] =semAfficheOpen;
	semOp[4] =semAfficheWait;
	semOp[5] =semAfficheClose;
	
	


   	semOp[1].sem_num = 0;        /* Operate on semaphore 0 */
   	semOp[1].sem_op = 0;         /* Wait for value to equal 0 */
   	semOp[1].sem_flg = 0;

   	semOp[2].sem_num = 0;        /* Operate on semaphore 0 */
   	semOp[2].sem_op = 1;         /* Increment value by one */
   	semOp[2].sem_flg = 0;
   
   	semOp[0].sem_num = 0;        /* Operate on semaphore 0 */
   	semOp[0].sem_op = -1;         /* Decrement value by one */
   	semOp[0].sem_flg = 0;
   	
   	semOp[4].sem_num = 1;        /* Operate on semaphore 0 */
   	semOp[4].sem_op = 0;         /* Wait for value to equal 0 */
   	semOp[4].sem_flg = 0;

   	semOp[5].sem_num = 1;        /* Operate on semaphore 0 */
   	semOp[5].sem_op = 1;         /* Increment value by one */
   	semOp[5].sem_flg = 0;
   
   	semOp[3].sem_num = 1;        /* Operate on semaphore 0 */
   	semOp[3].sem_op = -1;         /* Decrement value by one */
   	semOp[3].sem_flg = 0;
   	
   	

         
	
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
	struct voiture copy[nbr];
	
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
		clas->compteur = 0;
		clas->flag = 0;
		
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

		memcpy(copy, shCoureurs, sizeof(struct voiture)*nbr);
	}
	else if ((strcmp(argv[1], "fichier") == 0)){
		//lance une course avec le fichier
		nbr = creeVoitures(shCoureurs, clas, "rapport.txt");
		memcpy(copy, shCoureurs, sizeof(struct voiture)*nbr);
	}
		
	
	//if Q1 n = 20
	// if Q2 n = 15
	//if Q3 n = 10
	int n = 20;
		
	for (int i = 0; i < n; i++){
	
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
			if (i == n-1){
				clas->start = 1;
				clas->time_start = time(NULL);
				clas->current_time = 0;
			}

			while (clas->start != 1){}
			//assure que la course commence qu'une fois que la derniere voiture est créée
			
			//if P1/P2/P3
			tourne(&shCoureurs[i], duree, clas, semId, semOp);
			
			//if Q1/Q2/Q3
			//pareil que P1, mais on disqualifie les dernieres voitures (on les retire du fichier texte rapport) on modifie l'attribut position chaque Qx a sa propre durée
			
			//if sprint
			//on determine une distance/un nombre de tour, les voitures tournent selon cette valeur plutot que le temps
			
			//la course en elle-meme (dimanche)
			
			shmdt(shCoureurs);
			shmdt(clas);
			exit(-1);
		}
		else{
			
			if (i == n-1){
				while (clas->start != 1){} //a quelle fréquence?
				printf("\n");
				while(clas->current_time<=duree){
					//if(clas->current_time % 2 == 0){
						sleep(1);
						//sem_wait(&listeSem[1]);//il ferme la porte aux voitures
						if (semop(semId, &semOp[5], 1) == -1) {
						       perror("semop");
						       exit(EXIT_FAILURE);
						}
						printf("Je veux afficher\n");
						
						while(clas->compteur > 0);//il attend que celles qui ont déja commencé leur tour aie fini
						
						system("clear");
						
						affichage(tri(copy, shCoureurs,sizeof(struct voiture), nbr), n, clas);

						if (semop(semId, &semOp[3], 1) == -1) {
						       perror("semop");
						       exit(EXIT_FAILURE);
						}

				}
					
				printf("\n");
			}
		}
	};
	
	affichageTxt(tri(copy, shCoureurs,sizeof(struct voiture), nbr), nbr, clas, "rapportQ1.txt");
	
	


	//détache les zones de MP
	shmdt(shCoureurs);
	shmdt(clas);

	
	//supprime les zones de MP
	shmctl(shmId, IPC_RMID, NULL);
	shmctl(shmIdClas, IPC_RMID, NULL);
	semctl(semId,2, IPC_RMID);
}
