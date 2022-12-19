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
