
int fctsort(const void* t1, const void* t2){
	const struct voiture * a = t1; 
	const struct voiture * b = t2;
	int r = 0;
	if (a->m_tour-b->m_tour < 0){
		r = -1;
	}
	else{
		r= 1;
	}
	return r;
};

float diff(struct voiture v1, struct voiture v2){
	if (v1.position > 1){ //changer localisation de la vérification
		return(v1.m_tour-v2.m_tour);
	}
	else{
		return 0;
	}
}


struct voiture * tri(struct voiture * v,int taille, int n){
	struct voiture * copiedV = v;
	qsort(copiedV, n, taille, fctsort);
	for (int i = 0; i < n; i++){
		copiedV[i].position = i+1;
	}
	return copiedV;
};



