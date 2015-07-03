/*****************************************************************************/
/* STDIO.H v4.1.4                                                            */
/* Copyright (c) 1993-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/
#ifndef _STDIO 
#define _STDIO

#include <linkage.h>
#include <stdarg.h>

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cstdio> IS RECOMMENDED OVER <stdio.h>.  <stdio.h> IS PROVIDED FOR
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std {
#endif

/****************************************************************************/
/* TYPES THAT ANSI REQUIRES TO BE DEFINED                                   */
/****************************************************************************/
#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_T_TYPE__ size_t;
#endif

typedef struct {
      int fd;                    /* File descriptor */
      unsigned char* buf;        /* Pointer to start of buffer */
      unsigned char* pos;        /* Position in buffer */
      unsigned char* bufend;     /* Pointer to end of buffer */
      unsigned char* buff_stop;  /* Pointer to last read char in buffer */
      unsigned int   flags;      /* File status flags (see below) */
      int index;                 /* Location in ftable */
} FILE;

#ifndef _FPOS_T
#define _FPOS_T
typedef long fpos_t;
#endif /* _FPOS_T */

/****************************************************************************/
/* MACROS THAT DEFINE AND USE FILE STATUS FLAGS                             */
/****************************************************************************/
#define _IOFBF       0x0001
#define _IOLBF       0x0002
#define _IONBF       0x0004
#define _BUFFALOC    0x0008
#define _MODER       0x0010
#define _MODEW       0x0020
#define _MODERW      0x0040
#define _MODEA       0x0080
#define _MODEBIN     0x0100
#define _STATEOF     0x0200
#define _STATERR     0x0400
#define _UNGETC      0x0800
#define _TMPFILE     0x1000

#define _SET(_fp, _b)      (((_fp)->flags) |= (_b))
#define _UNSET(_fp, _b)    (((_fp)->flags) &= ~(_b))
#define _STCHK(_fp, _b)    (((_fp)->flags) & (_b))
#define _BUFFMODE(_fp)     (((_fp)->flags) & (_IOFBF | _IOLBF | _IONBF))
#define _ACCMODE(_fp)      (((_fp)->flags) & (_MODER | _MODEW))

/****************************************************************************/
/* MACROS THAT ANSI REQUIRES TO BE DEFINED                                  */
/****************************************************************************/
#define BUFSIZ          256 
#define FOPEN_MAX       12
#define FILENAME_MAX    256  
#define TMP_MAX         65535

#define SEEK_SET  (0x0000)
#define SEEK_CUR  (0x0001)
#define SEEK_END  (0x0002)

#ifndef NULL
#define NULL 0
#endif

#ifndef EOF
#define EOF    (-1)
#endif

#define stdin     (&_ftable[0])      
#define stdout    (&_ftable[1])
#define stderr    (&_ftable[2])

#define L_tmpnam  (sizeof(P_tmpdir) + 15)

/******** END OF ANSI MACROS ************************************************/

#define P_tmpdir        ""                   /* Path for temp files         */

/****************************************************************************/
/* DEVICE AND STREAM RELATED DATA STRUCTURES AND MACROS                     */
/****************************************************************************/
/*- If you modify these values, be sure to also modify the ftable[] to     -*/
/*- correctly initialize the entries.  This is necessary since we do not   -*/
/*- clear bss by default!                                                  -*/
/****************************************************************************/
#define _NFILE          20                   /* Max number of files open    */

extern _DATA_ACCESS FILE _ftable[_NFILE];
extern _DATA_ACCESS char _tmpnams[_NFILE][L_tmpnam];

/****************************************************************************/
/*   FUNCTION DEFINITIONS  - ANSI                                           */
/****************************************************************************/
/****************************************************************************/
/* OPERATIONS ON FILES                                                      */
/****************************************************************************/
extern _CODE_ACCESS int     remove(const char *_file);
extern _CODE_ACCESS int     rename(const char *_old, const char *_new);
extern _CODE_ACCESS FILE   *tmpfile(void);
extern _CODE_ACCESS char   *tmpnam(char *_s);

/****************************************************************************/
/* FILE ACCESS FUNCTIONS                                                    */
/****************************************************************************/
extern _CODE_ACCESS int     fclose(FILE *_fp); 
extern _CODE_ACCESS FILE   *fopen(const char *_fname, const char *_mode);
extern _CODE_ACCESS FILE   *freopen(const char *_fname, const char *_mode,
			            register FILE *_fp);
extern _CODE_ACCESS void    setbuf(register FILE *_fp, char *_buf);
extern _CODE_ACCESS int     setvbuf(register FILE *_fp, register char *_buf, 
			            register int _type, register size_t _size);
extern _CODE_ACCESS int     fflush(register FILE *_fp); 

/****************************************************************************/
/* FORMATTED INPUT/OUTPUT FUNCTIONS                                         */
/****************************************************************************/
extern _CODE_ACCESS int fprintf(FILE *_fp, const char *_format, ...);
extern _CODE_ACCESS int fscanf(FILE *_fp, const char *_fmt, ...);
extern _CODE_ACCESS int printf(const char *_format, ...);
extern _CODE_ACCESS int scanf(const char *_fmt, ...);
extern _CODE_ACCESS int sprintf(char *_string, const char *_format, ...);
extern _CODE_ACCESS int snprintf(char *_string, size_t _n, 
				 const char *_format, ...);
extern _CODE_ACCESS int sscanf(const char *_str, const char *_fmt, ...);
extern _CODE_ACCESS int vfprintf(FILE *_fp, const char *_format, va_list _ap);
extern _CODE_ACCESS int vprintf(const char *_format, va_list _ap);
extern _CODE_ACCESS int vsprintf(char *_string, const char *_format,
				 va_list _ap);
extern _CODE_ACCESS int vsnprintf(char *_string, size_t _n, 
				  const char *_format, va_list _ap);

/****************************************************************************/
/* CHARACTER INPUT/OUTPUT FUNCTIONS                                         */
/****************************************************************************/
extern _CODE_ACCESS int     fgetc(register FILE *_fp);
extern _CODE_ACCESS char   *fgets(char *_ptr, register int _size,
				  register FILE *_fp);
extern _CODE_ACCESS int     fputc(int _c, register FILE *_fp);
extern _CODE_ACCESS int     fputs(const char *_ptr, register FILE *_fp);
extern _CODE_ACCESS int     getc(FILE *_p);
extern _CODE_ACCESS int     getchar(void);
extern _CODE_ACCESS char   *gets(char *_ptr); 
extern _CODE_ACCESS int     putc(int _x, FILE *_fp);
extern _CODE_ACCESS int     putchar(int _x);
extern _CODE_ACCESS int     puts(const char *_ptr); 
extern _CODE_ACCESS int     ungetc(int _c, register FILE *_fp);

/****************************************************************************/
/* DIRECT INPUT/OUTPUT FUNCTIONS                                            */
/****************************************************************************/
extern _CODE_ACCESS size_t  fread(void *_ptr, size_t _size, size_t _count,
				  FILE *_fp);
extern _CODE_ACCESS size_t  fwrite(const void *_ptr, size_t _size,
				   size_t _count, register FILE *_fp); 

/****************************************************************************/
/* FILE POSITIONING FUNCTIONS                                               */
/****************************************************************************/
extern _CODE_ACCESS int     fgetpos(FILE *_fp, fpos_t *_pos);
extern _CODE_ACCESS int     fseek(register FILE *_fp, long _offset,
				  int _ptrname);
extern _CODE_ACCESS int     fsetpos(FILE *_fp, const fpos_t *_pos);
extern _CODE_ACCESS long    ftell(FILE *_fp);
extern _CODE_ACCESS void    rewind(register FILE *_fp); 

/****************************************************************************/
/* ERROR-HANDLING FUNCTIONS                                                 */
/****************************************************************************/
extern _CODE_ACCESS void    clearerr(FILE *_fp);
extern _CODE_ACCESS int     feof(FILE *_fp);
extern _CODE_ACCESS int     ferror(FILE *_fp);
extern _CODE_ACCESS void    perror(const char *_s);

#define _getchar()      getc(stdin)
#define _putchar(_x)    putc((_x), stdout)
#define _clearerr(_fp)   ((void) ((_fp)->flags &= ~(_STATERR | _STATEOF)))

#define _ferror(_x)     ((_x)->flags & _STATERR)

#define _remove(_fl)    (unlink((_fl)))

#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::size_t;
using std::FILE;
using std::fpos_t;
using std::_ftable;
using std::_tmpnams;
using std::remove;
using std::rename;
using std::tmpfile;
using std::tmpnam;
using std::fclose;
using std::fopen;
using std::freopen;
using std::setbuf;
using std::setvbuf;
using std::fflush;
using std::fprintf;
using std::fscanf;
using std::printf;
using std::scanf;
using std::sprintf;
using std::sscanf;
using std::vfprintf;
using std::vprintf;
using std::vsprintf;
using std::fgetc;
using std::fgets;
using std::fputc;
using std::fputs;
using std::getc;
using std::getchar;
using std::gets;
using std::putc;
using std::putchar;
using std::puts;
using std::ungetc;
using std::fread;
using std::fwrite;
using std::fgetpos;
using std::fseek;
using std::fsetpos;
using std::ftell;
using std::rewind;
using std::clearerr;
using std::feof;
using std::ferror;
using std::perror;
#endif  /* _CPP_STYLE_HEADER */

#endif  /* __cplusplus */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::size_t;
using std::FILE;
using std::fpos_t;
using std::_ftable;
using std::_tmpnams;
using std::remove;
using std::rename;
using std::tmpfile;
using std::tmpnam;
using std::fclose;
using std::fopen;
using std::freopen;
using std::setbuf;
using std::setvbuf;
using std::fflush;
using std::fprintf;
using std::fscanf;
using std::printf;
using std::scanf;
using std::sprintf;
using std::sscanf;
using std::vfprintf;
using std::vprintf;
using std::vsprintf;
using std::fgetc;
using std::fgets;
using std::fputc;
using std::fputs;
using std::getc;
using std::getchar;
using std::gets;
using std::putc;
using std::putchar;
using std::puts;
using std::ungetc;
using std::fread;
using std::fwrite;
using std::fgetpos;
using std::fseek;
using std::fsetpos;
using std::ftell;
using std::rewind;
using std::clearerr;
using std::feof;
using std::ferror;
using std::perror;
#endif  /* _CPP_STYLE_HEADER */

#endif  /* __cplusplus */

#endif  /* #ifndef _STDIO */

