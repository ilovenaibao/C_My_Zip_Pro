#include <stdio.h>
#include "BufferDecoder.h"

char * BufferAddCoder(char *pBuffer, int size)
{
	int bufferLen = 0;
	int bufferCount = 0;
	
	bufferLen = size;
	for ( ; bufferCount < bufferLen; bufferCount++)
	{
		pBuffer[bufferCount] += (char)Decode_Char;
	}

	return pBuffer;
}

wchar_t * WCSBufferAddCoder(wchar_t *pWCSBuffer, int size)
{
	int bufferLen = 0;
	int bufferCount = 0;
	
	bufferLen = size;
	for ( ; bufferCount < bufferLen; bufferCount++)
	{
		pWCSBuffer[bufferCount] += (wchar_t)Decode_WChar;
	}

	return pWCSBuffer;
}

char * BufferDelCoder(char *pBuffer, int size)
{
	int bufferLen = 0;
	int bufferCount = 0;
	
	bufferLen = size;
	for ( ; bufferCount < bufferLen; bufferCount++)
	{
		pBuffer[bufferCount] -= (char)Decode_Char;
	}
	
	return pBuffer;
}

wchar_t * WCSBufferDelCoder(wchar_t *pWCSBuffer, int size)
{
	int bufferLen = 0;
	int bufferCount = 0;
	
	bufferLen = size;
	for ( ; bufferCount < bufferLen; bufferCount++)
	{
		pWCSBuffer[bufferCount] -= (wchar_t)Decode_WChar;
	}
	
	return pWCSBuffer;
}