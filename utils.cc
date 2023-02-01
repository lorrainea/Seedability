/**
    Seedability: A tool to determine parameters t (shared number of seeds) 
    and k (seed length) between a set of reads or between a reference and 
    set of reads.
    Copyright (C) 2022 Lorraine A.K. Ayad, Rayan Chikhi and Solon P. Pissis
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

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
   { "min-k",				optional_argument, NULL, 'l' },
   { "max-k",				optional_argument, NULL, 'k' },
   { "delta",				optional_argument, NULL, 'd' },
   { "output-filename",		required_argument, NULL, 'o' },
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
   sw -> output_filename		= NULL;
   sw -> d				= 0.05;
   sw -> l				= 3;
   sw -> k				= 15;
   args = 0;

   while ( ( opt = getopt_long ( argc, argv, "r:q:l:k:d:o:h", long_options, &oi ) ) != -1 ) 
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
          
          case 'l':
           val = strtod ( optarg, &ep );
           if ( optarg == ep )
            {
              return ( 0 );
            }
           sw -> l = val;
           break;

	 case 'k':
           val = strtod ( optarg, &ep );
           if ( optarg == ep )
            {
              return ( 0 );
            }
           sw -> k = val;
           break;
           
	 case 'd':
           val = strtod ( optarg, &ep );
           if ( optarg == ep )
            {
              return ( 0 );
            }
           sw -> d = val;
           break;

         case 'h':
           return ( 0 );
       }
    }

   if ( args < 2 )
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
   fprintf ( stdout, "  -o, --output-filename		<str>		Output filename.\n" );
   fprintf ( stdout, " Optional:\n" );
   fprintf ( stdout, "  -l, --min-k			<int>		Minimum k value to explore (Default: 3).\n" );
   fprintf ( stdout, "  -k, --max-k			<int>		Maximum k value to explore (Default: 15).\n" );
   fprintf ( stdout, "  -d, --delta			<double>	Threshold allowance between best alignment identity and alignment identity for larger k values (Default: 0.05).\n" );
   fprintf ( stdout, "  -r, --ref-file		<str>		FASTA reference filename.\n" );
 }

double gettime( void )
{
    struct timeval ttime;
    gettimeofday( &ttime , 0 );
    return ttime.tv_sec + ttime.tv_usec * 0.000001;
}
