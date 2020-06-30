#ifndef __FIRSTPASS_H__
#define __FIRSTPASS_H__


#define MAX_TA_LINE 81
#define NBR_MAX_COMD 17
#define MIN_NBR_REGIS 0
#define MAX_NBR_REGIS 7
#define MAX_INT 32767 
#define MIN_INT -32767
#define MIN_OGUER 0
#define MAX_OGUER 7
#define MAXLAB 30
#define TYPE1 "string"
#define TYPE2 "data"
#define TYPE3 "entry"
#define TYPE4 "extern"

#include "DataManager.h"


typedef struct{
	char * namecmd;
	int opcode;
	int nbrop;
	int ope1;
	int ope2;
}Command;


void firstPass(FILE * ,Label * ,Data * ,Info * ,Entry * );

#endif 	/*__FIRSTPASS_H__*/
