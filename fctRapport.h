void affichageTxt(struct voiture * v,int nbr,struct classement * clas){
	FILE* f = fopen("rapport.txt", "w");  // open the file for writing
	if (f != NULL)                       // check for success
	{
	    
	    	fprintf(f,"meilleurs temps\n");
		fprintf(f,"S1\t n° %i\t%.3f\n",clas->S1_id, clas->best_S1);
		fprintf(f,"S2\t n° %i\t%.3f\n",clas->S2_id, clas->best_S2);
		fprintf(f,"S3\t n° %i\t%.3f\n",clas->S3_id, clas->best_S3);
		fprintf(f,"tour\t n° %i\t%s\n",clas->total_id, minSec(clas->best_total));
		fprintf(f,"resumé dernière course\n");
		
		fprintf(f,"%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-16s%s\n","n","S1","S2","S3","dernier tour","meilleur tour","nb","diff","etat");	
		/*for (int i = 0; i < nbr; i++){
			fprintf(f,"%i\t%.3f\t%.3f\t%.3f\t%s\t%s\t%i\t+%.3f\t%s\n", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3, minSec(v[i].dern_tour),minSec(v[i].m_tour),v[i].tour_fait,diff(v[i], v[i-1]),v[i].etat);
		}*/
		for (int i = 0; i < nbr; i++){
			fprintf(f,"%-15i%-15.3f%-15.3f%-15.3f%-15s%-15s%-15i+%-15.3f%s\n", v[i].id, v[i].tps_S1, v[i].tps_S2, v[i].tps_S3, minSec(v[i].dern_tour),minSec(v[i].m_tour),v[i].tour_fait,diff(v[i], v[i-1]),v[i].etat);
		}
		
		
		
	    
		fclose(f);                       // close the file
		f = NULL;                        // set file handle to null since f is no longer valid
	}
	
}
