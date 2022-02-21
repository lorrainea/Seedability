
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <float.h>
#include <sys/time.h>
#include <getopt.h>
#include <assert.h>
#include "seedability.h"

static struct option long_options[] =
 {
   { "read-file",              	required_argument, NULL, 'r' },
   { "ref-file",        	      	optional_argument, NULL, 'q' },
   { "help",                    	no_argument,       NULL, 'h' },
   { NULL,                      	0,                 NULL,  0  }
 };


/* 
Decode the input switches 
*/
int decode_switches ( int argc, char * argv [], struct TSwitch * sw )
 {
   int          oi;
   int          opt;
   double       val;
   char       * ep;
   int          args;

   /* initialisation */
   sw -> input_reads           	= NULL;
   sw -> input_ref	          	= NULL;
   args = 0;

   while ( ( opt = getopt_long ( argc, argv, "r:q:h", long_options, &oi ) ) != -1 ) 
    {

      switch ( opt )
       {

         case 'r':
           sw -> input_ref = ( char * ) malloc ( ( strlen ( optarg ) + 1 ) * sizeof ( char ) );
           strcpy ( sw -> input_ref, optarg );
           break;

         case 'q':
           sw -> input_reads = ( char * ) malloc ( ( strlen ( optarg ) + 1 ) * sizeof ( char ) );
           strcpy ( sw -> input_reads, optarg );
           args ++;
           break;

         case 'o':
           sw -> output_filename = ( char * ) malloc ( ( strlen ( optarg ) + 1 ) * sizeof ( char ) );
           strcpy ( sw -> output_filename, optarg );
           args ++;
          break;


         case 'h':
           return ( 0 );
       }
    }

   if ( args < 1 )
     {
       usage ();
       exit ( 1 );
     }
   else
     return ( optind );
 }

/* 
Usage of the tool 
*/
void usage ( void )
 {
   fprintf ( stdout, " Usage: seedability <options>\n" );
   fprintf ( stdout, " Standard (Mandatory):\n" );
   fprintf ( stdout, "  -q, --reads-file		<str>		multiFASTA reads filename.\n" ); 
   fprintf ( stdout, " Optional:\n" );
   fprintf ( stdout, "  -r, --ref-file		<str>		FASTA reference filename.\n" );
 }

double gettime( void )
{
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
}
