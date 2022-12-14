void affichageTxt(struct voiture * v,int nbr,struct classement * clas){
	FILE* f = fopen("rapport.txt", "w");  // open the file for writing
	if (f != NULL)                       // check for success
	{
	    
	    	fprintf(f,"meilleurs temps\n");
		fprintf(f,"S1\t n° %i\t %.3f\n",clas->S1_id, clas->best_S1);
		fprintf(f,"S2\t n° %i\t %.3f\n",clas->S2_id, clas->best_S2);
		fprintf(f,"S3\t n° %i\t %.3f\n",clas->S3_id, clas->best_S3);
		fprintf(f,"tour\t n° %i\t %.3f\n",clas->total_id, clas->best_total);
		fprintf(f,"resumé dernière course\n");
		
		fprintf(f,"%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-16s%s\n","n","S1","S2","S3","dernier tour","meilleur tour","nb","diff","etat");	
		/*for (int i = 0; i < nbr; i++){
			fprintf(f,"%i\t%.3f\t%.3f\t%.3f\t%s\t%s\t%i\t+%.3f\t%s\n", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3, minSec(v[i].dern_tour),minSec(v[i].m_tour),v[i].tour_fait,diff(v[i], v[i-1]),v[i].etat);
		}*/
		for (int i = 0; i < nbr; i++){
			fprintf(f,"%-15i%-15.3f%-15.3f%-15.3f%-15f%-15f%-15i+%-15.3f%i\n", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3,v[i].dern_tour,v[i].m_tour,v[i].tour_fait,diff(v[i], v[i-1]),v[i].etat);
		}
		
		
		
	    
		fclose(f);                       // close the file
		f = NULL;                        // set file handle to null since f is no longer valid
	}
	
}


int creeVoitures(struct voiture * v, struct classement * clas, char * rapport){
	
	
	int fd;
	char* ligne;
	char* mot;
	char* save = NULL;
	struct stat st;
	char ** lignes;
	int n = 0;
   	
	
	
	if ((fd = open("rapport.txt", O_RDONLY)) == -1){ //ouvre et vérifie l'ouverture du fichier
     		printf("erreur open");
      		exit(-2);
   	}
   	if(fstat(fd, &st) !=0){ //crée la stucture stat du fichier afin d'obtenir sa taille 
    		printf("erreur stat");
   		exit(-3);
  	}
  	
  	char fich[st.st_size];
	if(read(fd, fich, st.st_size) == -1){ //lecture du fichier
        	printf("erreur read");
        	exit(-5);
    	}
    	char* str = fich;
    	
    	if((lignes = (char**) malloc(st.st_size)) == NULL){ //initialisation du tableau contenant chaque ligne 
    		printf("erreur malloc ligne");
    		exit(-7);
    	}

	int cptLigne = 0;
	int cptMots = 0;
	while ((ligne = strtok_r(str, "\n",&save)) != NULL){
		if (ligne == NULL){
			lignes[cptLigne] = NULL;
			break;
		}
		//printf("%s", ligne);
		lignes[cptLigne++] = ligne;
		//printf("\n");
		str = NULL;
	}
	
	for (int l = 0 ; l < cptLigne-1; l++){
		char * str2 = lignes[l];
		cptMots = 0;
		while ((mot = strtok_r(str2, " ",&save)) != NULL){
			if ((l == 1)){
				if (cptMots == 2){
					clas->S1_id = atoi(mot);
					//printf("a%s\n", mot);
				}
				else if(cptMots == 3){
					clas->best_S1 = (float)strtod(mot,NULL);
				}
			}
			else if ((l == 2)){
				if (cptMots == 2){
					clas->S2_id = atoi(mot);
					//printf("a%s\n", mot);
				}
				else if(cptMots == 3){
					clas->best_S2 = (float)strtod(mot,NULL);
				}
			}
			else if ((l == 3)){
				if (cptMots == 2){
					clas->S3_id = atoi(mot);
					//printf("a%s\n", mot);
				}
				else if(cptMots == 3){
					clas->best_S3 = (float)strtod(mot,NULL);
				}
			}
			else if ((l == 4)){
				if (cptMots == 2){
					clas->total_id = atoi(mot);
					//printf("a%s\n", mot);
				}
				else if(cptMots == 3){
					clas->best_total = (float)strtod(mot,NULL);
				}
			}
			else if ((l >= 7)){
				if (cptMots == 0){
					v[l-7].id = (float)strtod(mot,NULL);
					v[l-7].position = l-6;
					//printf("%s ", mot);
					n++;
				}
				else if(cptMots == 1){
					v[l-7].tps_S1 = (float)strtod(mot,NULL);
					//printf("%s ", mot);
				}
				else if(cptMots == 2){
					v[l-7].tps_S2 = (float)strtod(mot,NULL);//printf("%s ", mot);
				}
				else if(cptMots == 3){
					v[l-7].tps_S3 = (float)strtod(mot,NULL);//printf("%s ", mot);
				}
				else if(cptMots == 4){
					v[l-7].dern_tour = (float)strtod(mot,NULL);//printf("%s ", mot);
					//printf("%f,%s", v[l-7].dern_tour,mot );//C'EST PAS DES VRAIS FLOATSSSSSSSSSSSSSSSSSSSSS
				}
				else if(cptMots == 5){
					v[l-7].m_tour = (float)strtod(mot,NULL);//printf("%s ", mot);
					
				}
				else if(cptMots == 6){
					v[l-7].tour_fait = (float)strtod(mot,NULL);//printf("%s ", mot);
				}
				else if(cptMots == 8){
					v[l-7].etat = atoi(mot);
					//printf("%s\n", v[l-7].etat);
					
				}
				
				
			}
			str2 = NULL;
			cptMots++;
		}
		//printf("n%i\n",n);
	}
	close(fd);
	//printf("#############test1%s\n", v[0].etat);
	clas->start = 0;
	clas->time_start = 0;
	clas->current_time = 0;
	
	
	
	
	return n;

}
