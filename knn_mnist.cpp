


// multiclass style

#include "deus.h"
#include "mnist.h"

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
mnist *train = new mnist(ip);
//fclose(ip);
sscanf(argv[4],"%s",line);
jp = fopen(line,"r");
mnist *test = new mnist(jp);
//fclose(ip);

int nfeature;
//nfeature = train->nfeature;
nfeature = WIDTH*WIDTH; // from mnist.h
int nclass;
//nclass = train->classid.size();
nclass = 10;

float *encoded;
encoded = new float[nfeature];

//RBM *a[nclass];
//for( int i=0; i< nclass; i++)
//	a[i] = new RBM(nfeature,hidden_size);

RBM *a;
a = new RBM(nfeature, hidden_size);
Knn *b;
b = new Knn(hidden_size, hidden_deep);

	


int window_size = nfeature;

//train->scale(); test->scale();

train->rewind_me();

for( int i=0; i< 3; i++)
{

int who;
//while(train->get_next( who, nfeature, encoded))
while(train->get_next( ))
{
	for( int ii=0; ii< nfeature; ii++)
		encoded[ii] = (float)train->raw[ii]/255.;

	who = train->id;
  	// a[who]->train(encoded, nfeature,0.0001F,0.01F);
  	 a->train(encoded, nfeature,0.01F,0.001F);
	if(i > 0)
		b->train(a->gradH, hidden_size,who);
//	a[who]->graham_schmidt();
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
	FILE *oop = fopen("Pixmaps_big.xpm","w");
	test->xpm_multi_header(oop, 2,100); // raw reconstruct & predict
	int irow;
	irow = 0;
while( test->get_next( ))
{
	test->xpm_multi_add(0,0,irow);
	for( int i=0; i< nfeature; i++)
		encoded[i] = (float)test->raw[i]/255.;
	//	encoded[i] = test->raw[i];
	who = test->id;
	int imin; float emin;
	imin = 0;
	float re[nfeature];
	float e,e2,e3;
	e3 =a->reconstruct(encoded, nfeature);
	for( int i=0; i< nfeature; i++)
		test->reconstructed[i] = a->gradV[i];
	test->xpm_multi_add(2,1,irow++);
	emin = -10.e10;
//	emin =a[0]->score(encoded, nfeature);
	e2 =a->reconstruct(encoded, nfeature);
	imin = b->category(a->gradH, hidden_size,who);
/*
	for( int j=0; j< nclass; j++)
	{
		e2 = -b[j]->score(a->gradH, hidden_size);
//		e2 =a[j]->score(encoded, nfeature);
             printf("%f %f %d %d %d\n", e2,emin,j, imin,who);
		if( e2 > emin)
		{
			emin = e2;
			imin = j;
		}
	}
*/
	printf("%f %d %d\n", e2, imin, who);


	confusion[imin*nclass+who] += 1.0F;
	if( imin == who){ correct += 1.0F;}
	inme += 1.F;
	if( irow == 100){ test->xpm_multi_close(oop); fclose(oop); break;}
    
}// testing while
	printf("%f %f %f\n", correct, inme,correct/inme);
	for( int i=0; i< nclass; i++)
	{
	for( int j =0; j < nclass; j++)
		printf("%f ", confusion[i*nclass+j]);
	printf("\n");
	}
}
