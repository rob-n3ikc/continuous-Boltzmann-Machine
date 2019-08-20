/* this is the general .h file for the machine
//
// it includes the definitions of nodes, link geometry, and overall
// parser/framework
//
//
// use the autovector construct when possible
//
*/

// we'll use FILE*
// Lump it.

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

class RBM{
public: // don't really need to hide anything in this class
float *hidden, *visible, *U ,*gradH, *gradV;
float **layers;
float *normal;
bool *initialized, all_initialized;
int nhid,nvis;

RBM( int,int);
~RBM();

void train( float *,int, float,float); 
void graham_schmidt();

void set_H_values( float *,int);

float reconstruct(float *, int);
float score(float *, int);
float* energy(float *,int); // returns U
float dot(float*,float*,int); // utility function

}; //end of class RBM definition

class Kmeans{
public:
	float **nodes, **accumulator, *inaccumulate;
	bool *initialized, all_initialized;
	int nnodes, node_length;

Kmeans(int,int);
~Kmeans();

float distance(float*,float*,int);
void train(float*, int); // accumuates neighbors;
void summarize(); // updates the centres
float score( float*, int);
int which(float *, int);
};

class Knn{
public:

float **nodes;
int *categories;
int in_nodes;
int width, max_nodes;
	
Knn(int, int);
~Knn();
void train(float*,int, int); // loads nodes
float distance(float*,float*,int);
int category(float *, int, int);
};
