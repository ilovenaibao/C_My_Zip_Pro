#ifndef _SELECT_ARGV_H_
#define _SELECT_ARGV_H_


//add define
#define My_Zip ("ZIP")
#define My_Unzip ("UNZIP")
#define Hello_Words (L" -----------------------------\n\
|    Taylor Zip file tools    |\n\
 -----------------------------\n")
#define Argv_List (L"Please input number :\n1. Show all files in this \
direct\n2. Zip this directory\n3. UnZip one file\n4. Exit\n")



//add func
int GetInputInfo(int argc, char **argv);
int CheckInputNumber(char *pInputChar);
int Case2CheckInfo(int argc, char **argv);



#endif