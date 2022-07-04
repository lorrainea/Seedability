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

#define INITIAL_SC		-100000
#define ALLOC_SIZE               104857
#define INF			2147483647

using namespace std;

struct TSwitch
 {
   char               * input_ref;
   char               * input_reads;
   char               * output_filename;
   unsigned int         k;
   
 };
 
 struct Seed
 {
   int shared_seeds;
   double alignment;
   int matches;
   int errors;
   int k;
   int total_length;
   unsigned char * y_id;
   
 };


int decode_switches ( int argc, char * argv [], struct TSwitch * sw );
double gettime ( void );
void usage ( void );
Seed q_gram_distance(unsigned char * x_id, unsigned char * y_id, unsigned char * x, unsigned char * y, Seed * seed, TSwitch sw, unsigned int q);
bool contains_qgram(unsigned char * x, unsigned char * y, TSwitch sw, unsigned int q );
