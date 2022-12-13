float genererTemps(int id, int tr){
	int t = random();
	float f;
	t = t/id;
	t = t/(tr+1);
	f = (t%20000)/1000.0;
	f = 25+f;
	return f;
};

void tour(struct voiture * v, struct classement * clas){ //la fonction attend un pointeur sur une structure voiture
	usleep(100000);//ralenti les voitures pour pas qu'elles aille trop vite
	
	v->tps_S1 = genererTemps(v->id, v->tour_fait);
	v->tps_S2 = genererTemps(v->id, v->tour_fait);
	v->tps_S3 = genererTemps(v->id, v->tour_fait);
	
	if(v->tps_S1 < clas->best_S1){
		clas->best_S1= v->tps_S1;
		clas->S1_id = v->id;
	}
	
	if(v->tps_S2 < clas->best_S2){
		clas->best_S2= v->tps_S2;
		clas->S2_id = v->id;
	}
	
	if(v->tps_S3 < clas->best_S3){
		clas->best_S3= v->tps_S3;
		clas->S3_id = v->id;
	}
	
	if(v->m_tour > v->tps_S1+v->tps_S2+v->tps_S3){
		v->m_tour = v->tps_S1+v->tps_S2+v->tps_S3;
	}
	if(v->m_tour < clas->best_total){
		clas->best_total = v->m_tour;
		clas->total_id = v->id;
	}
	
	
	printf("n°%i: %.3f/",v->id,v->tps_S1);
	printf("%.3f/",v->tps_S2);
	printf("%.3f\n",v->tps_S3);
	v->tour_fait += 1;
	v->dern_tour = v->tps_S2+v->tps_S1+v->tps_S3;
	v->total += v->tps_S1+v->tps_S2+v->tps_S3;
};

void tourne(struct voiture * v, int d,struct classement * clas){ //peut-etre pas nécessaire
	while(clas->current_time<=d){
		clas->current_time=time(NULL)-clas->time_start; //obtien le temps en secondes depuis début du tour
		tour(v, clas);
		
	}
};
