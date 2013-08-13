#ifndef _BUFFER_DECODER_H_
#define _BUFFER_DECODER_H_


//add define
#define Decode_Char 0xAD
#define Decode_WChar 0xADAD

//add func
char * BufferAddCoder(char *pBuffer, int size);
wchar_t * WCSBufferAddCoder(wchar_t *pWCSBuffer, int size);
char * BufferDelCoder(char *pBuffer, int size);
wchar_t * WCSBufferDelCoder(wchar_t *pWCSBuffer, int size);


#endif