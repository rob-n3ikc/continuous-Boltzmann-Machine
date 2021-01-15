// the machine definition file.
//
// FUZZY version
//
#include "fuzzy.h"
#include <cstring>

using namespace std;


bool fuzzy_category::init_count()
{
	if( count.size() < 1) count.push_back(0.F);
	for( int i=0; i< count.size(); i++) count[i] = 0.F;
	return true;
}

bool fuzzy_category::load( int who, float what)
{
	if( count.size() < 1) count.push_back(0.F);
	if( who+1 > count.size())
		for( int i= count.size(); i <= who+1; i++)
			count.push_back(0.F);
	count[who] = what;
	return true;
}

bool fuzzy_category::add( int who)
{
	if( count.size() < 1) count.push_back(0.F);
	if( who+1 > count.size())
		for( int i= count.size(); i <= who+1; i++)
			count.push_back(0.F);
	count[who] += 1.0F;
	return true;
}

int fuzzy_category::most_likely()
{
	float max;
	if( count.size() < 1) count.push_back(0.F);
	max = count[0];
	int imax;
	imax = 0;
	for( int i= 1; i < count.size(); i++)
		if( count[i] > max) {max = count[i]; imax = i;}
	return imax;
}

float fuzzy_category::pml()
{
	float max,sum;
	if( count.size() < 1) count.push_back(0.F);
	max = sum = count[0];
	int imax;
	imax = 0;
	for( int i= 1; i < count.size(); i++)
	{
		if( count[i] > max) {max = count[i]; imax = i;}
		sum += count[i];
		
	}
	if( sum > 0.F) return max/sum;
	return 0.F;
}

bool fuzzy_category::dump( int my_layer, FILE *op)
{
	for( int i=0; i< count.size(); i++)
	{
		fprintf(op,"fuzzy %d %d %f ; \n", my_layer, i, count[i]);
	}
	return true;
}

bool fuzzy::add( int which, int what)
{
	if( Z.size() < 1) Z.push_back(new fuzzy_category );
	if( which+1 > Z.size())
		for( int i= Z.size(); i <= which+1; i++)
			Z.push_back(new fuzzy_category );
	Z[which]->add(what) ;
	return true;
}

bool fuzzy::clear()
{
	for( int i=0; i< Z.size(); i++)
		Z[i]->init_count();
	return true;
}

float fuzzy::probability(int which)
{
	if( Z.size() < which+1) return 0.F;
	return Z[which]->pml();
}

int fuzzy::expected_value(int which)
{
	if( Z.size() < which+1) return -1;
	return Z[which]->most_likely();
}


	

