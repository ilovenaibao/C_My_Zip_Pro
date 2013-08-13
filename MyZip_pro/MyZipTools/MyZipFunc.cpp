#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>

#include "MyZipFunc.h"
#include "..\\MyDirMain.h"
#include "..\\BufferDecoder\\BufferDecoder.h"


bool StartMyZip(wchar_t *pFullPath, wchar_t *pTargetName)
{
	bool bRet = false;
	wchar_t *pTmpTargetName = NULL;
	FILE *pOraginFile = NULL;
	FILE *pTargetFile = NULL;
	char *pBuffer = NULL;
	int oraginFileLen = 0;
	int oraginFilePos = 0;
	int targetFileLen = 0;
	wchar_t ZipFileCut = (wchar_t)Zipfile_Head_2;
	// wchar_t OneFileCut = (wchar_t)Onefile_Cut;
	OneFileInfo oneFile;
	
	pTmpTargetName = (wchar_t *)malloc(sizeof(wchar_t) * MAX_NAME_LEN);
	wcscpy(pTmpTargetName, pFullPath);
	wcscat(pTmpTargetName, pTargetName);
	pTmpTargetName = Absolute2IndirectPath(pTmpTargetName);
	oneFile.pFileName = pTmpTargetName;
	oneFile.fileNameLen = wcslen(pTmpTargetName);
	
	wprintf(L"Zip--->%s\n", pTmpTargetName);
	pOraginFile = _wfopen(pTmpTargetName, L"rb");
	if (pOraginFile == NULL)
	{
		wprintf(L"error: %s cannot open!\n", pTmpTargetName);
		return bRet;
	}
	else
	{
		pTargetFile = _wfopen(Output_Name, L"ab+");
		if (pTargetFile == NULL)
		{
			wprintf(L"error: %s cannot open\n", Output_Name);
			return bRet;
		}
		pBuffer = (char *)malloc(sizeof(char) * Buffer_Len);
		
		//Initialize OraginFile
		fseek(pOraginFile, 0, SEEK_END);
		oraginFileLen = ftell(pOraginFile);
		fseek(pOraginFile, 0, SEEK_SET);
		oraginFilePos = 0;
		oneFile.fileLen = oraginFileLen;
		//Initialize TargetFile
		fseek(pTargetFile, 0, SEEK_END);
		targetFileLen = ftell(pTargetFile);
		if (targetFileLen == 0)
		{
			fwrite(Zipfile_Head, strlen(Zipfile_Head), 1, pTargetFile);
			fwrite(&ZipFileCut, sizeof(wchar_t), 
			1, pTargetFile);
		}
		
		WriteOneFileInfoStrcut(pTargetFile, oneFile.fileNameLen);
		WriteOneFileInfoStrcut(pTargetFile, oneFile.fileLen);
		oneFile.pFileName = WCSBufferAddCoder(oneFile.pFileName,
		oneFile.fileNameLen);
		fwrite(oneFile.pFileName, oneFile.fileNameLen * 2, 1, pTargetFile);
		
		while (oraginFilePos < oraginFileLen)
		{
			if (Buffer_Len <= (oraginFileLen - oraginFilePos))
			{
				fread(pBuffer, Buffer_Len, 1, pOraginFile);
				pBuffer = BufferAddCoder(pBuffer, Buffer_Len);
				fwrite(pBuffer, Buffer_Len, 1, pTargetFile);
			}
			else
			{
				fread(pBuffer, oraginFileLen - oraginFilePos, 
				1, pOraginFile);
				pBuffer = BufferAddCoder(pBuffer, 
				oraginFileLen - oraginFilePos);
				fwrite(pBuffer, oraginFileLen - oraginFilePos, 
				1, pTargetFile);
			}
			memset(pBuffer, '\0', Buffer_Len);
			oraginFilePos = ftell(pOraginFile);
		}

		fclose(pTargetFile);
		pTargetFile = NULL;
		fclose(pOraginFile);
		pOraginFile = NULL;
		free(pBuffer);
		pBuffer = NULL;
	}

	free(pTmpTargetName);
	pTmpTargetName = NULL;
	
	return bRet;
}

bool WriteByteMyZip2Func(char *pBuffer, int size, FILE *pTargetFile)
{
	bool bRet = false;
	char tmpWchar = '\0';
	char tmpWcharOneBit[8] = {
		0x80, 0x40, 0x20, 0x10,
		0x08, 0x04, 0x02, 0x01
	};
	int preResult = 0;
	int tmpResult = 0;
	int oneBufferCount = 0;
	int oneBufferLen = 8;
	int collectCount = 0;
	int bufferCount = 0;
	
	// printf("pBuffer = %s\n", pBuffer);
	// printf("size = %d\n", size);
	
	for ( ; bufferCount < size; bufferCount++)
	{
		tmpWchar = pBuffer[bufferCount];
		for (oneBufferCount = 0; 
		oneBufferCount < oneBufferLen; oneBufferCount++)
		{
			tmpResult = tmpWchar & tmpWcharOneBit[oneBufferCount];
			if (tmpResult != 0)
			{
				tmpResult = 1;
			}
			
			if (preResult != tmpResult)
			{
				// printf("Result = %d[%d]\n", preResult, collectCount);
				// getch();
				fwrite(&preResult, 1, 1, pTargetFile);
				fwrite(&collectCount, 4, 1, pTargetFile);
				
				preResult = tmpResult;
				collectCount = 1;
			}
			else 
			{
				collectCount ++;
			}
		}
	}
	preResult = tmpResult;
	// printf("Result = %d[%d]\n", preResult, collectCount);

	fwrite(&preResult, 1, 1, pTargetFile);
	fwrite(&collectCount, 4, 1, pTargetFile);
	// printf("pBuffer end\n");
	// getch();
	return bRet;
}

bool WriteWcharMyZip2Func(wchar_t *pBuffer, int size, FILE *pTargetFile)
{
	bool bRet = false;
	wchar_t tmpWchar = L'\0';
	wchar_t tmpWcharOneBit[16] = {
		0x8000, 0x4000, 0x2000, 0x1000,
		0x0800, 0x0400, 0x0200, 0x0100,
		0x0080, 0x0040, 0x0020, 0x0010,
		0x0008, 0x0004, 0x0002, 0x0001
	};
	int preResult = 0;
	int tmpResult = 0;
	int oneBufferCount = 0;
	int oneBufferLen = 16;
	int collectCount = 0;
	int bufferCount = 0;
	
	wprintf(L"pBuffer = %s\n", pBuffer);
	wprintf(L"size = %d\n", size);
	
	for ( ; bufferCount < size; bufferCount++)
	{
		tmpWchar = pBuffer[bufferCount];
		// wprintf(L"tmpWchar = %c\n", tmpWchar);
		for (oneBufferCount = 0; 
		oneBufferCount < oneBufferLen; oneBufferCount++)
		{
			tmpResult = tmpWchar & tmpWcharOneBit[oneBufferCount];
			if (tmpResult != 0)
			{
				tmpResult = 1;
			}
			
			if (preResult != tmpResult)
			{
				// printf("Result = %d[%d]\n", preResult, collectCount);
				// getch();
				fwrite(&preResult, 1, 1, pTargetFile);
				fwrite(&collectCount, 4, 1, pTargetFile);
				
				preResult = tmpResult;
				collectCount = 1;
			}
			else 
			{
				collectCount ++;
			}
		}
		// preResult = tmpResult;
		// printf("Result = %d[%d]\n", preResult, collectCount);
		// collectCount = 0;
		// printf("one buffer\n");
		
	}
	preResult = tmpResult;
	// printf("Result = %d[%d]\n", preResult, collectCount);
	fwrite(&preResult, 1, 1, pTargetFile);
	fwrite(&collectCount, 4, 1, pTargetFile);
	// wprintf(L"pBuffer end\n");
	getch();
	return bRet;
}

wchar_t * Absolute2IndirectPath(wchar_t *pFullPath)
{
	wchar_t *pTmpNowFullPath = NULL;
	int bufferLen = 0;
	wchar_t *pIndirectPath = NULL;

	pTmpNowFullPath = GetFullNowPath(pTmpNowFullPath, MAX_NOW_PATH);	
	bufferLen = wcslen(pTmpNowFullPath);
	pIndirectPath = pFullPath + bufferLen + 1;
	
	free(pTmpNowFullPath);
	pTmpNowFullPath = NULL;
	
	return pIndirectPath;
}

void WriteOneFileInfoStrcut(FILE *pTargetFile, int len)
{
	int count = 0;
	unsigned char oneByte = 1;
	
	for ( ; count < 4; count++)
	{
		oneByte = len;
		fwrite(&oneByte, sizeof(unsigned char), 1, pTargetFile);
		len >>= 8;
	}
}

int StartMyUnzip()
{
	int Ret = Ret_Error;
	wchar_t *pUnzipFileName = NULL;
	
	pUnzipFileName = (wchar_t *)malloc(sizeof(wchar_t) * MAX_NAME_LEN);
	wprintf(L"Input Unzip file name:");
	wscanf(L"%s", pUnzipFileName);
	ReadMyZipFile(pUnzipFileName);
	
	free(pUnzipFileName);
	pUnzipFileName = NULL;
	
	return Ret;
}

bool ReadMyZipFile(wchar_t *pMyZipFileName)
{
	bool bRet = false;
	FILE *pMyZipFile = NULL;
	int myZipFileLen = 0;
	int myZipFilePos = 0;
	char *pMyZipFileHead = NULL;
	int bufferLen = 0;
	char *pReadBuffer = NULL;
	char zipFileHeadCut[2] = {0xDA, 0xAD};
	int fileCount = 0;
	
	pMyZipFile = _wfopen(pMyZipFileName, L"r+b");
	if (pMyZipFile == NULL)
	{
		wprintf(L"error: Can not find %s file.\n", pMyZipFileName);
		return bRet;
	}
	else
	{
		pReadBuffer = (char *)malloc(Buffer_Len);
		memset(pReadBuffer, '\0', Buffer_Len);
		bufferLen = sizeof(Zipfile_Head) + sizeof(zipFileHeadCut);
		pMyZipFileHead = (char *)malloc(bufferLen + 2);
		memset(pMyZipFileHead, '\0', bufferLen + 2);
		
		strcpy(pMyZipFileHead, Zipfile_Head);
		strcat(pMyZipFileHead, zipFileHeadCut);
		pMyZipFileHead[bufferLen - 1] = '\0';
		
		fseek(pMyZipFile, 0, SEEK_END);
		myZipFileLen = ftell(pMyZipFile);
		fseek(pMyZipFile, 0, SEEK_SET);
		
		fread(pReadBuffer, bufferLen - 1, 1, pMyZipFile);
		pReadBuffer[bufferLen - 1] = '\0';
		if (strcmp(pReadBuffer, pMyZipFileHead) != 0)
		{
			wprintf(L"error: %s is not my zip file.\n", pMyZipFileName);
		}
		else
		{
			memset(pReadBuffer, '\0', Buffer_Len);
			myZipFilePos = ftell(pMyZipFile);
			while (myZipFilePos < myZipFileLen)
			{
				myZipFilePos = SplitOneFile(pMyZipFile, myZipFilePos);
				if (myZipFilePos == -1)
				{
					bRet = false;
					break;
				}
				fileCount++;
				bRet = true;
			}
			fileCount--;
			if (fileCount >= 0 && bRet)
			{
				wprintf(L"Zip file count = %d\n", fileCount);
			}
		}

		free(pReadBuffer);
		pReadBuffer = NULL;
		free(pMyZipFileHead);
		pMyZipFileHead = NULL;
		fclose(pMyZipFile);
		pMyZipFile = NULL;
	}
	
	return bRet;
}

int SplitOneFile(FILE *pOraginFile, int startPos)
{
	int ret = 0;
	OneFileInfo fileInfo;
	char *pBuffer = NULL;
	int bufferLen = 0;
	wchar_t *pWcharBuffer = NULL;
	
	fseek(pOraginFile, startPos, SEEK_SET);
	bufferLen = sizeof(char) * Buffer_Len;
	pBuffer = (char *)malloc(bufferLen);
	memset(pBuffer, '\0', bufferLen);
	bufferLen = sizeof(wchar_t) * Buffer_Len;
	pWcharBuffer = (wchar_t *)malloc(bufferLen);
	memset(pWcharBuffer, L'\0', bufferLen);
	
	fileInfo.fileNameLen = Read4Byte(pOraginFile);
	fileInfo.fileLen = Read4Byte(pOraginFile);

	bufferLen = fileInfo.fileNameLen * 2;
	fread(pWcharBuffer, bufferLen , 1, pOraginFile);
	pWcharBuffer[bufferLen] = '\0';
	pWcharBuffer = WCSBufferDelCoder(pWcharBuffer, 
	fileInfo.fileNameLen);
	fileInfo.pFileName = pWcharBuffer;
	
	startPos = startPos + 8 + fileInfo.fileNameLen * 2;
	wprintf(L"%s\n", fileInfo.pFileName);
	if (MakeOneFile(fileInfo.pFileName, startPos, fileInfo.fileLen,
				pOraginFile))
	{
		ret = startPos + fileInfo.fileLen;
	}
	else 
	{
		ret = -1;
	}
	
	free(pBuffer);
	pBuffer = NULL;
	free(pWcharBuffer);
	pWcharBuffer = NULL;
	
	
	return ret;
}

bool MakeOneFile(wchar_t *pFileName, int startPos, int len,
FILE *pOraginFile)
{
	bool bRet = false;
	FILE *pOneFile = NULL;
	int count = 0;
	int nameLen = 0;
	wchar_t *pTmpNameStr = NULL;
	char *pCharName = NULL;
	int mkdirFlag = 0;
	
	pTmpNameStr = (wchar_t *)malloc(sizeof(wchar_t) * MAX_NOW_PATH);
	memset(pTmpNameStr, L'\0', MAX_NOW_PATH);
	pCharName = (char *)malloc(sizeof(char) * 2 * MAX_NOW_PATH);
	memset(pCharName, '\0', 2 * MAX_NOW_PATH);
	
	nameLen = wcslen(pFileName);
	
	//make dir
	for ( ; count < nameLen; count++)
	{
		pTmpNameStr[count] = pFileName[count];
		if (pFileName[count] == L'\\')
		{
			pTmpNameStr[count] = L'\0';
			wcstombs(pCharName, pTmpNameStr, count);
			mkdirFlag = _mkdir(pCharName);
			
			pTmpNameStr[count] = pFileName[count];
			memset(pCharName, '\0', 2 * MAX_NOW_PATH);
		}
	}
	free(pTmpNameStr);
	pTmpNameStr = NULL;
	free(pCharName);
	pCharName = NULL;
	//make one file 
	pOneFile = _wfopen(pFileName, L"wb+");
	if (pOneFile == NULL)
	{
		wprintf(L"error: cannot create %s file\n", pFileName);
	}
	else 
	{
		pCharName = (char *)malloc(sizeof(char) * Buffer_Len);
		memset(pCharName, '\0', Buffer_Len);
		fseek(pOraginFile, startPos, SEEK_SET);
		
		count = 0;
		nameLen = len - count;
		while (count < len)
		{
			if (nameLen < Buffer_Len)
			{
				fread(pCharName, nameLen, 1, pOraginFile);
				pCharName = BufferDelCoder(pCharName, nameLen);
				fwrite(pCharName, nameLen, 1, pOneFile);
				break;
			}
			else 
			{
				fread(pCharName, Buffer_Len, 1, pOraginFile);
				pCharName = BufferDelCoder(pCharName, Buffer_Len);
				fwrite(pCharName, Buffer_Len, 1, pOneFile);
				memset(pCharName, '\0', Buffer_Len);
				nameLen -= Buffer_Len;
			}
		}
		
		fclose(pOneFile);
		pOneFile = NULL;
		bRet = true;
	}
	
	return bRet;
}

int Read4Byte(FILE *pOraginFile)
{
	int ret = 0;
	int count = 0;
	unsigned char my4Byte[4];
	
	fread(my4Byte, sizeof(unsigned char) * 4, 1, pOraginFile);
	for (count = 3; count >= 0; count--)
	{
		ret += my4Byte[count];
		if (count > 0)
		{
			ret <<= 8;
		}
	}

	return ret;
}