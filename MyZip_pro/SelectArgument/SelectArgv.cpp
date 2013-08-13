#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SelectArgv.h"
#include "..\\MyDirMain.h"


int GetInputInfo(int argc, char **argv)
{
	int ret = 0;
	char *p = NULL;
	
	switch (argc)
	{
	case 2:
		{
			ret = Case2CheckInfo(argc, argv);
			break;
		}
	default:
		break;
	}
	
	if (ret == 0)
	{
		p = (char *)malloc(sizeof(char) * MAX_NOW_PATH);
		wprintf(Hello_Words);
		wprintf(Argv_List);
		scanf("%s", p);
		ret = CheckInputNumber(p);
		
		p = NULL;
	}
	
	return ret;
}

int CheckInputNumber(char *pInputChar)
{
	int ret = 0;
	
	ret = strlen(pInputChar);
	if (ret == 1)
	{
		switch (pInputChar[0])
		{
		case '1':
			ret = Argv_None;
			break;
		case '2':
			ret = Argv_Zip;
			break;
		case '3':
			ret = Argv_UnZip;
			break;
		case '4':
			ret = Argv_Exit;
			break;
		default:
			ret = Argv_Error;
			break;
		}
	}
	else 
	{
		ret = Argv_Error;
	}
	
	free(pInputChar);
	pInputChar = NULL;
	return ret;
}

int Case2CheckInfo(int argc, char **argv)
{
	int ret = 0;
	
	strupr(argv[1]);
	if (strcmp(argv[1], My_Zip) == 0)
	{
		ret = Argv_Zip;
	}
	else if (strcmp(argv[1], My_Unzip) == 0)
	{
		ret = Argv_UnZip;
	}

	return ret;
}