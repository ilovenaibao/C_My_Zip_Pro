#ifndef _MY_DIR_MAIN_H_
#define _MY_DIR_MAIN_H_



//add define
#define Ret_Error -1
#define Ret_Success 0
#define MAX_NOW_PATH 512
#define Argv_None 0
#define Argv_Zip 1
#define Argv_UnZip 2
#define Argv_Error 3
#define Argv_Exit 4


//add func
void RecallPro(int *argc, char **argv);
wchar_t * GetFullNowPath(wchar_t *pFullPath, int size);  //Get full now path
void ViewAllFIleInNowDir(wchar_t *pFullPath, int choose, 
char *pMySelfName); //View all files
int CheckMySelfFileName(wchar_t *pMyZipFileName, wchar_t *pTmplateName);
void RunMyDefApp(wchar_t *pFullPath, wchar_t *pFileName, int select);
void DeleteMyZipFile();




#endif