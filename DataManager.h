#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__

#include <unistd.h>
#include <stdio.h>

#define INVA -1
#define IMEDIATELY 0
#define DIRECT 1
#define INDEX 2
#define OGERDIRECT 3

#define MAXLAB 30
typedef enum {False , True} Bool;
typedef struct{ 						/* We need a 15 integers for the word, we can use the struct since minimum size for int is 16 */
	
	unsigned int era : 2; 				/* First two are reserved for E.R.A bits*/
	
	union							/* Union struct since the word can be a command or data*/
	{
		int dataB			: 13;		

		struct{
			unsigned int dstOp	: 2;		/* Destination Operand Address Method ID */
			unsigned int srcOp	: 2;		/* Source Operand Address Method ID */
			unsigned int opcode	: 4;		/* Command ID */
			unsigned int group	: 2;		/* Number of operand*/
			unsigned int unused	: 3;		/* Unused Bit, it is always 111 */
		} commandB;
		
		struct{						/* Operation between two registers requires only one word*/
			unsigned int dstReg	: 6; 		/* Destination register Number*/
			unsigned int srcReg	: 6; 		/* Source register Number*/
			unsigned int unused : 1;		/* Unused Bit, it is always 0 */
		} operandB;


	}convertB;
	
} Binarycode; 

typedef struct{
	int opType;
	int adr;
	char name [MAXLAB];
	int number;
}op;
typedef struct{
	char nameLabel [MAXLAB];
	int adrOfInfo;
}infoLabel;
typedef struct{
	char nameLabel [MAXLAB];
	int cmd;
	int nbrop;
	op *op1;
	op *op2;
	int linebug;
}Info;
typedef struct {
	char nameLabel[MAXLAB];
	int adress;
	Bool isData;
	Bool isextern;
	int adressData;
}Label;


typedef struct {
	char  nameData[MAXLAB];
	int adress;
	int ta_data;
}Data;


typedef struct{
	char nameLabel[MAXLAB];
	int linebug;
}Entry;	


#endif
