#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT ReadData(HWND hwnd, char* buffer, DWORD bufferSize);
void DLL_EXPORT WriteDataInNewLine(HWND hwnd, char* data);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
