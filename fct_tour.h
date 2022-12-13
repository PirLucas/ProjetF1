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
	usleep(100);
	float t1;
	float t2;//quand on fera plusieurs voitures en meme temps, est-ce que une voiture peut modifier le t1 pendant qu'une autre est encore sur le t2->non
	float t3;
	t1 = genererTemps(v->id, v->tour_fait);
	v->tps_S1 = t1;
	t2 = genererTemps(v->id, v->tour_fait);
	v->tps_S2 = t2;
	t3 = genererTemps(v->id, v->tour_fait);
	v->tps_S3 = t3;
	/*if(v->tps_S1 > (t1 = genererTemps(v->id, v->tour_fait))){
		v->tps_S1 = t1;
	}*/
	
	/*if(v->tps_S1 < clas.best_S1){
		clas.best_S1= v->tps_S1;
		clas.S1_id = v->id;
	}*/
	if(t1 < clas->best_S1){
		clas->best_S1= t1;
		clas->S1_id = v->id;
	}
	
	
	/*if(v->tps_S2 > (t2 = genererTemps(v->id, v->tour_fait))){
		v->tps_S2 = t2;
	}*/
	
	if(t2 < clas->best_S2){
		clas->best_S2= t2;
		clas->S2_id = v->id;
	}
	
	/*if(v->tps_S3 > (t3 = genererTemps(v->id, v->tour_fait))){
		v->tps_S3 = t3;
	}*/
	
	if(t3 < clas->best_S3){
		clas->best_S3= t3;
		clas->S3_id = v->id;
	}
	
	if(v->m_tour > t1+t2+t3){
		v->m_tour = t1+t2+t3;
	}
	
	
	printf("n°%i: %.3f/",v->id,t1);
	printf("%.3f/",t2);
	printf("%.3f\n",t3);
	v->tour_fait += 1;
	v->dern_tour = t1+t2+t3;
	v->total += t1+t2+t3;
};

void tourne(struct voiture * v, int d){ //peut-etre pas nécessaire
	while(v->total < d){
		//tour(v);
	}
};
