#ifndef _MY_ZIP_START_H_
#define _MY_ZIP_START_H_


//add define
#define MAX_NAME_LEN 512
#define Buffer_Len 2048
#define Output_Name (L".\\TaylorZip.tad")
#define Zipfile_Head ("rolyaT_zip>>")
#define Zipfile_Head_2 (0xADDA)

//add struct
typedef struct One_File_Info
{
	wchar_t *pFileName;
	int fileNameLen;
	int fileLen;
} OneFileInfo;


//add func
bool StartMyZip(wchar_t *pFullPath, wchar_t *pTargetName);
bool WriteByteMyZip2Func(char *pBuffer, int size, FILE *pTargetFile);
bool WriteWcharMyZip2Func(wchar_t *pBuffer, int size, 
FILE *pTargetFile);
wchar_t * Absolute2IndirectPath(wchar_t *pFullPath);
void WriteOneFileInfoStrcut(FILE *pTargetFile, int len);
int StartMyUnzip();
bool ReadMyZipFile(wchar_t *pMyZipFileName);
int SplitOneFile(FILE *pOraginFile, int startPos);
bool MakeOneFile(wchar_t *pFileName, int startPos, int len,
FILE *pOraginFile);
int Read4Byte(FILE *pOraginFile);



#endif