/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 390fce4 (2024-10-25)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            c799efc (2024-05-14)                                     **
**    ABCC Driver    edc67ee (2024-10-25)                                     **
**                                                                            */
/*******************************************************************************
** Copyright 2015-present HMS Industrial Networks AB.
** Licensed under the MIT License.
********************************************************************************
** File Description:
** Definition of ABCC types
********************************************************************************
*/

#ifndef ABCC_TD_H
#define ABCC_TD_H

/* #define ABCC_SYS_BIG_ENDIAN  */
/* #define ABCC_SYS_16_BIT_CHAR */

#ifndef ABCC_CFG_PAR_EXT_BUS_ENDIAN_DIFF
   #define ABCC_CFG_PAR_EXT_BUS_ENDIAN_DIFF 0
#endif

/*---------------------------------------------------------------------------
**
** BOOL8
**
** INT8
** INT16
** INT32
**
** UINT8
** UINT16
** UINT32
**
** FLOAT32
**
** The standard boolean data type, 8-bit.
** The standard signed 8, 16, and 32 bit data types (respectively).
** The standard unsigned 8, 16, and 32 bit data types (respectively).
**---------------------------------------------------------------------------
*/
typedef int             BOOL;
typedef unsigned char   BOOL8;
typedef unsigned char   UINT8;
typedef signed char     INT8;
typedef unsigned short  UINT16;
typedef signed short    INT16;
typedef unsigned int    UINT32;
typedef signed int      INT32;
typedef float           FLOAT32;

/*---------------------------------------------------------------------------
**
** FLOAT64
**
** INT64
** UINT64
**
** 64-bit floating point type according to IEC 60559 / IEEE 754.
** 64-bit signed and unsigned integer types.
**
** Those types are optional and support for them must also be enabled in
** "abcc_driver_config.h" if they are to be used. See the comments in
** "abcc_config.h" for more information.
**---------------------------------------------------------------------------
*/
typedef double            FLOAT64;
typedef unsigned long int UINT64;
typedef signed long int   INT64;

/*------------------------------------------------------------------------------
** LeINT16
** LeINT32
** LeUINT16
** LeUINT32
**
** Little endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     LeINT16;
typedef INT32     LeINT32;
typedef UINT16    LeUINT16;
typedef UINT32    LeUINT32;

/*------------------------------------------------------------------------------
** BeINT16
** BeINT32
** BeUINT16
** BeUINT32
**
** Big endian data types for words and longwords.
**------------------------------------------------------------------------------
*/
typedef INT16     BeINT16;
typedef INT32     BeINT32;
typedef UINT16    BeUINT16;
typedef UINT32    BeUINT32;

/*---------------------------------------------------------------------------
**
** FALSE
** TRUE
**
** These are the symbolic constants for true and false used in boolean
** data type comparisons.
**
**---------------------------------------------------------------------------
*/
#ifndef FALSE
   #define FALSE     0
#endif

#ifndef TRUE
   #define TRUE      ( !FALSE )
#endif

/*---------------------------------------------------------------------------
**
** NULL
**
** Default value for invalid pointers.
**
**---------------------------------------------------------------------------
*/
#ifndef NULL
   #define NULL 0
#endif

#ifdef __cplusplus
   #define CPLUSPLUS
#endif

#ifdef CPLUSPLUS
   #define EXTFUNC extern "C"
   #define EXTVAR extern "C"
#else
   #define EXTFUNC extern
   #define EXTVAR extern
#endif

/*---------------------------------------------------------------------------
** Compiler/platform-specific packing macros.
**
** The macros ABCC_SYS_PACK_ON, ABCC_SYS_PACK_OFF and PACKED_STRUCT must be
** defined if the compiler/platform in use does not pack (byte-align) data by
** default. This is relevant for the message-related typedef:s and struct:s
** in "abp.h", but may also be used elsewere in the driver and example code.
**
** Five example sets of macros are listed here for the compilers that the
** official ports of the ABCC example code use. GCC, IAR, Keil, Microsoft, TI.
**---------------------------------------------------------------------------
*/
#if defined( __GNUC__ )
   /*
   ** https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
   ** https://gcc.gnu.org/onlinedocs/gcc-10.2.0/gcc/Common-Type-Attributes.html
   */
   #define ABCC_SYS_PACK_ON
   #define ABCC_SYS_PACK_OFF
   #define PACKED_STRUCT       __attribute__((packed))
#elif defined( __IAR_SYSTEMS_ICC__ )
   /*
   ** "IAR C/C++ Development Guide", "Twenty-fifth edition: May 2019"
   ** Document "DARM-25", "EWARM_DevelopmentGuide.ENU.pdf"
   **
   ** Chapter "Descriptions of extended keywords", page 375
   ** Chapter "Supported GCC attributes", page 381
   ** Chapter "Descriptions of pragma directives", page 400
   ** Chapter "Descriptions of compiler options", page 465
   ** Packing-related restrictions are listed on page 375
   */
   #define ABCC_SYS_PACK_ON   _Pragma("pack(1)")
   #define ABCC_SYS_PACK_OFF  _Pragma("pack()")
   #define PACKED_STRUCT
#elif defined( __ARMCC_VERSION )
   /*
   ** "Arm(R) Compiler, Version 6.12, armclang Reference Guide"
   ** Document "100067_0612_00_en"
   **
   ** Chapter 3.26 "__attribute__((packed)) type attribute", page 3-185
   ** Chapter 3.32 "__attribute__((packed)) variable attribute", page 3-191
   ** Chapter 5.5 "#pragma pack(...)", page 5-219
   ** Chapter 6.2 "Predefined macros", page 6-228
   **
   ** "ARM(R) Compiler v5.06 for μVision(R) Version 5, armcc User Guide"
   ** Document "ARM DUI0375G"
   **
   ** Chapter 9.12 "__packed", page 9-521
   ** Chapter 9.58 "__attribute__((packed)) type attribute", page 9-571
   ** Chapter 9.65 "__attribute__((packed)) variable attribute", page 9-578
   ** Chapter 9.94 "#pragma pack(n)", page 9-608
   ** Chapter 9.156 "Predefined macros", page 9-682
   */
   #define ABCC_SYS_PACK_ON   _Pragma("pack(1)")
   #define ABCC_SYS_PACK_OFF  _Pragma("pack()")
   #define PACKED_STRUCT
#elif defined( _MSC_VER )
   /*
   ** https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
   ** https://docs.microsoft.com/en-us/cpp/preprocessor/pack
   */
   #define ABCC_SYS_PACK_ON   __pragma(pack(push,ABCC_BYTEALIGN,1))
   #define ABCC_SYS_PACK_OFF  __pragma(pack(pop,ABCC_BYTEALIGN))
   #define PACKED_STRUCT
#elif defined( __TI_COMPILER_VERSION__ )
   /*
   ** "TMS320C28x Optimizing C/C++ Compiler v18.12.0.LTS, User's Guide"
   ** "Literature Number: SPRU514R"
   ** "August 2001–Revised April 2019"
   **
   ** Chapter "2.5.1 Predefined Macro Names", page 40
   ** Chapter "6.16.3 Variable Attributes", page 143
   **
   ** Explicit packing is not supported by this compiler/platform.
   */
   #define ABCC_SYS_PACK_ON
   #define ABCC_SYS_PACK_OFF
   #define PACKED_STRUCT
#endif /* Packing macros */

#endif  /* inclusion lock */
