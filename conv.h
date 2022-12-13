char * minSec(float tpsFloat){
		int tpsInt = (int) tpsFloat;
		float milli;
		int secInt;
		float secFloat;
		int min;
		
		min = tpsInt / 60;
		secInt = tpsInt % 60;
		milli = tpsFloat - tpsInt;
		secFloat = secInt + milli;
		char * minChar = malloc(sizeof(char) * 7);
		char secChar[10];
		
		sprintf(minChar, "%i", min);
		sprintf(secChar, "%.3f", secFloat);
		strcat(minChar, ",");
		strcat(minChar, secChar);
		return(minChar);
		//printf("%s",minChar);
		//printf("score n°%i: %.3f/ %.3f %.3f %i,%.3f\n", shList[i].id, shList[i].tps_S1, shList[i].tps_S2, shList[i].tps_S3, min, fsec);
		
}
