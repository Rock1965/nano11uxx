/*! \headerfile ucxpresso.h "uCXpresso.h"
 *
 */
#ifndef UCXPRESSO_H
#define UCXPRESSO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// VERSION
#define RELEASED				255

/*! \cond PRIVATE */
#define uCXpresso_VER_MAJOR		1
#define uCXpresso_VER_MINOR		0
#define uCXpresso_VER_REV		4
#define uCXpresso_VER_RC		RELEASED
#define uCXpresso_VER_BUILD		"2014/4/12"
#define uCXpresso_VER_STR		"V1.0.4"

//
// Device
//
#define NANO11UXX

//
//	defined
//
#define DEFAULT_POOL_SIZE	(7072)

#if __GNUC__				// GCC
	#if __bool_true_false_are_defined==0
		typedef enum BOOLEAN
		{
			false = 0,
			true = !false
		}bool;
	#endif
	#define TRUE	true
	#define FALSE	false
	#define BOOL	bool
	#define PACK_STRUCT __attribute__ ((__packed__))
#endif

typedef const char*			LPCTSTR;
typedef char*				LPTSTR;

/* These types must be 16-bit, 32-bit or larger integer */
typedef int					INT;
typedef unsigned int		UINT;

/* These types must be 8-bit integer  */
#ifndef CHAR
typedef char				CHAR;
#endif

#ifndef TCHAR
typedef char TCHAR;
#endif

typedef unsigned char		UCHAR;
typedef unsigned char		BYTE;

/* These types must be 16-bit integer */
typedef short				SHORT;
typedef unsigned short		USHORT;
typedef unsigned short		WORD;
typedef unsigned short		WCHAR;

/* These types must be 32-bit integer */
typedef long				LONG;
typedef unsigned long		ULONG;
typedef unsigned long		DWORD;

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned long		dword;
typedef bool 				boolean;



/*! \cond PRIVATE */
/*! \union _u16_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u16_u
{
	uint16_t   Int;
	uint8_t	Char[2];
}u16_u;

/*! \union _u132_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u32_u
{
	uint32_t	Long;
	uint16_t	Int[2];
	uint8_t	Char[4];
}u32_u;

/*! \union _u64_u ucxpresso.h "uCXpresso.h"
 */
typedef union _u64_u
{
	uint64_t	Int64;
    uint32_t   	Long[2];
    uint16_t   	Int[4];
	uint8_t		Char[8];
}u64_u;
/*! \endcond */

#define null_str			((const char *)0)
#define PURE_VIRTUAL_FUNC	0
typedef void* 				xHandle;

//
// Core
//
extern uint32_t MAIN_STACK_SIZE;
#define USB_MEM_BASE	0x20004000
#define USB_MEM_SIZE	0x7FC

//
// RTOS
//
#define MAX_DELAY_TIME	0xffffffff
extern void sleep(int ms);
extern void sysReboot(void);

//
// Bit Control
//
#define bit(x) 		(1<<(x))
#define bitmask(x) 	~bit(x)

#define bit_set(f,b)	f |= bit(b)
#define bit_clr(f,b)	f &= ~bit(b)
#define bit_chk(f,b)    ((f & bit(b))? true : false)

//
// pool memory functions
//
#ifndef _POOL_MEM_FUNCS_
#define _POOL_MEM_FUNCS_
extern void *pool_memcpy(void *dest, const void *sour, size_t size);
extern void *pool_memset(void *dest, int val, size_t size);
#endif

extern void pool_memadd(uint32_t base, size_t size);
//#define memcpy(x,y,z)	xMemcpy(x,y,z)
//#define memset(x,y,z)	xMemset(x,y,z)
#define IS_FLASH_MEM(x)	(((uint32_t)x)<0x100000)	// Max flash memory 1MB

extern size_t heapAvailableSize();		// return the available size of heap memory
extern void *tryMalloc(size_t size);	// try to malloc a memory with

//
// Miscellaneous
//
#define MHZ(x)				(x*1000000ul)
#define KHZ(x)				(x*1000ul)
#define MAX(a, b)			(((a) > (b)) ? (a) : (b))
#define MIN(a, b)			(((a) < (b)) ? (a) : (b))
#define ABS(a)				(((a) < 0) ? -(a) : (a))
#define OFFSET(a, b, max)	(((a) >= (b)) ? (a-b):(max-b+a+1))		// offset

#define MAX_UINT32			0xffffffff
#define CRLF				"\r\n"
#define NO_EFFECT			{}


extern const unsigned char _zero_[];

//
// for GCC/G++ (newlib)
//
#ifndef __IMPORT
#define __IMPORT
#endif

#ifndef _READ_WRITE_RETURN_TYPE
#define _READ_WRITE_RETURN_TYPE _ssize_t
#endif

#ifdef __cplusplus
}
#endif

//
// ARDUINO marco Library
//
#define map(value, fromLow, fromHigh, toLow, toHigh)	((value-fromLow) * (toHigh-toLow) / (fromHigh-fromLow) + toLow)
#define constrain(x, a, b) 								((x<a) ? a : (x>b) ? b : x)

#endif // NANO_H
