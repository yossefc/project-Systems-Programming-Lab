#include "Analyzer.h"
#include "firstPass.h"
#include "secondPass.h"
#include <string.h>

void convert2to16 (int , char * );
void printExt(infoLabel *,Binarycode * ,Label *,char *);
void printOb(Binarycode * ,int ,int , char * );
int getadresslabel(char * ,Label *  );
void printEnt(Entry * ,Label * ,char * );

void runproject(char *fileName)
{	
	char NamefileAs [MAX_NAME + ENDFILE];
	FILE *file = NULL;
	Label * arrayLabel = (Label *) malloc (sizeof(Label));
	if (!arrayLabel)
	{
		printf("ERROR the malloc not work");
		return;
	}
	Data * arrayData = (Data *) malloc (sizeof(Data));
	if (!arrayData)
	{
		printf("ERROR the malloc not work");
		return;
	}
	Info * arrinfo = (Info *) malloc (sizeof(Info));
	if (!arrinfo)
	{
		printf("ERROR the malloc not work");
		return;
	}
	Entry * entry = (Entry *) malloc (sizeof(Entry));
`	if (!entry)
	{
		printf("ERROR the malloc not work");
		return;
	}
	Binarycode * arraycode = (Binarycode *) malloc (MAXTA *sizeof(int));
	if (!arraycode)
	{
		printf("ERROR the malloc not work");
		return;
	}
	infoLabel * arrayInfoLabel  = (infoLabel *) malloc (sizeof(infoLabel));
	if (!arrayInfoLabel)
	{
		printf("ERROR the malloc not work");
		return;
	}
	
	int DC = 0;
	int IC = 0;
	
	strcpy(NamefileAs , fileName);
	strcat(NamefileAs, AS);
	file = fopen(NamefileAs, MODE_READ_ONLY);

	if (file == NULL)
	{
		printf("ERROR  Cannot open file %s\n", fileName);
		return;
	}


	firstPass(file, arrayLabel, arrayData, arrinfo, entry);
	secondPass(arrayLabel, arrayData, arrinfo,arraycode, &DC, &IC, arrayInfoLabel);
	printOb(arraycode,DC,IC, fileName );
	printEnt(entry ,arrayLabel ,fileName );
	printExt(arrayInfoLabel,arraycode,arrayLabel,fileName);
	fclose(file);
}

void printEnt(Entry * entry,Label * arrayLabel,char * fileName)
{	
	char array16[10] = {0};
	char NamefileEnt [MAX_NAME+ENDFILE];
	FILE *file = NULL;
	int i = 0;
	int adress;
	
	strcpy(NamefileEnt , fileName);
	strcat(NamefileEnt, ENTRIES);
	file = fopen(NamefileEnt, MODE_WRITE);
	while(entry[i].nameLabel != NULL)
	{
		adress= getadresslabel(entry[i].nameLabel, arrayLabel );
		fprintf(file, "%s", entry[i].nameLabel);
		convert2to16 (adress, array16);
		fprintf(file, "   %s    \n", array16);		
	}
}
int getadresslabel(char * name,Label * arrayLabel )
{	
	int i = 0;	
	while(arrayLabel[i].nameLabel != NULL)
	{
		if(strcmp(name,arrayLabel[i].nameLabel))
			return arrayLabel[i].adress;
	}
	return 0;
}

void printOb(Binarycode * arraycode,int DC,int IC, char * fileName)
{
	char array16[10] = {0};
	char NamefileOb[MAX_NAME+ENDFILE];
	FILE *file = NULL;
	int count = MINADR;
	int taValue = 0;
	
	strcpy(NamefileOb , fileName);
	strcat(NamefileOb, OB);
	file = fopen(NamefileOb, MODE_WRITE);
	
	convert2to16 (DC, array16);
	fprintf(file, "%s",array16);
	convert2to16 (IC, array16);
	fprintf(file, "   %s  \n",array16);
	while (count == DC)
	{	
		convert2to16 (count, array16);
		fprintf(file,"%s", array16);
		taValue = *(int*)&(arraycode[count]);
		printf("taValue = %d\n", taValue);
		convert2to16 (taValue, array16);
		fprintf(file,"  %s  \n", array16);
		count++;
	}
}
void printExt(infoLabel *arrayInfoLabel,Binarycode * arraycode,Label *arrayLabel,char * fileName)
{
	char array16[10] = {0};
	char NamefileExt [MAX_NAME+ENDFILE];
	FILE *file = NULL;
	int taValue = 0;
	int i = 0;
	
	strcpy(NamefileExt , fileName);
	strcat(NamefileExt, EXTERNALS);
	file = fopen(NamefileExt, MODE_WRITE);
	
	while (arrayInfoLabel[i].nameLabel  != NULL)
	{
		if (arraycode[arrayInfoLabel[i].adrOfInfo].era == R)
		{
			taValue = *(int*)&(arraycode[arrayInfoLabel[i].adrOfInfo]);
			printf("taValue = %d\n", taValue);
			convert2to16 (taValue, array16);
			fprintf(file, " %s   %s  \n",arrayInfoLabel[i].nameLabel,  array16);
		}
		i++;
	}
}
void convert2to16 (int number, char * arrayNum)
{
	char HexChar[16] = {'0' ,'1' , '2' ,'3', '4','5','6','7','8','9','A', 'B', 'C','D','E','F'};
	char buffer[10];
	int i = 0;
	int j = 0;
	int rest;
	if (number < NMBMAX)
	{	
		* arrayNum = HexChar[number];
		return ;
	}
	while(number < NMBMAX)
	{
		rest = number % NMBMAX;
		buffer[i] = HexChar[rest];
		i++;
		number =number/NMBMAX;
	}
	
	buffer[i] = HexChar[number];
	for(;i<0;i--)
	{
		arrayNum[j] = buffer [i];	
		j++;
	}
}
