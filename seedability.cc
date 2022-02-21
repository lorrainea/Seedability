
#include <iostream>
#include <fstream>  
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>  
#include <limits>
#include <unordered_map>
#include <sys/time.h>
#include <omp.h>
#include <map>
#include "seedability.h"

int main(int argc, char **argv)
{
	struct TSwitch  sw;
	FILE *          in_fd;                 
        char *          input_ref;        
        char *          input_reads;

        unsigned char ** read    = NULL;         // the sequence(s) in memory
        unsigned char ** read_id = NULL;         // the sequence(s) id in memory
        unsigned char *  ref = NULL;
        unsigned char *  ref_id = NULL;

	unsigned int    i, j;
	unsigned int    total_length = 0;
	
	Seed final_result;

	/* Decodes the arguments */
        i = decode_switches ( argc, argv, &sw );


	/* Check the arguments */
        if ( i < 1 )
        {
                usage ();
                return ( 1 );
        }
        else
        {

                input_reads       = sw . input_reads;
                input_ref	  = sw . input_ref;
		if ( input_reads == NULL )
		{
			fprintf ( stderr, " Error: Cannot open reads file for input!\n" );
			return ( 1 );
		}

        }

	double start = gettime();

	/* Read the (Multi)FASTA file in memory */
	fprintf ( stderr, " Reading the (Multi)FASTA input file: %s\n", input_reads );
	if ( ! ( in_fd = fopen ( input_reads, "r") ) )
	{
		fprintf ( stderr, " Error: Cannot open file %s!\n", input_reads );
		return ( 1 );
	}

	char c;
        unsigned int num_read = 0;           // the total number of sequences considered
	unsigned int max_alloc_read_id = 0;
	unsigned int max_alloc_read = 0;
	c = fgetc( in_fd );

	do
	{
		if ( c != '>' )
		{
			fprintf ( stderr, " Error: input file %s is not in FASTA format!\n", input_reads );
			return ( 1 );
		}
		else
		{
			if ( num_read >= max_alloc_read_id )
			{
				read_id = ( unsigned char ** ) realloc ( read_id,   ( max_alloc_read_id + ALLOC_SIZE ) * sizeof ( unsigned char * ) );
				max_alloc_read_id += ALLOC_SIZE;
			}

			unsigned int max_alloc_read_id_len = 0;
			unsigned int read_id_len = 0;

			read_id[ num_read ] = NULL;

			while ( ( c = fgetc( in_fd ) ) != EOF && c != '\n' )
			{
				if ( read_id_len >= max_alloc_read_id_len )
				{
					read_id[ num_read ] = ( unsigned char * ) realloc ( read_id[ num_read ],   ( max_alloc_read_id_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
					max_alloc_read_id_len += ALLOC_SIZE;
				}
				read_id[ num_read ][ read_id_len++ ] = c;
			}
			read_id[ num_read ][ read_id_len ] = '\0';
			
		}

		if ( num_read >= max_alloc_read )
		{
			read = ( unsigned char ** ) realloc ( read,   ( max_alloc_read + ALLOC_SIZE ) * sizeof ( unsigned char * ) );
			max_alloc_read += ALLOC_SIZE;
		}

		unsigned int read_len = 0;
		unsigned int max_alloc_read_len = 0;

		read[ num_read ] = NULL;

		while ( ( c = fgetc( in_fd ) ) != EOF && c != '>' )
		{
			if( read_len == 0 && c == '\n' )
			{
				fprintf ( stderr, " Omitting empty sequence in file %s!\n", input_reads );
				c = fgetc( in_fd );
				break;
			}
			if( c == '\n' || c == ' ' ) continue;

			c = toupper( c );

			if ( read_len >= max_alloc_read_len )
			{
				read[ num_read ] = ( unsigned char * ) realloc ( read[ num_read ],   ( max_alloc_read_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
				max_alloc_read_len += ALLOC_SIZE;
			}

			read[ num_read ][ read_len++ ] = c;
		}

		if( read_len != 0 )
		{
			if ( read_len >= max_alloc_read_len )
			{
				read[ num_read ] = ( unsigned char * ) realloc ( read[ num_read ],   ( max_alloc_read_len + ALLOC_SIZE ) * sizeof ( unsigned char ) ); 
				max_alloc_read_len += ALLOC_SIZE;
			}
			read[ num_read ][ read_len++ ] = '\0';

			total_length += read_len;
			num_read++;
		}
		
	} while( c != EOF );

	read[ num_read ] = NULL;

	
	if( sw . input_ref != NULL )
	{
		fprintf ( stderr, " Reading the FASTA input file: %s\n", input_ref );
		if ( ! ( in_fd = fopen ( input_ref, "r") ) )
		{
		        fprintf ( stderr, " Error: Cannot open file %s!\n", input_ref );
		        return ( 1 );
		}

		char c;
		unsigned int num_ref = 0;           	// the total number of sequences considered
		unsigned int total_length = 0;          // the total number of sequences considered
		unsigned int max_alloc_ref_id = 0;
		unsigned int max_alloc_ref = 0;
		unsigned int ref_len = 0;
		c = fgetc( in_fd );
		
		do
        	{
			if ( c != '>' )
			{
				fprintf ( stderr, " Error: input file %s is not in FASTA format!\n", input_ref );
				return ( 1 );
			}
			else
			{
				max_alloc_ref_id = 0;
				int ref_id_len = 0;
				while ( ( c = fgetc( in_fd ) ) != EOF && c != '\n' )
				{
					if ( ref_id_len >= max_alloc_ref_id )
					{
						ref_id = ( unsigned char * ) realloc ( ref_id,   ( max_alloc_ref_id + ALLOC_SIZE ) * sizeof ( unsigned char ) );
						max_alloc_ref_id += ALLOC_SIZE;
					}
					ref_id[ ref_id_len++ ] = c;
				}
				ref_id[ ref_id_len ] = '\0';
				
			}
		        
		        int max_alloc_ref_len = 0;

		        while ( ( c = fgetc( in_fd ) ) != EOF )
		        {
		                if( ref_len == 0 && c == '\n' )
		                {
		                        fprintf ( stderr, " Omitting empty sequence in file %s!\n", input_ref );
		                        c = fgetc( in_fd );
		                        break;
		                }
		                if( c == '\n' || c == ' ' ) 
					continue;
				
		                c = toupper( c );

		                if ( ref_len >= max_alloc_ref_len )
		                {
		                        ref = ( unsigned char * ) realloc ( ref,   ( max_alloc_ref_len + ALLOC_SIZE ) * sizeof ( unsigned char ) );
		                        max_alloc_ref_len += ALLOC_SIZE;
		                }

				ref[ ref_len++ ] = c;
		        }

			ref[ ref_len ] = '\0';

       	} 
		while( c != EOF );
	}


	if ( fclose ( in_fd ) )
	{
		fprintf( stderr, " Error: file close error!\n");
		return ( 1 );
	}

	bool seed_found = true;
	unsigned int seed_length = 0;
	unsigned int shared_seeds = 0;
	unsigned int max_q_gram = 0;
	
	double best_alignment = 0;
	
	unsigned int final_k = 0;
	double final_alignment = 1.0;
	unsigned int final_shared = 0;
	
	unsigned int f_k = 0;
	double f_a = 1.0;
	int f_s = 0;
			
	int matches = 0;
	int total_len = 0;
	int errors = 0;
		
	if( sw . input_ref != NULL )
	{
		
		Seed * seed;
		if ( ( seed = ( Seed * ) calloc ( ( max_q_gram+1 ) , sizeof( Seed ) ) ) == NULL )
		{
			fprintf( stderr, " Error: Cannot allocate memory!\n" );
			return 1;
		}
		

		for(int i = 0; i< num_read; i++)
		{
			int q_gram_size = min( (int) strlen( ( char* ) ref ), min( (int) strlen( (char*) read[i] ), 15 ) );
			
			best_alignment = 0;
			
			for(int q = q_gram_size; q>=3; q--)
			{
				Seed result =   q_gram_distance( ref_id, read_id[i], ref, read[i], seed, sw, q );	
				if( result.alignment > best_alignment )
				{
					best_alignment  = result.alignment;
					final_alignment = result.alignment;
					final_shared = result.shared_seeds;
					final_k = result.k;
					matches = result.matches;
					total_len  = result.total_length;
					errors = result.errors;
				}
					
	
			}
			
				
			if( final_alignment < f_a )
			{
				f_a = final_alignment;
				f_s = final_shared;
				f_k = final_k;
			}
		}
	}			
    	else
    	{

	
		Seed * seed;
		if ( ( seed = ( Seed * ) calloc ( ( max_q_gram+1 ) , sizeof( Seed ) ) ) == NULL )
		{
			fprintf( stderr, " Error: Cannot allocate memory!\n" );
			return 1;
		}
		
		for(int i = 0; i< num_read; i++)
		{	
			for(int j = i+1; j<num_read; j++)
			{
				int q_gram_size = min( (int) strlen( ( char* ) read[i] ), min( (int) strlen( (char*) read[j] ), 15 ) );
				best_alignment = 0;
				
				
				for(int q =q_gram_size; q>=3; q--)
				{
					Seed result =   q_gram_distance( read_id[i], read_id[j], read[i], read[j], seed, sw, q );	
						
					if( result.alignment > best_alignment )
					{
						best_alignment  = result.alignment;
						final_alignment = result.alignment;
						final_shared = result.shared_seeds;
						final_k = result.k;
						matches = result.matches;
						total_len  = result.total_length;
						errors = result.errors;
					}
					
	
				}

					
				if( final_alignment < f_a )
				{
					f_a = final_alignment;
					f_s = final_shared;
					f_k = final_k;
				}
						
			}
		}	
	}
	
	
	printf ( "%d\t%d\n" , f_k, f_s );
	

	double end = gettime();
        fprintf( stderr, "Elapsed time: %lf secs.\n", end - start );

	free ( sw . input_reads );

	for( int i =0; i<num_read; i++)
	{
		free ( read[i] );
		free( read_id[i] );
	}

	free( read_id );
	free( read );
	
	if( sw . input_ref  != NULL )
	{
		free( sw . input_ref );
		free( ref_id );
		free( ref );
	}

return 0;
}


