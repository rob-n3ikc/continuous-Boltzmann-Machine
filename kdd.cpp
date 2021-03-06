
#include "kdd.h"
// class definitions for the file

// nothing special in the constructors
// so use the defaults


kdd_datum::kdd_datum( int mykind, int s, float *d)
{
	data = new float[s];
	id = mykind;
	for( int i=0; i< s; i++)
		data[i] = d[i];
	inme = s;
}
kdd_datum::~kdd_datum(){ delete[] data;}

bool kdd::scale()
{
	float *max,*min;
	max = new float[the_data[0]->inme];
	min = new float[the_data[0]->inme];
	int s;
	s = the_data[0]->inme;
	for( int j=0; j< the_data.size(); j++)
	for( int i=0; i< s; i++)
	{
		if( the_data[j]->data[i] > max[i]) max[i] = the_data[j]->data[i];
		if( the_data[j]->data[i] < min[i]) min[i] = the_data[j]->data[i];
	}
	for( int i=0; i< s; i++)
	{
		max[i] = max[i]-min[i];
	}
	for( int j=0; j< the_data.size(); j++)
	for( int i=0; i< s; i++)
	{
		if( max[i] == 0.0F) continue;
		 the_data[j]->data[i] -=min[i];
		 the_data[j]->data[i] /=max[i];
		 the_data[j]->data[i] *=2.0F;
		 the_data[j]->data[i] -=1.0F;
	}
	

		
	delete[] min;
	delete[] max;
	return true;
}

bool kdd::get_next(int &id, int s, float *d)
{
	if( iterator >= the_data.size()) return false;
	s = the_data[iterator]->inme;
	for( int i=0; i< s; i++)
		d[i] = the_data[iterator]->data[i];
	id = the_data[iterator++]->id;
	return true;
}


bool kdd::rewind_me(){ iterator = 0;}
/*
here's a header - from glass.

10
@attribute A1	numerical
@attribute A2	numerical
@attribute A3	numerical
@attribute A4	numerical
@attribute A5	numerical
@attribute A6	numerical
@attribute A7	numerical
@attribute A8	numerical
@attribute A9	numerical
@attribute class category 1 2 3 4 5 6
@data
// the only one that's different in my set is heart which is mixed numerical
// and category
*/

bool kdd::load( FILE *ip)
{
	char buffer[10000];
	int nfield;
// the header information.
	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	sscanf( buffer,"%d",&nfield);
	nfeature = nfield -1;
	for( int i=0; i< nfeature; i++)
	{
		if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
// can add parsing here for more complicated data sets
		if( buffer[0] != '@') return false; // it's an error
	}
// @attribute class category a b c d e ...
	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	if( buffer[0] != '@') return false; // it's an error
	char *cp;
	cp = &buffer[0];
	while( *cp != '\n') {if( *cp == '\t' ) *cp = ' '; cp++;}
	cp = &buffer[0];
// this parsing is fragile.
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++;
	while( *cp != ' ') cp++;
	while( *cp == ' ') cp++; 

// assumes single letter categories. Beware
	while(*cp != ' ' ){ 
	if( *cp == '\n') break;
	if( *cp == 13) break;
	if( *cp == '\0') break;
	classid.push_back( *cp); cp++;
	while( *cp  == ' ') cp++;
	if( *cp == '\n') break;
	if( *cp == 13) break;
	if( *cp == '\0') break;
	}


	if( fgets(buffer, sizeof(buffer), ip) != buffer) return false;
	if( buffer[0] != '@') return false; // it's an error

	
	while( fgets(buffer, sizeof(buffer), ip) == buffer) 
	{
	// strip ',' and '\n'
	for( int i=0; i< sizeof(buffer); i++)
		if( buffer[i] == ',') buffer[i] = ' ';
		else if( buffer[i] == '\n'){ buffer[i] = ' '; break;}

	cp = &buffer[0];
	float attr[nfeature];
	int myid;
	for( int i=0; i< nfeature; i++)
	{
	while(*cp == ' ') cp++;
	sscanf(cp,"%f",&attr[i]);	
	while(*cp != ' ') cp++;
	}// i
//	printf("%s\n", cp);
	while(*cp == ' ') cp++;
	myid = -1;
	for( int i=0; i< classid.size(); i++)
		if( *cp == classid[i]){myid = i; break;}
	if( myid < 0) return false; // an error - unspecified class


	the_data.push_back( new kdd_datum( myid, nfeature, attr));

//	printf("%d ",myid);
//	for( int i=0; i< nfeature; i++)
//	printf("%f ", attr[i]);
//	printf("\n");

	}// while fgets
// normalize;
#define NORMALIZE_BY_CASE
#ifdef NORMALIZE_BY_CASE
	float min[nfeature],max[nfeature];
	for( int i=0; i< nfeature; i++)
		min[i] = max[i] = the_data[0]->data[i];
	for( int i=1; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j++)
	{
		if( min[j] > the_data[i]->data[j]) min[j] = the_data[i]->data[j];
		if( max[j] < the_data[i]->data[j]) max[j] = the_data[i]->data[j];
	}
	for( int i=0; i< the_data.size(); i++)
	for( int j=0; j< nfeature; j++)
	{
		float x;
		x = the_data[i]->data[j];
		if( max[j] != min[j])
		the_data[i]->data[j] = 2.*(x-min[j])/(max[j]-min[j])-1.;
		else
		the_data[i]->data[j] = 0.;
//		printf("%f %f\n", x,the_data[i]->data[j]);
	}	
#endif
	return true;
}


