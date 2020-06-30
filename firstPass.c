#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "firstPass.h"
#include <stdio.h>


Bool isEmpty(char* ta);
Bool isComment(char* ta);
char* passSpace(char *ta);
void getLabel( Label * );
Bool isCommand(char * );
Bool isRegister(char * );
void check_label(Label * ,Label *  );
void check_labelData(Label *  ,Label *  );
void cmdData(Label * ,Data *,char *,Label *);
void DataEnter(Label *,Data *  ,char * );
void StringEnter (Label * ,Data * ,char *);
void getcmd(Info *);
void getopdir(op * );
void getNumber(op * );
void getLab (op *);
void cmdop( op * );
void getEntry(char * ,Entry *);
void getExtern(Label * ,char * );
Bool checkcmdop(Info *);

int nbrData = 1 ;
int nbrLabel = 1;
int nbrinfo = 1;
int nbrEntry = 1;
int nbrtalab = 1;
int adresse = 0 ;
int linebug = 0;
Bool check = True;
char * linetoArr;

 Command paramCommands[] = 
{
	{ "mov" , 0, 2 ,4 ,3},
	{ "cmp" , 1, 2 ,4 ,4},
	{ "add" , 2, 2 ,4 ,3},
	{ "sub" , 3, 2 ,4 ,3},
	{ "not" , 4, 1 ,0 ,3},
	{ "clr" , 5, 1 ,0 ,3},
	{ "lea" , 6, 2 ,2 ,3},
	{ "inc" , 7, 1 ,0 ,3},
	{ "dec" , 8, 1 ,0 ,3},
	{ "jmp" , 9, 1 ,0 ,3},
	{ "bne" , 10,1 ,0 ,3},
	{ "red" , 11,1 ,0 ,3},
	{ "prn" , 12,1 ,0, 4},
	{ "jsr" , 13,1 ,0, 3},
	{ "rts" , 14,0 ,0, 0},
	{ "stop", 15,0 ,0, 0},
	{ '\0'   , 0 ,0 ,0, 0}
};

void firstPass(FILE *_pFile,Label * arrayLabel,Data * arrayData,Info * arrinfo,Entry * entry)
{

	Label * label = (Label *) malloc (sizeof(Label)) ;
	if (!label)
	{
		printf("ERROR the malloc not work");
		return;
	}
	Info * info = (Info *) malloc (sizeof(Info));
	if (!info)
	{
		printf("ERROR the malloc not work");
		return;
	}
	op * op1 = (op *) malloc (sizeof(op));
	*if (!op1 )
	{
		printf("ERROR the malloc not work");
		return;
	}
	op * op2 = (op *) malloc (sizeof(op)); 
	*if (!op2)
	{
		printf("ERROR the malloc not work");
		return;
	}
	char line_ta[MAX_TA_LINE] ;

	while (fgets (line_ta, MAX_TA_LINE , _pFile))/* entry the line in array*/
	{	
		linebug++;
		linetoArr = line_ta;	
			
		if (line_ta[MAX_TA_LINE] == '\0' )
		{
			printf("ERROR: the length of the line %d is too long",linebug);	
		}
		else if (!(isEmpty(linetoArr) || isComment(linetoArr)))
		{
			 getLabel(label);
			linetoArr = passSpace(linetoArr);
			check_label(arrayLabel,label);
			if(check == True)/* check if label is command or data*/
			{
				if (*linetoArr == EOF || *linetoArr == '\n' || *linetoArr == '\0' )
				{
					printf("ERROR to line %d must after label data or command",linebug);
					check = False;
				}
				if (*linetoArr == '.')/*is data*/
				{	
					cmdData(label,arrayData,linetoArr,arrayLabel);
				}
				else /*check command*/
				{
					getcmd(info);/*enter the command*/
					if (check == True)
					{
							cmdop(op1);
							info[0].op1 = op1 ;
							(info->nbrop)++;
							linetoArr++;
						if(*linetoArr== ' ' || *linetoArr== '\t')
							linetoArr = passSpace(linetoArr);
						if (check == True && *linetoArr == ',')
						{
							cmdop(op2);
							info[0].op2 = op2 ;
							(info->nbrop)++;
						}
						if (check == True && checkcmdop(info))
						{	
							arrinfo[nbrinfo].cmd = info->cmd;
							arrinfo[nbrinfo].nbrop = info->nbrop;
							arrinfo[nbrinfo].op1 = info->op1;
							arrinfo[nbrinfo].op2 = info->op2;
							strcpy(arrinfo[nbrinfo].nameLabel,label->nameLabel);
							arrinfo[nbrinfo].linebug = linebug;
							nbrinfo++;
							arrinfo = realloc (arrinfo ,  (sizeof(Info *) * nbrinfo));
							if (!arrinfo)
							{
								printf("ERROR the malloc not work");
								return;
							}
						
							if(label->nameLabel != NULL )
							{
								strcpy(arrayLabel[nbrtalab].nameLabel,label->nameLabel);
								arrayLabel[nbrtalab].adress = adresse;
								arrayLabel[nbrtalab].isData = False;
								nbrLabel++;
								arrayLabel = realloc (arrayLabel,nbrtalab*sizeof(Label));
								*if (!arrinfo)
								{
									printf("ERROR the malloc not work");
									return;
								}
							}
							if (info->nbrop == 2 && info->op1->opType == OGERDIRECT && info->op2->opType == OGERDIRECT)	
								adresse = 2;
							else if (info->nbrop == 2)
								adresse = 3;
							else if (info->nbrop == 1)
								adresse = 2;
							else 
								adresse = 1;
							
						}	
						else if (!(paramCommands[info->cmd].nbrop==info->nbrop))
						{
							printf("ERROR to line %d the number operand is not compatible ",linebug);
							check = False;
						}
					}
				}	 
			
			}	
			else if(*linetoArr == '.')/* extern or entry*/
			{
				char nameCmd [MAX_TA_LINE];
				int i = 0;
				linetoArr++;
				while (*linetoArr != '\n' && *linetoArr != '\0')
				{
					nameCmd[i] = *linetoArr;
					linetoArr++;
					i++;
				}
				passSpace(linetoArr);
				if (!strcmp(nameCmd,TYPE3))/*.entry*/
				{
					getEntry(linetoArr,entry);
				}
				else if (!strcmp(nameCmd,TYPE4))/*.extern*/
				{
					getExtern(arrayLabel, linetoArr);
				}
				else
				{
					printf("ERRRO to line %d the command is not correct",linebug);
				}
			}
		}
	}
	
}	

void getExtern(Label * arrayLabel,char * line)
{
	char nameCmd [MAX_TA_LINE];
	int i = 0;
	if (*line == EOF || *line == '\n' || *line == '\0' )
	{
		printf("ERROR to line %d there not extern",linebug);
		return;	
	}	
	while (*line != '\n' && *line != '\0')
	{
		nameCmd[i] = *line;
		line++;
		i++;
	}
	strcpy(arrayLabel[nbrtalab].nameLabel,nameCmd);
	arrayLabel[nbrtalab].adress = 0;
	arrayLabel[nbrtalab].isData = False;
	arrayLabel[nbrtalab].isextern = True;
	nbrtalab++;
	arrayLabel = realloc (arrayLabel,nbrtalab*sizeof(Label));
}


void getEntry(char * line,Entry *entry)
{
	char nameCmd [MAX_TA_LINE];
	int i = 0;
	if (*line == EOF || *line == '\n' || *line == '\0' )
	{
		printf("ERROR to line %d there not entry",linebug);
		return;	
	}	
	while (*line != '\n' && *line != '\0')
	{
		nameCmd[i] = *line;
		line++;
		i++;
	}
	strcpy(entry[nbrEntry].nameLabel,nameCmd);
	entry[nbrEntry].linebug = linebug;
	nbrEntry++;
	entry = realloc (entry,nbrEntry*sizeof(Entry));
}


Bool checkcmdop(Info * info)
{
	if(!(paramCommands[info->cmd].nbrop==info->nbrop))
		return False;
	if(paramCommands[info->cmd].nbrop == 1)
	{	
		if ((info->op1->opType !=IMEDIATELY || info->cmd == 12) && info->op2 == NULL)
			return True;
		else	
			return False;
	}
	if (paramCommands[info->cmd].nbrop == 2)
	{
		if (info->cmd != 7 && info->cmd != 1 && info->op1!= NULL && info->op1->opType != IMEDIATELY)
		{
			return True;
		}
		else if (info->cmd == 7 && (info->op1->opType == DIRECT || info->op1->opType == INDEX ) && info->op2->opType != IMEDIATELY)
			return True;
		else if (info->cmd == 1 && (info->op1 != NULL && info->op2 != NULL))
			return True;
		else
			return False;
	}
	return True;
}


void cmdop( op * op)
{
	linetoArr = passSpace(linetoArr);
	if(*linetoArr == 'r')/*oguer or label or index*/
	{
		getopdir(  op);
	}
	else if (*linetoArr == '#')
	{
		getNumber(op);
	}
	else/*label*/
	{
		getLab(op);
	}
}


void getLab (op * op)
{
	int i =0;
	char NameOpLab [MAXLAB] = "\0";
	linetoArr++;
	linetoArr = passSpace(linetoArr);
		
	while (*linetoArr != '\n' && *linetoArr != ':' && *linetoArr != '\0' && *linetoArr != ' ' && *linetoArr != '\t' &&*linetoArr !=',')
	{
	printf("ofokpfnafonaw\n");
		NameOpLab[i] = *linetoArr;
		linetoArr++;
		i++;
	}
	strcpy(op->name, NameOpLab);
	op->opType = DIRECT;
	
}


void getNumber(op * op)
{	
	int number;  
	linetoArr++;/* # */
	
	if (sscanf(linetoArr,"%d",&number))
	{
		op->opType = IMEDIATELY;
		op->adr = -1;
		op->number = number;
		while (!(*linetoArr == EOF || *linetoArr == '\n' || *linetoArr == '\0' || *linetoArr ==',' || *linetoArr == ' ' || *linetoArr =='\t'))
			linetoArr++;
	}
	else
	{
		printf("ERROR to line %d the number of operand is not correct",linebug);
		check = False;
	}
}

/*good*/
void getopdir(op * op)
{
	int i =0;
	char operand[MAXLAB] = "\0";	
	operand[i] = *linetoArr;
	i++;
	linetoArr++;
		if(*linetoArr == EOF || *linetoArr == '\n' || *linetoArr == '\0' ||*linetoArr == ' ' || *linetoArr == '\t')/* (r) is operand*/
			{
				op->opType = DIRECT;
				op->adr = -1;
				strcpy(op->name , operand);
				return;
			}
		else if(atoi(linetoArr)>=MIN_OGUER && atoi(linetoArr)<=MAX_OGUER)/* oger or index (r0-7...)*/ 
		{	
			operand[i] = *linetoArr;
			i++;	
			linetoArr++;		
			if ( *linetoArr == ' ' ||*linetoArr == '\t')
				linetoArr = passSpace(linetoArr);
			if(*linetoArr == '[')/*index (r0-7[...)*/
			{
				if(atoi(&operand[i-1])%2)/*(r1[)*/
				{	
					linetoArr++;
					if ( *linetoArr == ' ' || *linetoArr == '\t')
						linetoArr = passSpace(linetoArr);	
					if(*linetoArr == 'r')/*(r1[r...)*/
					{	
						linetoArr++;
						operand[i] =*linetoArr;
						if(!(atoi(&operand[i])%2 && atoi(&operand[i])>=MIN_OGUER && atoi(&operand[i])<=MAX_OGUER))/*(r1[r2)*/
						{
							linetoArr++;
							linetoArr = passSpace(linetoArr);
							if (*linetoArr == ' ' ||  *linetoArr == '\t')
								linetoArr = passSpace(linetoArr);
							if (*linetoArr == ']')
							{
								op->opType = INDEX;
								op->adr = -1;
								*(op->name) = '\0';
								strcpy(op->name,operand);
								printf("le nombre qui suit3 %s \n",op->name);
								return;
							}
							else
							{
								printf("ERROR to line %d the operator must end is ]",linebug);
								check = False;
								return;
							}
						}
						else
						{
							printf("ERROR to line %d the operator is strong must number pair or in interval between 0-7",linebug);
							check = False;
							return;
						}
					}
					else
					{
						printf("ERROR o line %d the operator in []is not correct must oguer and start by r",linebug);
						check = False;
						return;
					}
				}
				else
				{
					printf("ERROR o line %d the operator in is not correct number's oguer must odd",linebug);
					check =  False;
					return;
				}
			}
			else if(*linetoArr == ',' || *linetoArr == EOF || *linetoArr == '\n' || *linetoArr == '\0' ||  *linetoArr == ' ' || *linetoArr == '\t')/*oguer (r3)*/
			{
				op->opType = OGERDIRECT;
				strcpy(op->name , operand);
				op->adr = -1;
			}
			else  /*label r3...*/
			{
				while(! (*linetoArr == EOF || *linetoArr == '\n' || *linetoArr == '\0' || *linetoArr == ','|| *linetoArr == ' ' || *linetoArr == '\t' ))
				{
					operand[i] = *linetoArr;
					i++;
					linetoArr++;
				}
				op->opType = DIRECT;
				op->adr = -1;
				strcpy(op->name , operand);
				return;
			}			
		}
		
}

/*good*/
void getcmd(Info * info)
{
	int i = 0;
	int count = 0;
	char namecmd[MAX_TA_LINE] = "\0";
	
	/*enter the name of the command in the char*/
	while ( *linetoArr != '\n' && *linetoArr != ':' && *linetoArr != '\0' && *linetoArr != ' ' && *linetoArr != '\t')
	{
		namecmd[count]=*linetoArr;
		linetoArr++;
		count++;
	}
	/*check the number is not empty*/
	if (!count)
	{
		printf("ERROR to line %d without command the line is empty",linebug);
		check = False;
		return;
	}
	
	/*add the number of the command*/
	while (paramCommands[i].namecmd)
	{
		if (! (strcmp(paramCommands[i].namecmd, namecmd)))
		{
			info->cmd = i;
			return;
		}
		i++;
	}
	
	printf("ERROR to line %d the commnand is not correct",linebug);
	check = False;
	
}


void cmdData(Label *label,Data * arrayData ,char * line,Label * arraylabel)
{
	char nameCmd [MAX_TA_LINE];
	int i = 0;
	int ta_la = nbrData;
	line++;
	while (*line != '\n' && *line != '\0')
	{
		nameCmd[i] = *line;
		line++;
		i++;
	}
	if (!strcmp(nameCmd,TYPE1)) /* the data is string*/
	{
		StringEnter(label,arrayData,line);
	}
	else if (!strcmp(nameCmd,TYPE2)) /* the data is int */
	{
		DataEnter(label,arrayData,line);
	}
	else 
	{
		printf("ERROR to line %d the command is not just",linebug);
		return;
	}
	if (check == True)
	{
		strcpy(arraylabel[nbrLabel].nameLabel , label->nameLabel);
		arraylabel[nbrLabel].adressData = ta_la;
		arraylabel[nbrLabel].adress = ta_la;
		arraylabel[nbrLabel].isData = True;
		nbrLabel++;
		arraylabel = realloc (arraylabel,nbrtalab*sizeof(Label));
		
	}
	else
		nbrData = ta_la;

}


void DataEnter(Label *label, Data * arrayData ,char * line)
{
	int arrData[MAX_TA_LINE];
	int number;	
	Bool isEnd = True;
	

	int i = 0;
	int countda = 0;
	Bool negatif = False;
	passSpace(line);
	if (*line == EOF || * line == '\n' ||  *line == '\0' )
	{
		printf("ERRORto line %d after .data must intenger",linebug);
		check = False;
		return ;
	}
	
	
	while (isEnd)
	{	
		if (*line == '\0')
			passSpace(line);
		negatif = False;
		if(*line == '+' || *line == '-')
		{
			if(*line == '-')
				negatif = True;
			line++;
		}
		if (*line == '\0')
			passSpace(line);
		if (sscanf(line,"%d",&number))
		{	
			if (number <=MAX_INT && number>= MIN_INT)
			{
				if (negatif == True)
					arrData[countda] = number *(-1);
				else
					arrData[countda] = number;
				countda++;
				passSpace(line);		
				if (*line == ',')
				{
					line++;
				}
				else if (*line == EOF || * line == '\n' ||  *line == '\0' )
				{
			
					isEnd = False;
					for ( i = 0 ;i< countda +1;i++)
					{
						arrayData =  realloc(arrayData,nbrData * sizeof(Data));
						strcpy(arrayData[nbrData].nameData , label->nameLabel);
						arrayData[nbrData].adress = nbrData;
						arrayData[nbrData].ta_data = arrData[i];
						nbrData++;
					}
					while(!(*line == EOF || * line == '\n' ||  *line == '\0'))
					{	
					line++;
					}
				}
				else
				{
					printf("ERROR to line %d must , after integer",linebug);
					check = False;
					return;
				}
			}
			else
			{	
				printf("ERROR to line %d the number is long must intenger",linebug);
				check = False;
				return;			
			}
		}		 
		else
			printf("ERROR to line %d the data must intenger",linebug);
			check = False;
			return;
	}
}


void StringEnter (Label * label,Data * arrayData ,char * line)
{
	char  arrDaSt [MAX_TA_LINE];
	int i = 0;
	int y;
	passSpace(line);
	if (*line == '\0'|| *line == '\n' || *line == EOF)
	{
		printf("ERROR to line %d not data",linebug);
		check = False;
		return;
	}
	if (*line != '"')
	{
		printf("ERROR to line %d the data must start with  ",linebug);
		check = False;
		return;
	}	
	line++;
	while(*line == '\n' || *line == '\0' || *line == '"' || *line == EOF )
	{	
		arrDaSt[i] = *line;
		line++;
		i++;
	}
	if (*line == '\n' || *line == '\0' || *line == EOF || !isdigit(*line))
	{
		printf("ERROR to line %d there ara probleme in the data string less "" ",linebug);
		check = False;
		return;
	}
	
	passSpace(line);
	if (!(*line == '\n' || *line == '\0' || *line == EOF))
	{
		printf("ERROR to line %d there are to munch character",linebug);
		check = False;
		return;
	}
	
	for ( y = 0 ; y<=i; y++)
	{	
		arrayData =  realloc(arrayData,nbrData * sizeof(Data));
		strcpy(arrayData[nbrData].nameData , label->nameLabel);
		arrayData[nbrData].adress = nbrData;
		arrayData[nbrData].ta_data = atoi(&arrDaSt[i]);
		nbrData++;
	}
	
	
}

void check_labelData(Label * arrLabData ,Label * label )
{
	int i = 0;
	while (i<nbrLabel)
	{
		if (!strcmp(arrLabData[i].nameLabel , label->nameLabel))
		{
			printf("ERROR to line %d there are the same name label",linebug);
			check = False;				
			return ;
		}
		i++;
	}
}


void check_label(Label * arrLab,Label * label )
{
	int i = 0;
	while (nbrData > i)
	{
		if (!(strcmp(arrLab[i].nameLabel , label->nameLabel)))
		{
			printf("ERROR to line %d there are the same name label",linebug);
			check = False;		
			return;
		}
		i++;
	}
}

Bool isEmpty(char *ta)
{
	printf("the name of label is tex1  \n");
	ta = passSpace(ta);
	if (*(ta) == '\0' || *(ta) =='\n' || *(ta) == EOF)	
		return True;
	return False;
}

Bool isComment(char * ta)
{
	printf("the name of label is tex2  \n");
	ta = passSpace(ta);
	if ( *ta == ';')
		return True;
	return False;
}

/*good*/
char * passSpace(char * ta)
{
		while(*ta == '\0' || *ta =='\n' || *ta == EOF || *ta == ' ' || *ta == '\t')
			ta++;		
	return ta;
}
/*good*/
void getLabel( Label *label)
{
	char word[MAXLAB] = "\0";
	char  * charac = linetoArr ;
	int count_length_label = 0;
	linetoArr = passSpace(linetoArr);
	
	if (!(strchr (linetoArr, ':')))
		return ;/* not label*/
	if (!isalpha(*charac))/*the first character is not char*/
	{	
		printf("ERROR to line %d the label must star with lettre",linebug);
		check = False;
		return ;
	}
	*charac = *linetoArr;
	
	/*take the name Label */
	while (*linetoArr != '\n' && *linetoArr != ':' && *linetoArr != '\0' && *linetoArr != ' ' && *linetoArr != '\t')
	{
		count_length_label++;
		linetoArr++;
	}
	if (count_length_label > MAXLAB)
	{
		check = False;
		return ;
	}

	linetoArr = passSpace(linetoArr);
	/*check ther are :  */
	if (*linetoArr != ':')
	{
		printf("ERROR at line %d after label must :",linebug);
		check = False;
		return ;	
	}
	linetoArr++; /*pass the :*/
	strncpy(word,charac,count_length_label);
		
	if (isCommand(word))
	{
		printf("ERROR at line %d the label must different to commend",linebug);
		check = False;
		return ;
	}
	else if (isRegister(word))
	{
		printf("ERROR at line %d after label must different register",linebug);
		check = False;
		return ;
	}
	
	strcpy(label->nameLabel,word);
	return ; 

}
/*good*/
Bool isCommand(char * cmd){
	int i;
	for(i = 0; i < NBR_MAX_COMD; i++)
	{
		if (paramCommands[i].namecmd == cmd )
			return True;
	}
return False;
}
/*good*/
Bool isRegister(char * regis)
{
	
	if (regis[0] == 'r'  && regis[1] >= MIN_NBR_REGIS && regis[1] <= MAX_NBR_REGIS && (regis[2] == '\0' ||regis[2] == '\n') )
	{
		return True;
	}
	return False;
}









