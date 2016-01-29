/* ------------------------------- BGIF.C --------------------------------
    8-bit graphics file formats dumper v1.7.
    Written by Javier Arevalo, AKA Jare/Iguana.
    I compile this with Watcom/32, but I guess it should work with
        any compiler and OS combination for which the typedefs are
        valid i.e. any that I know for PCs... Try it and see.
        Oh, and also check the #pragma pack() thing.

    - DISCLAIMER -

    I hope I have not broken any patents or trade secrets by releasing
        this info. This is purely a mind exercise to break into file
        formats that are quite useful to know. As far as I have been told
        a file format is not subject to anything such as copyright or
        patent, so I have done this because I believe I'm allowed to.

    This is especially true for the GIF file format, which uses the LZW
        decompression algorithm patented by Unisys. This source is free,
        so there should not be any problem using it. However, if you put
        that part of this code in your own programs, make sure that you
        make your own arrangements.

    I PLACE THIS FILE IN THE PUBLIC DOMAIN, SO EVERYTHING CONTAINED HERE
        IS TOTALLY FREE FOR YOU TO EXPLORE AND USE. I DISCLAIM ANY AND ALL
        EVENTS COMING OUT OF ANY POSSIBLE USE (OR LACK OF USE) OR EXISTANCE
        OF THIS FILE. I WON'T BE LIABLE FOR ANYTHING RELATED TO THIS FILE,
        OR ANY PRIOR OR FUTURE VERSION OF IT.

    All trademarks mentioned are property of their respective holders.

    - Merits -

    Mark Morley for the first version of the GIF loader.

    Yann/Iguana for the first PCX reader.

    "Encyclopedia of Graphics File Formats" by James D. Murray & William
        VanRyper, O'Reilly & Associates, Inc. ISBN 1-56592-058-9


    If you decide to work on this further, please make your findings
        public like we have already done, ok? Upload it to
        x2ftp.oulu.fi, THE place for programming info, and/or to
        ftp.cdrom.com. But please PUBLISH it!

    V 1.7:
        - Added change log to have some idea what's going on.
        - Fix for less-than 256 colors in GIF & PCX.
        - Added 8-bit TGA support.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "src\misc\definer.h"

// =======================================================

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef signed char  sbyte;
typedef signed short sword;
typedef signed long  sdword;

typedef byte bool;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define NEW(c) (malloc((c)))

static FILE *FileOpen(const char *fname) {
    return fopen(fname, "rb");
}

static void FileClose(FILE *f) {
    fclose(f);
}

static long FileSize(const char *fname) {
    FILE *f;
    long l;

    f = fopen(fname, "rb");
    if (f == NULL)
        return 0;
    fseek(f, 0, SEEK_END);
    l = ftell(f);
    fclose(f);
    return l;
}

/******************************************************************************
 VGLGIF.C

 VGLGif( char* file, char far *outpix, char far* palette,
         int* width, int* height );

 Routine to load a 256 color .GIF file into a memory buffer.  *Only* 256
 color images are supported here!  Sorry, no routines to SAVE .GIFs...
 Memory required is allocated on the fly and no checks are in place.  If you
 don't have enough memory it will likely crash.  It's easy to add the checks
 yourself, just put one after each call to malloc().  If you supply a pointer
 to a palette, it will be filled in.  If you supply a pointer to a width
 and/or height variable, it will be filled in as well.

 This code is hereby placed in the public domain.  Have fun!

 Mark Morley
 morley@camosun.bc.ca
 ******************************************************************************/

static int getw(FILE *f) {
    int k = (unsigned)fgetc(f);
    k += 256*fgetc(f);
    return k;
}

#define MAX_CODES     4096

static FILE*          fp;
static int            curr_size;
static int            clear;
static int            ending;
static int            newcodes;
static int            top_slot;
static int            slot;
static int            navail_bytes = 0;
static int            nbits_left = 0;
static unsigned char  b1;
static unsigned char  byte_buff[257];
static unsigned char* pbytes;
static unsigned char* stack;
static unsigned char* suffix;
static word         * prefix;

static unsigned long code_mask[13] =
{
   0L,
   0x0001L, 0x0003L,
   0x0007L, 0x000FL,
   0x001FL, 0x003FL,
   0x007FL, 0x00FFL,
   0x01FFL, 0x03FFL,
   0x07FFL, 0x0FFFL
};

static int
get_next_code(void)
{
   register int  i;
   static unsigned long ret;

   if( ! nbits_left )
   {
      if( navail_bytes <= 0 )
      {
         pbytes = byte_buff;
         navail_bytes = getc( fp );
         if( navail_bytes )
            for( i = 0; i < navail_bytes; ++i )
               byte_buff[i] = (byte)getc( fp );
      }
      b1 = *pbytes++;
      nbits_left = 8;
      --navail_bytes;
   }
   ret = b1 >> (8 - nbits_left);
   while( curr_size > nbits_left )
   {
      if( navail_bytes <= 0 )
      {
         pbytes = byte_buff;
         navail_bytes = getc( fp );
         if( navail_bytes )
            for( i = 0; i < navail_bytes; ++i )
               byte_buff[i] = (byte)getc( fp );
      }
      b1 = *pbytes++;
      ret |= b1 << nbits_left;
      nbits_left += 8;
      --navail_bytes;
   }
   nbits_left -= curr_size;

   return( (int) (ret & code_mask[curr_size]) );
}

// *****************

#define OUTB(c) (*outpix++ = (byte)(c))

// *****************

int PIX_LoadGIF(const char* file, byte *outpix, byte * pal, int* width, int* height)
{
   unsigned char* sp;
   int            code, fc, oc;
   int            i;
   unsigned char  size;
   int            c;
   unsigned char  buf[1028];

   fp = FileOpen(file);
   if( !fp ) {
      return( 0 );
   }
   fread( buf, 1, 6, fp );
   if( strncmp( (char *)buf, "GIF", 3 ) )
   {
      FileClose( fp );
      return( 0 );
   }
   fread( buf, 1, 7, fp );
   if (pal != NULL)
      memset(pal, 0, 768);
   for( i = 0; i < 3*(2 << (buf[4] & 7)); i++) {
      c = (byte)getc( fp );
      if (pal != NULL)
          pal[i] = c >> 2;
   }
   fread( buf, 1, 5, fp );
   i = getw( fp );
   if( width )
      *width = i;
   i = getw( fp );
   if( height )
      *height = i;
   if(outpix == NULL) {
      FileClose(fp);
      return( 1 );
   }
   fread( buf, 1, 1, fp );
   size = (byte)getc( fp );
   if( size < 2 || 9 < size )
   {
      FileClose( fp );
      return( 0 );
   }

   stack  = (byte*)NEW( MAX_CODES + 1 );
   suffix = (byte*)NEW( MAX_CODES + 1 );
   prefix = (word*)NEW( sizeof(*prefix) * (MAX_CODES + 1) );

   curr_size = size + 1;
   top_slot = 1 << curr_size;
   clear = 1 << size;
   ending = clear + 1;
   slot = newcodes = ending + 1;
   navail_bytes = nbits_left = 0;
   oc = fc = 0;
   sp = stack;
   while( (c = get_next_code()) != ending )
   {
      if( c == clear )
      {
         curr_size = size + 1;
         slot = newcodes;
         top_slot = 1 << curr_size;
         while( (c = get_next_code()) == clear );
         if( c == ending )
            break;
         if( c >= slot )
            c = 0;
         oc = fc = c;
         OUTB(c);
      }
      else
      {
         code = c;
         if( code >= slot )
         {
            code = oc;
            *sp++ = (byte)fc;
         }
         while( code >= newcodes )
         {
            *sp++ = suffix[code];
            code = prefix[code];
         }
         *sp++ = (byte)code;
         if( slot < top_slot )
         {
            fc = code;
            suffix[slot]   = (byte)fc;
            prefix[slot++] = (word)oc;
            oc = c;
         }
         if( slot >= top_slot && curr_size < 12 )
         {
            top_slot <<= 1;
            ++curr_size;
         }
         while( sp > stack )
         {
            --sp;
            OUTB(*sp);
         }
      }
   }
   free( stack );
   free( suffix );
   free( prefix );
   FileClose( fp );
   return( 1 );
}