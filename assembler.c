#include <stdio.h>
#include <string.h>
#include "Analyzer.h"
#include "DataManager.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
	int i;	
	if (argc < MIN_ARGS)
	{ 
		printf("ERROR there are less file\n");
		return 1;
	}
	
	if (!strcmp(argv[0], NAMEPROJECT))
	{
		printf("the name of project is not correct \n");
		return 1;
	}	

	for (i = 1; i < argc; i++)
	{		
		runproject(argv[i]);
	}

	return 0;
}
	
