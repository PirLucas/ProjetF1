//seance d'1h d'essais
//est-ce que quand toutes les voitures tournent en mm tmeps, elles font toutes un tour, puis un tour....->NON
//->des que un total dépasse le temps max -> on stoppe
//nop, on utilise du temps réel

void tourne(struct voiture * v, int nbr, int d){ //peut-etre pas nécessaire
	float temps = 0;
	while (temps < d){
		for (int i = 0; i < nbr; i++){
			tour(v[i]);
			if (v[i].total > temps){
				temps = v[i].total;
			}
		}
	}
};
