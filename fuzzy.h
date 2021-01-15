//
// fuzzy number support
//
//  type one fuzzy - category uncertain
//
// we'll use FILE*
// Lump it.

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>


class fuzzy_category{
public:
std::vector<float> count;

bool init_count();
bool load( int, float); // loads a count
bool dump(int, FILE *); // what's my layer? where do I write?
bool add( int); // add a count to count[i]
int  most_likely();
float pml(); // return the most likely's probability
// NOT NEEDED count is public, bool prob(int, float*); // return the probabilities
};



class fuzzy{
public:
//fuzzy_category *Z;

std::vector<fuzzy_category *> Z;

bool add(int, int);
float probability(int);
int  expected_value(int);
bool clear();

};

