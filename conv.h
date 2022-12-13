char * minSec(float tpsFloat){
		int tpsInt = (int) tpsFloat;
		float milli;
		int secInt;
		float secFloat;
		int min;
		char * minChar = malloc(sizeof(char) * 7);
		char secChar[10];
		
		min = tpsInt / 60;
		secInt = tpsInt % 60;
		milli = tpsFloat - tpsInt;
		secFloat = secInt + milli;
		
		sprintf(minChar, "%i", min);
		sprintf(secChar, "%.3f", secFloat);
		strcat(minChar, ",");
		strcat(minChar, secChar);
		return(minChar);
		
}
