

// multiclass style

#include "deus.h"
#include "kdd.h"

float chebyshev(int, float);

int main( int argc, char **argv)
{
if( argc < 4){
	fprintf(stderr,"Usage: train_test nhidden ndeep train_file test_file\n");
	exit(0);
}
int hidden_size;
int hidden_depth;
sscanf(argv[1],"%d", &hidden_size);
sscanf(argv[2],"%d", &hidden_depth);
printf("%d hidden_size\n",hidden_size); fflush(stdout);
kdd *train = new kdd();
kdd *test = new kdd();
FILE *ip;
char line[80];
sscanf(argv[3],"%s",line);
ip = fopen(line,"r");
train->load(ip);
fclose(ip);
sscanf(argv[4],"%s",line);
ip = fopen(line,"r");
test->load(ip);
fclose(ip);

int nfeature;
nfeature = train->nfeature;
int nclass;
nclass = train->classid.size();

// either work, but this has nvisible == nhidden
// not equivalent  hidden_size > nfeature causes a crash
// probably related to the bug
orthog *basis =  new orthog(hidden_size, nfeature, hidden_size);
//orthog *basis =  new orthog(nfeature, nfeature, hidden_size);
basis->build(chebyshev);

float *encoded;
encoded = new float[nfeature];

RBM *a;
a = new RBM(nfeature,hidden_size);
Knn *b;
b = new Knn(hidden_size, hidden_depth);

int window_size = nfeature;

//train->scale(); test->scale();

train->rewind_me();

for( int i=0; i< 10; i++)
{

int who;
while(train->get_next( who, nfeature, encoded))
{

  	// a->train(encoded, nfeature,0.01F,0.1F,0.001F,basis);
  	 a->train(encoded, nfeature,0.01F,0.1F,0.001F,basis);
	if( i == 9)
//	if( i > 3)
	{
// 
	 a->reconstruct(encoded, nfeature,basis);
	 b->train(a->gradH, hidden_size, who);
	}
}
/*
for( int j=0; j< who; j++)
	a[j]->graham_schmidt();
*/
train->rewind_me();
}
// testing
	float inme, correct;
	int who;
	inme = 0.F;
	correct = 0.F;
	test->rewind_me();
	float *confusion;
	confusion = new float[nclass*nclass];
	for( int i=0; i< nclass*nclass; i++)
		confusion[i] = 0.0F;
while( test->get_next( who, nfeature, encoded))
{
	int imin; float emin;
	emin = a->reconstruct(encoded, nfeature,basis);
	imin = b->category(a->gradH, hidden_size, who);

	confusion[imin*nclass+who] += 1.0F;
	if( imin == who){ correct += 1.0F;}
	inme += 1.F;
    
}// testing while
	printf("%f %f %f\n", correct, inme,correct/inme);
	for( int i=0; i< nclass; i++)
	{
	for( int j =0; j < nclass; j++)
		printf("%f ", confusion[i*nclass+j]);
	printf("\n");
	}
}
