
//
// add the category to the training.
// 
//

// multiclass style

#include "deus.h"
#include "kdd.h"

int main( int argc, char **argv)
{
if( argc < 4){
	fprintf(stderr,"Usage: train_test nhidden train_file test_file\n");
	exit(0);
}
int hidden_size;
sscanf(argv[1],"%d", &hidden_size);
kdd *train = new kdd();
kdd *test = new kdd();
FILE *ip;
char line[80];
sscanf(argv[2],"%s",line);
ip = fopen(line,"r");
train->load(ip);
fclose(ip);
sscanf(argv[3],"%s",line);
ip = fopen(line,"r");
test->load(ip);
fclose(ip);

int nfeature;
nfeature = train->nfeature;
int nclass;
nclass = train->classid.size();

float *encoded;
encoded = new float[nfeature];

RBM *a[nclass];
for( int i=0; i< nclass; i++)
	a[i] = new RBM(nfeature,hidden_size);

int window_size = nfeature;

//train->scale(); test->scale();

train->rewind_me();

for( int i=0; i< 10; i++)
{

int who;
while(train->get_next( who, nfeature, encoded))
{

#ifdef anti_train
	for( int j=0; j< nclass; j++)
	{
		if( j==who)
		{
#endif
//	encoded[nfeature] = (float)who;
  	 a[who]->train(encoded, nfeature,0.1F,0.1F);
#ifdef anti_train
		}else{
	encoded[nfeature] = (float)j;
  	 a[who]->train(encoded, nfeature,0.001F,-0.000001F);

		}
	}
#endif
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
	imin = 0;
	float re[nfeature];
	emin = -1;
	for( int j=0; j< nclass; j++)
	{
		float e,e2,e3;
		e3 = 0.0F;
		for( int k=0; k < nclass; k++)
		{
//		encoded[nfeature] = (float)k;
		e2 =a[j]->reconstruct(encoded, nfeature);
		}
//		encoded[nfeature] = (float)j;
		e2 =a[j]->reconstruct(encoded, nfeature);
//		float scale,scaled;
		
//		scale = scaled = 0.0F;
//		for( int k=0; k< nfeature; k++)
//		{
//			scale += a[j]->gradV[k];
//			scaled += encoded[k];
//		}
///		e = a[j]->gradV[nfeature]*scaled/scale;
//		e = 0.0F;
//		for( int k=0; k< nfeature; k++)
//		{
//			float x;
//			x = a[j]->gradV[k]*scaled/scale - encoded[k];
//			e += fabs(x);
//		}
//		e = 0.0F;
//		for( int k=0; k < hidden_size; k++)
//			e += a[j]->hidden[k]/hidden_size;
//		e += e2;
   //          printf("%f %f %d %d %d\n", e3,e2,j, imin,who);
		printf("\n");
		for( int k=0; k< nfeature; k++)
		printf("%f ", a[j]->gradV[k]);
		printf("\n");
		for( int k=0; k< nfeature; k++)
		printf("%f ", encoded[k]);
		printf("\n");
		printf("%d %d %f %f\n",who, j,a[j]->gradV[nfeature-1],e2);
//		printf("%d %d %f %f\n",who, j,e3,e2);
		e = e2;
//		e = e3;
		if( e > emin)
		{
			emin = e;
			imin = j;
		}
	}

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
