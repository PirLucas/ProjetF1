struct voiture{
	int id;
	float tps_S1;//impossible de donner une valeur par defaut lors de l'initialisation
	float tps_S2;
	float tps_S3;
	float dern_tour;
	float m_tour;
	int tour_fait;
	float total;
	int etat;
	//char etat[10]; //0-6=en course, 7-8 = P, 9=Out
	int position;//abandonné pour le moment
};


struct classement{
	int start;
	int time_start;
	int current_time;
	float best_S1;
	int S1_id;
	float best_S2;
	int S2_id;
	float best_S3;
	int S3_id;
	float best_total;
	int total_id;
	int compteur;
	int flag;
};




