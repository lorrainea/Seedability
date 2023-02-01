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
	unsigned char * yq_gram = ( unsigned char * ) calloc( ( q + 1 ) , sizeof( unsigned char ) );

	unordered_map<string, vector<int>> * xmap = new unordered_map<string, vector<int>>;
	unordered_map<string, vector<int>> * ymap = new unordered_map<string, vector<int>>;
		
	int x_length = strlen( (char*) x );
	int y_length = strlen( (char*) y );
	
	unordered_map<string,vector<int>>::iterator it;

	int shared = 0;
	int errors =0;
	int matches = 0;
	bool close = false;
	
	int last_x = 0;
	int last_y = 0;

    
	for(int j = 0; j<= y_length-q; j++ ) 
	{		
		memcpy ( &yq_gram[0], &y[j], q );				
		it = ymap->find(reinterpret_cast<char*>(yq_gram));
		if ( it == ymap->end() ) 
			ymap->insert( pair<string,vector<int>>(reinterpret_cast<char*>(yq_gram), {j}));
		else it->second.push_back( j );			
	}
	
	for(int j = 0; j<= x_length-q; j++)
	{
		memcpy( &xq_gram[0], &x[j], q );
		it = xmap->find(reinterpret_cast<char*>(xq_gram)); 
		if ( it == xmap->end() ) 
			xmap->insert( pair<string,vector<int>>(reinterpret_cast<char*>(xq_gram), { j}));
		else it->second.push_back( j );
	}
	
	int prev_pos_x = -1;
	int prev_pos_y = -1;
	
	bool first = true;
	
	for(int l = 0; l<= x_length-q; l++ ) 
	{
		memcpy ( &xq_gram[0], &x[l], q );
		unordered_map<string,vector<int>>::iterator it = ymap->find(reinterpret_cast<char*>(xq_gram)); 

		int diff_first = max( x_length,  y_length);
		int diff_second = max( x_length,  y_length);

		int last_y_final = 0;
		int last_x_final = 0;
		
		int last_x_x = 0;
		int last_y_x = 0;
		
		int last_x_y =0 ;
		int last_y_y = 0;
		
		last_x = 0;
		last_y = 0;
		
		
		int prev_pos_x_x = 0;
		int prev_pos_y_x = 0;
		
		int prev_pos_x_y = 0;
		int prev_pos_y_y = 0;
		
		
		if ( it != ymap->end() ) 
		{
			for(int pos=0; pos<it->second.size(); pos++)
			{	
				
				if( abs( (it->second.at(pos) - prev_pos_y ) - (l - prev_pos_x) ) > q )
					continue;
		
				if( it->second.at(pos) == prev_pos_y+1 && l == prev_pos_x+1 )
				{
					close = true;
					last_y_x = it->second.at(pos);
					last_x_x = l;
					diff_first = 0;	
					break;	
				}
				else if( it->second.at(pos) >= prev_pos_y+q )
				{
					int diff = abs( (it->second.at(pos) - (int) prev_pos_y) - ( l - prev_pos_x ) );
					
				
					if(  diff < diff_first )
					{
						diff_first = diff;
			
						last_x_x = l;
						last_y_x = it->second.at(pos);
					}
				}
				
			}
			
			for(int pos=0; pos<it->second.size(); pos++)
				if( it->second.at(pos) <= prev_pos_y )
					it->second.erase(it->second.begin()+pos);
		
			for(int j = prev_pos_y+1 ; j<=last_y_x; j++)
			{	
				memcpy ( &yq_gram[0], &y[j], q );
				unordered_map<string,vector<int>>::iterator it = xmap->find(reinterpret_cast<char*>(yq_gram)); 
						
				if ( it != xmap->end() ) 
				{
					for(int pos=0; pos<it->second.size(); pos++)
					{		
								
						if( abs( (it->second.at(pos) - prev_pos_x ) - (j - prev_pos_y) ) > q )
							continue;
								
						if(  it->second.at(pos) >= prev_pos_x+q )
						{
							int diff = abs( (it->second.at(pos) - (int) prev_pos_x) - ( j - prev_pos_y ) );
									
							if( diff < diff_first )
							{
								diff_second = diff;
							
								last_y_y = j;
								last_x_y = it->second.at(pos);

											
							}
						}
						
					}
					
					for(int pos=0; pos<it->second.size(); pos++)
						if( it->second.at(pos) <= prev_pos_x )
							it->second.erase(it->second.begin()+pos);
			
				}
				
			}
		
			if( diff_first <= diff_second && diff_first <= q )
			{
				last_x = last_x_x;
				last_y = last_y_x;		
			}
			else if( diff_second <=q )
			{
				last_x = last_x_y;
				last_y = last_y_y;
			}
			
			if( diff_first <= q  || diff_second <=q  )
			{				
				if( last_x == prev_pos_x + 1)
				{	
					if( last_y == prev_pos_y + 1 )
					{
						prev_pos_x = last_x;
						shared = shared + 1;
						prev_pos_y = last_y;
						if ( first == true )
						 matches = matches + q;
						else matches = matches + 1;
						
					}
					else if( last_y > prev_pos_y  )
					{
						if( last_y == prev_pos_y + q )
							errors = errors + ( last_y - ( prev_pos_y + q - 1 ) );
						else errors = errors + ( last_y - ( prev_pos_y + q + 1 ) );
						prev_pos_x = last_x;
						shared = shared + 1;
						prev_pos_y = last_y;
						matches = matches + q;		
					}
													
				}
				else if( last_x >= prev_pos_x + q   )
				{		
					
					if( last_y == prev_pos_y + 1 )
					{		
						errors = errors + ( last_x - ( prev_pos_x + q -1) ) ;
						prev_pos_x = last_x;
						shared = shared + 1;
						prev_pos_y = last_y;
						matches = matches + q;
					}
					else if( last_y >= prev_pos_y + q )
					{
					
						if( abs( ( last_x -(int) prev_pos_x ) - ( last_y - (int)prev_pos_y )) == 0 )
							errors = errors + ( last_x - ( (int) prev_pos_x + (int)q) );
										
						else errors = errors + max(( last_x - ( (int) prev_pos_x + (int)q ) ),( last_y - ( (int)prev_pos_y + (int)q ) )) ;
					
						prev_pos_x = last_x;
						shared = shared + 1;
						prev_pos_y = last_y;
						matches = matches +q;
								
					}
						
				}	
				
			}
			
			if ( first == true )
			{
				errors = max( last_x , last_y ) ;
				first = false;
			}
			
		}
	}	
	
		
	if( prev_pos_x + q -1  <  x_length - 1  ||   prev_pos_y + q -1  <  y_length - 1  )
		errors = errors + max( x_length - (prev_pos_x + q -1 ) , y_length- (prev_pos_y + q - 1 )  );
	
	double alignment = (matches * 1.0) / ((errors + matches)*1.0);

	Seed result;
	result.k = q;
	result.matches = 0;
	result.errors = x_length;
	result.alignment = 0;
	result.total_length = 0;
	result.shared_seeds = 0;		
	result.k = q;
	result.matches = matches;
	result.errors = errors;
	result.alignment = alignment;
	result.total_length = errors+matches;
	result.shared_seeds = shared;
	
	
	delete( xmap );
	delete( ymap );
	
	free( xq_gram );
	free( yq_gram );

return result;
}


