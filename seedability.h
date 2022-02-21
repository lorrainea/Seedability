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
bool naive(unsigned char * x, unsigned char * y, Seed * seed, TSwitch sw );
bool contains_qgram(unsigned char * x, unsigned char * y, TSwitch sw, unsigned int q );
unsigned int simple_qgram(unsigned char * x, unsigned char * y, TSwitch sw  );
