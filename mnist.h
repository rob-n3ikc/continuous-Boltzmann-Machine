// definition of the data block
#include <cstdio>
// the standard data (csv) is 28x28
#define WIDTH 28
class mnist{
FILE *ip;
public: // I'm using this to organize functions, nothing oddball
mnist(FILE*);
int id; // what it is
int raw[WIDTH*WIDTH];// as read
int threshed[WIDTH*WIDTH];// converted to -1,1
int recon[WIDTH*WIDTH];
float reconstructed[WIDTH*WIDTH];
// xpm stuff
int *image_buffer;
int image_width,image_height;

bool get_next(); // get the next
bool get_next(int); // get the next that matches
bool rewind_me();
bool load(FILE*);
bool threshold( int);

bool xpm_raw(FILE*);
bool xpm_reconstruct(FILE*);
bool xpm_threshold(FILE*);
bool xpm_multi_header(FILE*, int,int); 
bool xpm_multi_add(int,int,int);
bool xpm_multi_close(FILE*);
};
