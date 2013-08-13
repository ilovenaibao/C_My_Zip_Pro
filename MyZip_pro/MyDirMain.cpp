#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>
#include <memory.h>
#include <string.h>

using namespace std;

#include "MyDirMain.h"
#include ".\\SelectArgument\\SelectArgv.h"
#include ".\\MyZipTools\\MyZipFunc.h"

long g_totalCount = 0;

/*
* Name         : main
*
* Synopsis     : int main(int argc, char **argv)
*
* Arguments    : int  argc : �ѼƭӼ�
*				  char **argv : �Ѽ�
*
* Description  : 1. ""�ȹM������󧨡A�}��ܫK�Q�����
*				  2. "-zip"�N����󧨶i�����Y
*				  3. "-unzip"�����Y
*				  3. "*"...
* 
* Returns      : int 
*/
int main(int argc, char **argv)
{
	int ret = Ret_Error;
	wchar_t *pFullPath = NULL;
	int selectArgv = 0;

Recall:
	//Initialize data
	selectArgv = GetInputInfo(argc, argv);
	if (selectArgv == Argv_Exit)
	{
		return ret;
	}
	else if (selectArgv == Argv_UnZip)
	{
		ret = StartMyUnzip();
	}
	else if (selectArgv == Argv_None || selectArgv == Argv_Zip)
	{
		if (selectArgv == Argv_Zip)
		{
			DeleteMyZipFile();
		}
		
		//��o��e������|
		pFullPath = GetFullNowPath(pFullPath, MAX_NOW_PATH);
		//�M�������|
		ViewAllFIleInNowDir(pFullPath, selectArgv, argv[0]);
		
		//�o�X�M�����G
		wprintf(L"\ntotal count = %ld\n", g_totalCount);
		
		free(pFullPath);
		pFullPath = NULL;
	}
	else if (selectArgv == Argv_Error)
	{
		wprintf(L"\nInput is error!\n\n");
	}

	RecallPro(&argc, argv);
	goto Recall;
	
	return ret;
}

void RecallPro(int *argc, char **argv)
{
	int count = 0;
	int bufferLen = 0;
	
	for (count = 1; count < *argc; count++)
	{
		bufferLen = strlen(argv[count]);
		memset(argv[count], '\0', bufferLen);
	}
	*argc = 1;
}

/*
* Name         : GetFullNowPath
*
* Synopsis     : wchar_t GetFullNowPath(wchar_t *pFullPath, int size)
*
* Arguments    : wchar_t  *pFullPath : 
*                int  size : 
*
* Description  : �����e��������|
* 
* Returns      : ������|
*/
wchar_t * GetFullNowPath(wchar_t *pFullPath, int size)
{
	pFullPath = (wchar_t *)malloc(sizeof(wchar_t) * size);
	memset(pFullPath, L'\0', size);
	_wgetcwd(pFullPath, size);
	
	return pFullPath;
}

/*
* Name         : ViewAllFIleInNowDir
*
* Synopsis     : void ViewAllFIleInNowDir(wchar_t *pFullPath, int choose)
*
* Arguments    : wchar_t  *pFullPath : 
*                int  choose : 
*
* Description  : �M�������|�U���Ҧ����
* 
*/
void ViewAllFIleInNowDir(wchar_t *pFullPath, int choose, 
char *pMySelfName)
{
	long hFile;  
	_wfinddata_t findInfo;
	wchar_t *pTmpPath = NULL;
	wchar_t *pWcMySelfName = NULL;
	wchar_t *pTmpMySelfZipName = NULL;
	int mySelfNameLen = 0;
	
	pTmpMySelfZipName = (wchar_t *)malloc(sizeof(wchar_t) 
	* (wcslen(Output_Name) + 1));
	wcscpy(pTmpMySelfZipName, Output_Name);
	pTmpMySelfZipName[wcslen(Output_Name)] = L'\0';
	
	mySelfNameLen = strlen(pMySelfName);
	pWcMySelfName = (wchar_t *)malloc(sizeof(wchar_t) 
	* (mySelfNameLen + 1));	
	mbstowcs(pWcMySelfName, pMySelfName, mySelfNameLen);
	pWcMySelfName[mySelfNameLen] = L'\0';
	
	mySelfNameLen = wcslen(pFullPath) - 1;
	if (pFullPath[mySelfNameLen] == L'\\')
	{
		wcscat(pFullPath, L"*");
	}
	else
	{
		wcscat(pFullPath, L"\\*");
	}
	
	hFile = _wfindfirst(pFullPath, &findInfo);
	pTmpPath = wcsrchr(pFullPath, L'*');
	if (pTmpPath != NULL)
	{
		memset(pTmpPath, L'\0', wcslen(pTmpPath));
	}
	
	if (hFile != -1)
	{
		if (
				!(findInfo.attrib & _A_SUBDIR)
				&& CheckMySelfFileName(findInfo.name, 
					pWcMySelfName) != 0
				&& CheckMySelfFileName(findInfo.name, 
					pTmpMySelfZipName) != 0
				)
		{
			if (wcscmp(pWcMySelfName, findInfo.name) != 0)
			{
				pTmpPath = wcschr(pFullPath, L'*');
				if (pTmpPath != NULL)
				{
					pTmpPath[0] = L'\0';
				}
				// wprintf(L"%s", pFullPath);
				// wprintf(L"%s\n", findInfo.name);
				RunMyDefApp(pFullPath, findInfo.name, choose);
				g_totalCount += 1;
			}
		}
		else
		{
			goto Goto_1;
		}
		
		while (_wfindnext(hFile, &findInfo) == 0)
		{
			if (!(findInfo.attrib & _A_SUBDIR))
			{
				if (
						CheckMySelfFileName(findInfo.name, 
							pWcMySelfName) != 0
						&& CheckMySelfFileName(findInfo.name, 
							pTmpMySelfZipName) != 0
						)
				{
					pTmpPath = wcschr(pFullPath, L'*');
					if (pTmpPath != NULL)
					{
						pTmpPath[0] = L'\0';
					}
					// wprintf(L"%s", pFullPath);
					// wprintf(L"%s\n", findInfo.name);
					RunMyDefApp(pFullPath, findInfo.name, choose);
					g_totalCount += 1;
				}
			}
			else
			{
				Goto_1:
				if (wcscmp(findInfo.name, L".") != 0
						&& wcscmp(findInfo.name, L"..") != 0)
				{
					wcscat(pFullPath, findInfo.name);
					ViewAllFIleInNowDir(pFullPath, choose, pMySelfName);
				}
			}

		}
		
		pTmpPath = wcsrchr(pFullPath, L'\\');
		pTmpPath[0] = L'0';
		pTmpPath = wcsrchr(pFullPath, L'\\');
		memset(pTmpPath, L'\0', wcslen(pTmpPath));
		pTmpPath[0] = L'\\';
		
		free(pTmpMySelfZipName);
		pTmpMySelfZipName = NULL;
		free(pWcMySelfName);
		pWcMySelfName = NULL;
		
		_findclose(hFile);
	}
	
}

int CheckMySelfFileName(wchar_t *pMyZipFileName, wchar_t *pTmplateName)
{
	int ret = 1;
	wchar_t *pMyZipName = NULL;
	wchar_t *pTmpMyZipName = NULL;
	
	pMyZipName = wcsrchr(pTmplateName, L'\\') + 1;
	pTmpMyZipName =  wcsrchr(pMyZipFileName, L'\\');
	if (pTmpMyZipName == NULL)
	{
		if (pMyZipFileName != NULL)
		{
			ret = wcscmp(pMyZipFileName, pMyZipName); 
		}
	}
	else
	{
		ret = wcscmp(pTmpMyZipName, pMyZipName);
	}
	
	return ret;
}

void RunMyDefApp(wchar_t *pFullPath, wchar_t *pFileName, int select)
{
	switch (select)
	{
	case Argv_None:
		wprintf(L"%s%s\n", 
		pFullPath, pFileName);
		break;
	case Argv_Zip:
		StartMyZip(pFullPath, pFileName);
		break;
	default:
		break;
	}
}

void DeleteMyZipFile()
{
	// FILE *pMyZipFile = NULL;
	
	// pMyZipFile = _wfopen(Output_Name, L"wb+");
	// if (pMyZipFile != NULL)
	// {
	// fwrite("", 0, 1, pMyZipFile);
	
	// fclose(pMyZipFile);
	// pMyZipFile = NULL;
	// }
	
	_wremove(Output_Name);
}