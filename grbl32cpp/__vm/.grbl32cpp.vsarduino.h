/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: chipKIT MAX32, Platform=pic32, Package=chipKIT
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __PIC32_32MX795F512L__
#define ARDUINO_ARCH_PIC32
#define F_CPU 80000000L
#define ARDUINO 106013
#define _BOARD_MEGA_
#define MPIDEVER 16777998
#define MPIDE 150
#define IDE Arduino
#define __cplusplus 201103L
#define GCC_VERSION 40803

#define __inline__
#define __asm__(x)
#define __asm__(char)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__


#define __attribute__(x)
#define __LANGUAGE_C_PLUS_PLUS

typedef void *__builtin_va_list;

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() { ; }
#include <arduino.h>
#include <Board_Defs.h> 
#undef cli
#define cli()
#include "grbl32cpp.ino"
#endif
