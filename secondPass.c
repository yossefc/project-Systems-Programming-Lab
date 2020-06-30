#include <string.h>
#include "secondPass.h"

void addOpDirect(Info * , Label * ,int *,int,infoLabel * ,Binarycode *);
void addOpOguer(Info * , Label * ,int *,int ,infoLabel * ,Binarycode *);
void addOpIndex(Info * , Label * ,int *,int ,infoLabel * ,Binarycode *);
void addOpImediately(Info * , Label * ,int *,int ,infoLabel *,Binarycode * );
int checkLabel (Label * ,char * ,int );
void addData(Data * ,int * ,Binarycode * ,Label *, int *  );
void reparInfo(infoLabel * ,Binarycode * ,Label *);
Bool checkSecond = True;
int nbrInfLab = 1;

void secondPass(Label * arrayLabel,Data *arrayData, Info * arrayInfo, Binarycode * arraycode, int * _dc, int *IC,infoLabel * arrayInfoLab)
{
	int i = 0;
	int adress = 0;
	int adressLabel = *_dc;
	
	char  nameLab[MAXLAB];

	while(arrayLabel[i].nameLabel != NULL)
	{	
		/* enter the adress true*/
		strcpy(nameLab , arrayLabel[i].nameLabel);/*copy the name of label in nameLab*/
		
	   if(!arrayLabel[i].isData && !arrayLabel[i].isextern) /* label cmd*/
	   {
		while (strcmp(nameLab,arrayInfo[adress].nameLabel))/*jusqua que le nomlabel n'est plus le mm*/
		{	
			/*entry de number binary of the command*/
			arraycode[*_dc].era = A;
			if(arrayInfo[adress].nbrop == 1)
			{
				arraycode[*_dc].convertB.commandB.dstOp =arrayInfo[adress].op1->opType;
				arraycode[*_dc].convertB.commandB.srcOp = 0;	
			}
		
			else if ((arrayInfo[adress].nbrop == 2))
			{
				arraycode[*_dc].convertB.commandB.dstOp =arrayInfo[adress].op1->opType;
				arraycode[*_dc].convertB.commandB.srcOp = arrayInfo[adress].op2->opType;	
			}	
			else
			{
				arraycode[*_dc].convertB.commandB.dstOp =0;
				arraycode[*_dc].convertB.commandB.srcOp = 0;
			}		
				arraycode[*_dc].convertB.commandB.opcode = arrayInfo[adress].cmd;
				arraycode[*_dc].convertB.commandB.group = arrayInfo[adress].nbrop;
				arraycode[*_dc].convertB.commandB.unused = FIX;
				(*_dc)++;
				addOpDirect(arrayInfo, arrayLabel,_dc,adress, arrayInfoLab,arraycode);/*check operand is DIRECT*/
				addOpOguer(arrayInfo, arrayLabel,_dc,adress, arrayInfoLab,arraycode);/*check operand is OGUERDIREct*/				
				addOpIndex(arrayInfo, arrayLabel,_dc,adress, arrayInfoLab,arraycode);
				addOpImediately(arrayInfo, arrayLabel,_dc,adress, arrayInfoLab,arraycode);
				if(arrayInfo[adress].nbrop == 1)
					(*_dc)++;
			adress++;
		}
		arrayLabel[i].adress = adressLabel;
		adressLabel = *_dc;		
	    }
	    else if (arrayLabel[i].isextern)
	    {
		arraycode[*_dc].era = E;
		arraycode[*_dc].convertB.dataB = 0;
		(*_dc)++;
		
	    }
		
		i++;
	}
	addData(arrayData, _dc, arraycode, arrayLabel,IC);
	reparInfo(arrayInfoLab,arraycode,arrayLabel);
}
/*corrige the adress of the label in the place*/
void reparInfo(infoLabel *arrayInfoLabel,Binarycode * arraycode,Label *arrayLabel)
{
	int i = 0;
	int adressOfLabel = 0 ;
	while (arrayInfoLabel[i].nameLabel  != NULL)
	{
		if (arraycode[arrayInfoLabel[i].adrOfInfo].era == 'E')
		{
			adressOfLabel = checkLabel ( arrayLabel, arrayInfoLabel[i].nameLabel,adressOfLabel);
			arraycode[arrayInfoLabel[i].adrOfInfo].convertB.dataB = arrayLabel[adressOfLabel].adress;
		}
		i++;
	}
}
void addData(Data *arrayData,int * _dc,Binarycode * arraycode,Label * arrayLabel,int *IC)
{
	int adressLab = 0;
	char nameLab [MAXLAB];
	strcpy(nameLab ,arrayData[*IC].nameData);
	adressLab = checkLabel(arrayLabel,nameLab,adressLab);
	arrayLabel[adressLab].adress= *_dc;
	while(arrayData[*IC].nameData != NULL)
	{
		if (!strcmp(arrayData[*IC].nameData, nameLab))
		{
			strcpy(nameLab ,arrayData[*IC].nameData);
			adressLab =checkLabel (arrayLabel,nameLab,adressLab);
			arrayLabel[adressLab].adress= *_dc;
		}
		arraycode[*_dc].convertB.dataB = arrayData[*IC].ta_data;
		(*_dc)++;
		(*IC)++;
	}
}
/*
	the function return the adressof the label
*/
int checkLabel (Label * arrayLabel,char * nameLab,int adress)
{
	while (True)
	{
		if (strcmp(arrayLabel[adress].nameLabel,nameLab))
			return adress;
		adress++;
	}
	return -1;
}
/*add the operand of number*/
void addOpImediately(Info * arrayInfo, Label * arrayLabel,int *_dc,int adress,infoLabel * arrayInfoLab,Binarycode * arraycode)
{
	if (arrayInfo[adress].op1->opType == IMEDIATELY)
	{
		arraycode[*_dc].era = A;
		arraycode[*_dc].convertB.dataB = arrayInfo[adress].op1->number;
	}
	if (arrayInfo[adress].op2->opType == IMEDIATELY)
	{
		arraycode[*_dc+1].era = A;
		arraycode[*_dc+1].convertB.dataB = arrayInfo[adress].op2->number;
		(*_dc)++;
		(*_dc)++;
	}
}

/* add to bunary the operand is r5[r2]*/
void addOpIndex(Info * arrayInfo, Label * arrayLabel,int *_dc,int adress,infoLabel * arrayInfoLab,Binarycode * arraycode)
{
	if (arrayInfo[adress].op1->opType == INDEX)
	{	
		arraycode[*_dc].era = E;
		arraycode[*_dc].convertB.operandB.dstReg = arrayInfo[adress].op1->name[2];
		arraycode[*_dc].convertB.operandB.srcReg = arrayInfo[adress].op1->name[1];
	}
	if (arrayInfo[adress].op2->opType == INDEX)
	{
		arraycode[*_dc+1].era = E;
		arraycode[*_dc+1].convertB.operandB.dstReg = arrayInfo[adress].op2->name[2];
		arraycode[*_dc+1].convertB.operandB.srcReg = arrayInfo[adress].op2->name[1];
		(*_dc)++;
		(*_dc)++;
	}
}

void addOpOguer(Info * arrayInfo, Label * arrayLabel,int *_dc,int adress,infoLabel * arrayInfoLab,Binarycode * arraycode)
{
	if (arrayInfo[adress].op1->opType == OGERDIRECT && arrayInfo[adress].op2->opType == OGERDIRECT)
	{
		arraycode[*_dc].era = E;
		arraycode[*_dc].convertB.operandB.dstReg = arrayInfo[adress].op1->name[1];
		arraycode[*_dc].convertB.operandB.srcReg = arrayInfo[adress].op2->name[2];
		(*_dc)++;
	}
	if (arrayInfo[adress].op1->opType == OGERDIRECT )
	{
		arraycode[*_dc].era = E;
		arraycode[*_dc].convertB.operandB.dstReg = arrayInfo[adress].op1->name[1];
	}
	if(arrayInfo[adress].op2->opType == OGERDIRECT)
	{
		arraycode[*_dc].era = E;
		arraycode[*_dc].convertB.operandB.srcReg = arrayInfo[adress].op2->name[1];
		(*_dc)++;
		(*_dc)++;
	}
	
}
void addOpDirect(Info * arrayInfo, Label * arrayLabel,int *_dc,int adress,infoLabel * arrayInfoLab,Binarycode * arraycode)
{
	int count;
	if (arrayInfo[adress].op1->opType == DIRECT )/*check if the operand is number*/
	{	
		count = checkLabel(arrayLabel,arrayInfo[adress].op1->name,0);
		if(count == -1)
		{
			printf("ERROR to line %d the operator is not in the memory",arrayInfo[adress].linebug);
			checkSecond = False;	
			return;			
		}
		else
		{
			strcpy(arrayInfoLab[nbrInfLab].nameLabel , arrayInfo[adress].op1->name);
			arrayInfoLab[nbrInfLab].adrOfInfo = *_dc + 1;
			nbrInfLab++;
			arrayInfoLab = realloc (arrayInfoLab,nbrInfLab*sizeof(arrayInfoLab));
			*if (!arrayInfoLab)
			{
				printf("ERROR the malloc not work");
				return;
			}
			if (arrayLabel[count].isData)
			{
				arraycode[*_dc].era =R;
				arraycode[*_dc].convertB.dataB = 0;
			}
			else
			{
				arraycode[*_dc].era = E;
				arraycode[*_dc].convertB.dataB = 0;
			}
		}
	}
	if (arrayInfo[adress].op2->opType == DIRECT )/*check if the operand is number*/
	{	
		count = checkLabel(arrayLabel,arrayInfo[adress].op2->name,0);
		if(adress == -1)
		{
			printf("ERROR to line %d the operator is not in the memory",arrayInfo[adress].linebug);
			checkSecond = False;	
			return;			
		}
		else
		{
			strcpy(arrayInfoLab[nbrInfLab].nameLabel , arrayInfo[adress].op2->name);
			arrayInfoLab[nbrInfLab].adrOfInfo = *_dc + 2;
			nbrInfLab++;
			arrayInfoLab = realloc (arrayInfoLab,nbrInfLab*sizeof(arrayInfoLab));
			if (!arrayInfoLab)
			{
					printf("ERROR the malloc not work");
					return;
			}
			if (arrayLabel[count].isData)
			{
				arraycode[*_dc].era =R;
				arraycode[*_dc].convertB.dataB = 0;
			}
			else
			{
				arraycode[*_dc].era = E;
				arraycode[*_dc].convertB.dataB = 0;
			}
		}
	}
}
