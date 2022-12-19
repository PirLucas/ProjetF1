float genererTemps(int id, int tr){
	int t = random();
	float f;
	t = t/id;
	t = t/(tr+1);
	f = (t%20000)/1000.0;
	f = 25+f;
	return f;
};

int genererEtat(struct voiture * v){//est-ce que c'est une modification des data qui demandera de reattacher? !! attacher dans chanque fils
	int r =(((int) (genererTemps(v->id, v->tour_fait)*1000))%100);
	int etat;
	if (r <= 90){
		etat = 0;
	}
	else if ((r>90) && (r<99)){
		etat = 1;//tout les X? au moins un pendandt la course du dimanche, le plus rapide possible!!25sec
	}
	else if (r == 99){
		etat = 2;
	}
	return etat;
}

void tour(struct voiture * v, struct classement * clas, int semId, struct sembuf* semOp){ //la fonction attend un pointeur sur une structure voiture
	usleep(100000);//ralenti les voitures pour pas qu'elles aille trop vite
	
	/*int val;
	sem_getvalue(&sem[1], &val);
	//printf("%d,", val);
	while(val == 0){
		sem_getvalue(&sem[1], &val);
		printf("%d,", val);
	};
	printf("\n");*/
	
	
	if (semop(semId, &semOp[4], 1) == -1) {
	       perror("semop");
	       exit(EXIT_FAILURE);
	}
	/*
	if(clas->flag == 1){ //elles ne font rien tant que l'afficheur veut afficher
		int val;
		sem_getvalue(&sem[1], &val);
		printf("avant%d,", val);
		sem_wait(&sem[1]);
		sem_getvalue(&sem[1], &val);
		printf("apres%d,", val);
		printf("flag, %i\n", clas->flag);
	}
	*/
	//sem_wait(&sem[0]);//exclusion mutuelle pour le compteur de voiture (pas nécessaire parce que ++, mais toujour bien a avoir)
	clas->compteur++;
	//compteur nombre de voitures++
	//sem_post(&sem[0]);
	
	v->tps_S1 = genererTemps(v->id, v->tour_fait);
	v->tps_S2 = genererTemps(v->id, v->tour_fait);
	v->tps_S3 = genererTemps(v->id, v->tour_fait);
	
	if(v->tps_S1 < clas->best_S1){
		if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		
		clas->best_S1= v->tps_S1;
		clas->S1_id = v->id;

		if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
	}

	
	if(v->tps_S2 < clas->best_S2){
		if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		clas->best_S2= v->tps_S2;
		clas->S2_id = v->id;
		if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
	}
	
	if(v->tps_S3 < clas->best_S3){
		if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		clas->best_S3= v->tps_S3;
		clas->S3_id = v->id;
		if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
	}
	if ((v->etat = genererEtat(v)) == 2){
		if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		clas->compteur--;
		printf("compteur:%i\n", clas->compteur);
		//compteur nombre de voitures--
		if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		return;
	}
	else if (v->etat == 1){
		usleep(100000);
		v->tps_S3 += 25;
		v->etat = 0;
	}
	
	if(v->m_tour > v->tps_S1+v->tps_S2+v->tps_S3){
		v->m_tour = v->tps_S1+v->tps_S2+v->tps_S3;
	}
	if(v->m_tour < clas->best_total){
		if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		clas->best_total = v->m_tour;
		clas->total_id = v->id;
		if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
	}
	//v->etat = genererEtat(v);
	
	//printf("n°%i: %.3f/",v->id,v->tps_S1);
	//printf("%.3f/",v->tps_S2);
	//printf("%.3f\n",v->tps_S3);
	v->tour_fait += 1;
	v->dern_tour = v->tps_S2+v->tps_S1+v->tps_S3;
	v->total += v->tps_S1+v->tps_S2+v->tps_S3;
	
	if (semop(semId, &semOp[1], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
		if (semop(semId, &semOp[2], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
	clas->compteur--;
	//printf("compteur:%i\n", clas->compteur);
	//compteur nombre de voitures--
	if (semop(semId, &semOp[0], 1) == -1) {
	        perror("semop");
	        exit(EXIT_FAILURE);
		}
};

void tourne(struct voiture * v, int d,struct classement * clas,int sem, struct sembuf* semOp){ //peut-etre pas nécessaire
	while(clas->current_time<=d){
		clas->current_time=time(NULL)-clas->time_start; //obtien le temps en secondes depuis début du tour
		if (v->etat == 2){return;};
		tour(v, clas, sem, semOp);
		
	}
};
