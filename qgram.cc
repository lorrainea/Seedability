#include <iostream>
#include <fstream>  
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <vector>
#include <unordered_map>
#include <sys/time.h>
#include <omp.h>
#include "seedability.h"
			
Seed q_gram_distance(unsigned char * x_id, unsigned char * y_id, unsigned char * x, unsigned char * y, Seed * seed, TSwitch sw, unsigned int q)
{

	bool threshold_met = false;
	
	unsigned char * xq_gram = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
	unsigned char * prev_xq_gram = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
	
	unsigned char * yq_gram = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
	unsigned char * prev_yq_gram = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
	
	unordered_map<string, vector<int>> * map = new unordered_map<string, vector<int>>;
	unordered_map<string, vector<int>> * ymap = new unordered_map<string, vector<int>>;
		
	int x_length = strlen( (char*) x );
	int y_length = strlen( (char*) y );
	unordered_map<string,vector<int>>::iterator it;

	int shared = 0;
	int errors =0;
	int matches = 0;
	unsigned int prev_pos_x = 0;
	unsigned int prev_pos_y = 0;
	bool first = true;
	bool close = false;
	bool init = true;
	int last_x = 0;
	int last_y = 0;
	int close_pos = 0;
	int closest_dist = max( x_length, y_length);
	
	for(int j = 0; j<= y_length -q; j ++ ) 
	{
	
			
		memcpy ( &yq_gram[0], &y[j], q );				
		it = ymap->find(reinterpret_cast<char*>(yq_gram));
		if ( it == ymap->end() ) 
			ymap->insert( pair<string,vector<int>>(reinterpret_cast<char*>(yq_gram), {j}));
		else it->second.push_back( j );			
	}
	
	for(int j = 0; j<=x_length-q; j++)
	{
		memcpy( &xq_gram[0], &x[j], q );
		it = map->find(reinterpret_cast<char*>(xq_gram)); 
		if ( it == map->end() ) 
			map->insert( pair<string,vector<int>>(reinterpret_cast<char*>(xq_gram), { j}));
		else it->second.push_back( j );
	}
	
	for(int i =0; i<=x_length - q; i++)
	{
		
		for(int l = 0; l<= y_length -q; l ++ ) 
		{
			memcpy ( &yq_gram[0], &y[l], q );
			unordered_map<string,vector<int>>::iterator it = map->find(reinterpret_cast<char*>(yq_gram)); 
			
				
			if ( it != map->end() ) 
			{
				close = false;
				close_pos = 0;
				closest_dist = max( x_length,  y_length);
				bool miss = true;

				for(int pos=0; pos<it->second.size(); pos++)
				{
					int dist = abs( it->second.at(pos) - l );
					
					
					if(  dist <= closest_dist  )
					{
						if(  it->second.at(pos) == prev_pos_x+1 && l == prev_pos_y+1 )
						{
							
							close = true;
							close_pos = it->second.at(pos);
							last_x = close_pos;
							last_y = l;
							closest_dist = dist;
							
						}
						else if( dist < closest_dist && dist >= q-1 && it->second.at(pos) > prev_pos_x )
						{
							unsigned char * temp = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
							unsigned char * tempx = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );
							memcpy ( &temp[0], &x[it->second.at(pos)+q], q );
							memcpy ( &tempx[0], &y[l+q], q );
							
							if(  strcmp((const char*)temp,(const char*)tempx) == 1 )
							{
								miss = false;
								break;
							}
							
							memcpy ( &temp[0], &x[it->second.at(pos)], q );
							memcpy ( &tempx[0], &y[l], q );
							
							unordered_map<string,vector<int>>::iterator it2 = ymap->find(reinterpret_cast<char*>(temp)); 
							unordered_map<string,vector<int>>::iterator it3 = map->find(reinterpret_cast<char*>(tempx)); 
								
							if ( it2 != ymap->end() ) 
							{	
									
								for(int pos2=0; pos2<it2->second.size(); pos2++)
								{	
									int dist2 = abs( it2->second.at(pos2)  - it->second.at(pos));
									
									if( dist2 <= dist && it2->second.at(pos2) > l &&  it2->second.at(pos2) < y_length -1)
									{
									
										miss = false;
										break;
									}
								}
							}
							if ( miss == true )
							{	
								if ( it3 !=map->end() )
								{
									for(int pos2=0; pos2<it3->second.size(); pos2++)
									{	
											
										if( it3->second.at(pos2) < it->second.at(pos) && it3->second.at(pos2) > prev_pos_x )
										{
											miss = false;
											break;
										}
									}
									
									
									
								}
								
							}
							if( miss == true )
							{
								
								close = true;
								close_pos = it->second.at(pos);
								last_x = close_pos;
								last_y = l;
								closest_dist = dist;
							}
							free( temp );
							free( tempx );
						}
						else if ( close_pos > 0 )
						{
								
							if( dist <= closest_dist &&  it->second.at(pos) > prev_pos_x && it->second.at(pos) -prev_pos_x < close_pos - prev_pos_x )						
							{
								close = true;
								close_pos = it->second.at(pos);
								last_x = close_pos;
								last_y = l;
								closest_dist = dist;
							}
							
							
						}
						else if( dist <= closest_dist &&  it->second.at(pos) > prev_pos_x || init == true )
						{
							close = true;
							close_pos = it->second.at(pos);
							last_x = close_pos;
							last_y = l;
							closest_dist = dist;
							init = false;
										
						}	
					}
				}
							
				if( close == true )
				{
						
					if( first == true )
					{
						if( max( close_pos , l ) > 0 )
							errors = errors + max( close_pos , l ); 
						
						first = false;
							
						prev_pos_y = l;
						prev_pos_x = close_pos;
						shared = shared + 1;
						matches = matches + q;
					} 
					else if( close_pos == prev_pos_x + 1)
					{	
						if( l == prev_pos_y + 1 )
						{
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + 1;
						}
						else if( l >= prev_pos_y + q )
						{
							
							if( l == prev_pos_y + q )
								errors = errors + ( l - ( prev_pos_y + q - 1 ) );
							else errors = errors + ( l - ( prev_pos_y + q + 1 ) );
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + q;
						
						}
						else if( l < prev_pos_y + q && l > prev_pos_y ) 
						{
							errors = errors + ( l - ( prev_pos_y + q - 1 ) ) ;
								
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + q;
							
						} 
								
																		
					}
					else if( close_pos >= prev_pos_x + q   )
					{	
						
						if( l == prev_pos_y + 1 )
						{
								
							errors = errors + ( close_pos - ( prev_pos_x + q -1) ) ;
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + q;
						}
						else if( l >= prev_pos_y + q )
						{
	
							if( abs( ( close_pos -(int) prev_pos_x ) - ( l - (int)prev_pos_y )) == 0 )
								errors = errors + ( close_pos - ( (int) prev_pos_x + (int)q) );
									
							else errors = errors + max(( close_pos - ( (int) prev_pos_x + (int)q ) ),( l- ( (int)prev_pos_y + (int)q ) )) ;
				
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches +q;
						
						
						}
						else if( l < prev_pos_y + q && l > prev_pos_y ) 
						{
							errors = errors + ( close_pos - ( prev_pos_x + q -1) ) ;
								
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l; 
							matches = matches + q;
							
						} 
							
								
					}	
					else if( close_pos < prev_pos_x + q && close_pos > prev_pos_x  )
					{	
						
						
						if( l == prev_pos_y + 1 )
						{
							
							errors = errors + ( close_pos - ( prev_pos_x + 1 ) ) ;
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							close_pos = prev_pos_x;
							matches = matches + q;
						}
						else if( l >= prev_pos_y + q )
						{
							errors = errors +  (l - (prev_pos_y + q-1 ));
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + q;

						
						}
						else if( l < prev_pos_y + q && l > prev_pos_y ) 
						{
							errors = errors + abs(( close_pos - ( (int) prev_pos_x + (int)q -1) ) -( l- ( (int)prev_pos_y + (int)q -1) )) ;
								
							prev_pos_x = close_pos;
							shared = shared + 1;
							prev_pos_y = l;
							matches = matches + q;
							
						}
					}
				}
			}
		}
	}	
		
		
	if( last_x + q -1  <  x_length - 1  ||   last_y + q -1  <  y_length - 1  )
		errors = errors + min( x_length - (last_x + q -1 ) , y_length- (last_y + q - 1 )  );
	
	double alignment = (matches * 1.0) / ((errors + matches)*1.0);

	Seed result;
	result.k = q;
	result.matches = matches;
	result.errors = errors;
	result.alignment = alignment;
	result.total_length = errors+matches;
	result.shared_seeds = shared;
	
	delete( map );
	delete( ymap );
	
	free( xq_gram );
	free( prev_xq_gram );
	free( yq_gram );
	free( prev_yq_gram );

return result;
}


