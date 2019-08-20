
//
// add the category to the training.
// 
//

// multiclass style

#include "deus.h"
#include "kdd.h"

float correlation( float*,float*, int);
int main( int argc, char **argv)
{
if( argc < 5){
	fprintf(stderr,"Usage: train_test nhidden nhidden train_file test_file\n");
	exit(0);
}
int hidden_size;
int hidden_deep_size;
sscanf(argv[1],"%d", &hidden_size);
sscanf(argv[2],"%d", &hidden_deep_size);
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

float *encoded;
encoded = new float[nfeature+1];

RBM *a;
a = new RBM(nfeature,hidden_size);

//Kmeans *b[nclass];
//for( int i=0; i< nclass; i++)
//	b[i] = new Kmeans(hidden_size,hidden_deep_size);
Knn *b;
b = new Knn(hidden_size, hidden_deep_size);


int window_size = nfeature;

//train->scale(); test->scale();

train->rewind_me();

for( int i=0; i< 10; i++)
{

int who;
while(train->get_next( who, nfeature, encoded))
{

  	 a->train(encoded, nfeature,0.1F,0.1F);
	//if( i > 0)
	//aa->train( a->gradH,nfeature,0.1F,0.1F);
	if( i > 1)
	b->train( a->gradH,hidden_size,who);
}
//	a->graham_schmidt();
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
	imin = 0;
	float re[nfeature];
	emin = -1;
         float e3,e4;
		e3 =a->reconstruct(encoded, nfeature);
		imin = b->category(a->gradH, hidden_size, who);// if doing average make this a loop	

	printf("%f %d %d\n", e3, imin, who);
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
