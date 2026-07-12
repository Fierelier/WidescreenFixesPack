#pragma once

#ifndef WINVER
	#define WINVER 0x0600 // Windows Vista
#endif

#define _WIN32_WINNT WINVER

#if WINVER < 0x0600
	#define TGT_SSCANF sscanf
	#define TGT_WCSCPY(dest,dest_size,src) wcscpy(dest,src)
	#define TGT_STRCPY(dest,dest_size,src) strncpy((char*)dest,(const char*)src,(size_t)dest_size)
#else
	#define TGT_SSCANF sscanf_s
	#define TGT_WCSCPY wcscpy_s
	#define TGT_STRCPY strcpy_s
#endif

//#include <SDKDDKVer.h>
