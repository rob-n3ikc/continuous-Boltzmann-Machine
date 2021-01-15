
// orthogonal version



#include "deus.h"
#include "kdd.h"

float chebyshev(int, int,int);
float sincos(int, int,int);
float delta(int, int,int);

int main( int argc, char **argv)
{
if( argc < 5){
	fprintf(stderr,"Usage: train_test nhidden nhidden_second train_file test_file\n");
	exit(0);
}
int hidden_size, hidden_deep;
sscanf(argv[1],"%d", &hidden_size);
sscanf(argv[2],"%d", &hidden_deep);
FILE *ip,*jp;
char line[80];
sscanf(argv[3],"%s",line);
ip = fopen(line,"r");
kdd *train = new kdd();
train->load(ip);
//fclose(ip);
sscanf(argv[4],"%s",line);
jp = fopen(line,"r");
kdd *test = new kdd();
test->load(jp);
//fclose(ip);

int nfeature;
nfeature = train->nfeature;
int nclass;
//nclass = train->classid.size();
nclass = 1;

float *encoded;
encoded = new float[nfeature];

//RBM *a[nclass];
//for( int i=0; i< nclass; i++)
//	a[i] = new RBM(nfeature,hidden_size);

RBM *a[nclass];
for( int i=0; i< nclass; i++)
	a[i] = new RBM(nfeature, hidden_size);
Knn *b;
b = new Knn(hidden_size, hidden_deep);

orthog *basis[nclass];
for( int i=0; i< nclass; i++)
	{
		basis[i] = new orthog(nfeature, nfeature, hidden_size);
//		basis[i] = new orthog(hidden_size, nfeature, hidden_size);
//		basis[i]->build(chebyshev);
//		basis[i]->build(sincos);
		basis[i]->build(delta);
	}
	

//basis[0]->test_expansion();
//a->random_init(10101);

int window_size = nfeature;

//train->scale(); test->scale();

train->rewind_me();

int imax;
imax = 100;
//imax = 1;
//for( int i=0; i< 100; i++)
//for( int i=0; i< 3; i++)
//for( int i=0; i< 10; i++)
for( int i=0; i< imax; i++)
{

int who;
who = 0;
while(train->get_next(who, nfeature, encoded ))
{

 	 a[who]->train(encoded, nfeature,0.2F,0.01F,0.0001F,basis[who]);
//	if(i == imax-1)
//		b->train(a->gradH, hidden_size,who);
}
train->rewind_me();
}
// testing
	test->rewind_me();
int who;
float e2;
while(test->get_next( who, nfeature, encoded))
{
        for( int i=0; i< nfeature; i++)
		printf("%f ", encoded[i]);
        printf("\n");
     
	e2 =a[0]->reconstruct(encoded, nfeature,basis[0]);
        printf("%f\n", e2);
        for( int i=0; i< nfeature; i++)
		printf("%f ", a[0]->gradV[i]);
        printf("\n");
}// testing while

}
