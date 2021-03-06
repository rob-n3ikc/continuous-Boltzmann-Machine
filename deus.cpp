#include "deus.h"

//float chebyshev( int n, float x)
float chebyshev( int n, int j, int s)
{
  float x;
  x = (float)j/(s-1);
// x in range -1 to 1
// build generates 0 to 1
// adjust range
	x = x+x -1.F;
	return cos( n * acos(x) );
}

//float sincos( int n, float x)
float sincos( int n, int j, int s)
{
	float x;
	x = (float)j/s;
	int i;
	i = n/2;
	if( n%2 == 1) return sin(2*3.14159265*i*x);
	 return cos(2*3.14159265*i*x);
}

float delta(int n, int j , int s)
{
	if( n == j) return 1.;
	return 0.;
}


RBM::RBM(int nv, int nh)
{
    nhid = nh; nvis = nv; // shadows
   all_initialized = false;
   hidden = new float[nh];
   normal = new float[nh*nh];
   visible = new float[nv];
   layers = new float*[nh];
   initialized = new bool[nh];
   U = new float[nh];
   gradH = new float[nh];
   gradV = new float[nv];
   for( int i=0; i< nh; i++)
	{
           layers[i] = new float[nv];
           initialized[i] = false;
	}
}
RBM::~RBM()
{

	for( int i=0; i< nhid; i++)
                delete[] layers[i];
	delete[] gradV;
	delete[] gradH;
        delete[] U;
        delete[] initialized;
        delete[] layers;
        delete[] visible;
        delete[] normal;
        delete[] hidden;
}

void RBM::train( float *data, int ds, float beta, float learning_rate)
{
       if( !all_initialized)
	{
          for( int i=0; i< nhid; i++)
		if( !initialized[i])
		{
			initialized[i] = true;
                        float *w;
			w = layers[i];
#define use_data
#ifdef use_data
                        for( int j=0; j< nvis; j++)
				w[j] = data[j];	
#else
                      for( int j=0; j< nvis; j++)
				w[j] =  0.0F;	
		      w[i%nvis] = 1.0F;
#endif
	//		   w[i%nvis] = 1.0F;
	//		   w[(i+1)%nvis] = 1.0F;
			return ;
		}
	   all_initialized = true; // don't get here until everyone is initialized.
	}
        set_H_values( data,ds);

#define expected_value
#ifdef expected_value
	float eup, edown, emid;
// figure out the expected value of dw
	eup = edown = emid = 0.0F;
        for( int j = 0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
		{
			float dw;
			dw = hidden[j]*data[i]*learning_rate;
			emid += row[i]*data[i]*hidden[j];
			eup += (row[i]+dw)*data[i]*hidden[j];
			edown += (row[i]-dw)*data[i]*hidden[j];
		}//i
	}//j
// the magic here is an integral over the degrees of freedom.
                float Z,dP;
		Z = exp(-beta*emid)+exp(-beta*eup) + exp(-beta*edown);
		dP = 3.0F*exp(-beta*eup)/Z ; // 3. normalizes it
                learning_rate = learning_rate*(1.0F - dP);
#endif
	for( int j=0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
		{
			float dw;
			dw = hidden[j]*data[i]*learning_rate;
			row[i] += dw;
		}//i
	}//j
} // train

void RBM::train( float *data, int ds, float beta, float learning_rate, float forget)
{
       if( !all_initialized)
	{
          for( int i=0; i< nhid; i++)
		if( !initialized[i])
		{
			initialized[i] = true;
                        float *w;
			w = layers[i];
#define use_data
#ifdef use_data
                        for( int j=0; j< nvis; j++)
				w[j] = data[j];	
#else
                      for( int j=0; j< nvis; j++)
				w[j] =  0.0F;	
		      w[i%nvis] = 1.0F;
#endif
	//		   w[i%nvis] = 1.0F;
	//		   w[(i+1)%nvis] = 1.0F;
			return ;
		}
	   all_initialized = true; // don't get here until everyone is initialized.
	}
        set_H_values( data,ds);

#define expected_value
#ifdef expected_value
	float eup, edown, emid;
// figure out the expected value of dw
	eup = edown = emid = 0.0F;
/*
        for( int j = 0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
		{
			float dw;
			dw = hidden[j]*data[i]*learning_rate;
			emid += row[i]*data[i]*hidden[j];
			eup += (row[i]+dw)*data[i]*hidden[j];
			edown += (row[i]-dw)*data[i]*hidden[j];
		}//i
	}//j
*/

// the magic here is an integral over the degrees of freedom.
                float Z,dP;
		Z = exp(-beta*emid)+exp(-beta*eup) + exp(-beta*edown);
		dP = 3.0F*exp(-beta*eup)/Z ; // 3. normalizes it
                learning_rate = learning_rate*(1.0F - dP);
#endif
	for( int j=0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
		{
			float dw;
			dw = hidden[j]*data[i]*learning_rate -row[i]*forget;
 
			row[i] += dw;
		}//i
	}//j
} // train






void RBM::train( float *data, int ds, float beta, float learning_rate, float forget, orthog *basis)
{
// note that gradH isn't the return of set_H_values here
// it's gradV
#define use_initialization_with_orthog
#ifdef use_initialization_with_orthog
       if( !all_initialized)
	{
          for( int i=0; i< basis->nhidden; i++)
		if( !initialized[i])
		{
			initialized[i] = true;
                        float *w;
		//	w = layers[i];
// expansion has the saved expansions nhidden many of them
			w = basis->expansion[i];
// old definition
/*
                        for( int j=0; j< nvis; j++)
				w[j] = data[j];	
*/
			for( int j=0; j < basis->nbasis; j++)
				w[j] =  dot(data, basis->basis[j], nvis)/basis->normalize[j];
			return ;
		}
	   all_initialized = true; // don't get here until everyone is initialized.
	}
#endif
// set_H_values does the expansion and sets up gradH
        set_H_values( data,ds,basis);

#define expected_value
#ifdef expected_value
	float eup, edown, emid;
// figure out the expected value of dw
	eup = edown = emid = 0.0F;
	float *row;
	row = basis->expansion[basis->closest];
//	for( int i=0; i<nhid ; i++)
	for( int i=0; i<nvis ; i++)
	{
		float dw;
//		dw = gradH[i]*learning_rate;
		dw = gradV[i]*learning_rate;
		emid += basis->working_expansion[i]*row[i];
		edown += basis->working_expansion[i]*(row[i]-dw);
		eup += basis->working_expansion[i]*(row[i]+dw);
	}//i
// the magic here is an integral over the degrees of freedom.
                float Z,dP;
		Z = exp(-beta*emid)+exp(-beta*eup) + exp(-beta*edown);
		dP = 3.0F*exp(-beta*eup)/Z ; // 3. normalizes it
                learning_rate = learning_rate*(1.0F - dP);
#endif
//	for( int i=0; i< nhid; i++)
	for( int i=0; i< nvis; i++)
	{
		float dw;
//		dw = gradH[i]*learning_rate -row[i]*forget;
		dw = gradV[i]*learning_rate -row[i]*forget;
		row[i] += dw;
	}//i
} // train



// m is a row*row matrix, v the vector,s the solution, and toler
// the criterion for solution.
float gs( float *m, float *v,float *s, int row, float toler, int maxit)
{
	for( int i=0; i< row; i++)
		s[i] = v[i]/m[i*row+i];
	float error;
	for( int iter=0; iter< maxit; iter++) 
	{
	// per row update s
	error = 0.0F;
	for( int i=0; i< row; i++)
	{
		int ir;
		ir = i*row;
		int iir;
		iir = ir + i;
		float sum;
		sum = 0.0F;
		for( int j=0; j< row; j++)
		{
// this continue is wrong  j==i 
	//		if( j == iir) continue;
			if( j == i) continue;
			sum += s[j]*m[ir+j];
		}
		error += fabs( s[i]*m[iir]+sum - v[i]);
		s[i] = (v[i]-sum)/m[iir];
//		s[i] = 0.5*s[i] + 0.5*(v[i]-sum)/m[iir];
	}
		
	if( error < toler) return error;
	}
	return error;
}

/*  this is a routine to solve a linear equation by
    guassian elimination.  (basically solve.for translated) */
/* in order to have the  array matrix be of any length it must be passed as
   a linear array.  Since C has the opposite convention for array packing from 
   FORTRAN ( row fastest rather than column fastest) the leading dimension
   ilead is the row size of the array to which matrix points */
int solve( float *matrix,float *vect,int irow,int ilead )
{

    float quotient;
    int i,j,k;
    int  mpi,mpj,mpk;
    mpi = 0;
    for ( i=0 ; i < irow - 1 ; i++ )
    {  /* for each row */
        j = i ;
        mpj = mpi;
        while ( matrix[mpi + i] == 0)
        {
            if( j == irow )
            { return (-1); }
            j ++;
            mpj += ilead;
            vect[i] +=  vect[j];
            for (k = i; k <irow  ; k++)
            {matrix[mpi + k] += matrix[mpj +k]; }
        }
        /* if here then the diagonal element is not zero so we can do the division*/
        mpj = mpi +ilead ;
        for ( j= i+1; j < irow ; j++ )
        {
            if( matrix[mpj + i] != 0 )
            {
                quotient = matrix[mpj + i]/matrix[mpi + i];
                vect[j] -= vect[i]*quotient;
                for ( k=i ; k< irow ; k++ )
                { matrix[mpj + k] -= matrix[mpi + k]*quotient; }
            }  /* if */
            mpj += ilead;
        } /* for j */
        mpi += ilead;
    } /* for i */

    /* now start the back substitution loop */
    mpi = 0;
    for ( i = 0; i < irow - 1 ; i++ )
    {
        k = irow - i - 1;
        mpj= 0;
        mpk =  k*ilead;
        for ( j = 0; j < k ; j++)
        { vect[j] -= matrix[mpj+k]/matrix[mpk+k]*vect[k];
            mpj +=ilead; }
    } /* i */

    /* and finally divide by the diagonal elements */
    mpi = 0;
    for ( i=0; i <irow ; i++ )
    { vect[i] /= matrix[mpi + i];
        mpi += ilead;    }
    return (0);
}


void RBM::set_H_values( float *data, int ds, orthog *basis)
{
	for(int i=0; i < basis->nbasis; i++)
		basis->working_expansion[i] = dot(data, basis->basis[i], ds)/basis->normalize[i];
	basis->closest = basis->select(basis->working_expansion);
	float *b;
//	float tgh[nvis];
	b = basis->expansion[basis->closest];
// gradV is the difference in the basis space
	for( int i=0; i< basis->nbasis; i++)
		gradV[i] = basis->working_expansion[i]-b[i];
	//	gradH[i] = basis->working_expansion[i]-b[i];
	//	tgh[i] = basis->working_expansion[i]-b[i];
/*
	float *row;
	row = basis->expansion[basis->closest];
	for( int j=0; j< ds; j++)
		gradV[j] = +data[j];
	for( int j=0; j < basis->nbasis; j++)
	{
		float *b;
		b = basis->basis[j];
		for( int i=0; i< nvis; i++)
		{
		gradV[i] -= b[i]*row[j];
		}//i
	}//j
	for(int i=0; i < basis->nbasis; i++)
	{
		float *b;
		b = basis->basis[i];
		gradH[i] = dot(gradV, basis->basis[i], ds)/basis->normalize[i];
	       printf("%f %f %f\n", data[i],gradH[i], tgh[i]);
	}
*/
// gradient is calculated
	
}

void RBM::set_H_values( float *data, int ds)
{
#define lsq
#ifdef lsq
// the original would only work if weight matrix was orthogonal
// and even then it was sub-optimal (to put it mildly.)	
// build the normal equations.
	for( int i=0; i< nhid*nhid; i++)
		normal[i] = 0.0F;
/*
*/
	for( int i=0; i< nhid; i++)
	{
		gradH[i] = 0.0F;
	//	normal[i*nhid+i] = 100.0F; // Tikionov regularization
//		normal[i*nhid+i] = 10.0F; // Tikionov regularization
//STANDARD		normal[i*nhid+i] = 1.0F; // Tikionov regularization
//		normal[i*nhid+i] = 0.001F; // Tikionov regularization
		normal[i*nhid+i] = 0.1F; // Tikionov regularization
	}
// build the normal matrix.
	for( int i = 0; i< nhid; i++)
	{
	float *row;
	row = layers[i];
	for( int k=0; k < nvis; k++)
		gradH[i] += data[k]*row[k];
// matrix is symmetrical
	for( int j = i; j< nhid; j++)
	{
		float *row2;
		row2 = layers[j];
		for( int k=0; k < nvis; k++)
			normal[i*nhid+j] += row[k]*row2[k];
		normal[j*nhid+i] = normal[i*nhid+j];
	}}// outer i,j
/*
	for( int i=0; i< nhid; i++)
	{
	for( int j=0; j< nhid; j++)
		printf("%f ",normal[i*nhid+j]);
		printf("\n");
	}
		printf("\n");
	for( int i=0; i< nhid; i++)
		printf("%f ",gradH[i]);
		printf("\n");
		printf("\n");
*/
//solve( float *matrix,float *vect,int irow,int ilead )
#ifdef exact
	solve( normal, gradH, nhid,nhid);
	for( int j=0; j< nhid; j++)
		hidden[j] = gradH[j];
#else
#ifdef TEST_VERSION
	float e;
	e= gs( normal, gradH, hidden,nhid, 1.e-5, nhid*1000);
	printf("%f\n",e);
#else
	 gs( normal, gradH, hidden,nhid, 1.e-0, nhid*1000);
#endif
#endif

/*
	for( int i=0; i< nhid; i++)
	{
	for( int j=0; j< nhid; j++)
		printf("%f ",normal[i*nhid+j]);
		printf("\n");
	}
		printf("\n");
	for( int i=0; i< nhid; i++)
		printf("%f ",gradH[i]);
		printf("\n");
		printf("\n");
*/
// testing
	
//#define TEST_VERSION
#ifdef TEST_VERSION
	for( int i=0; i< nvis; i++)
		gradV[i] = 0.0F;
	for( int j = 0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
			gradV[i] += row[i]*hidden[j];
	}
	float error;
	error = 0.0F;
	for( int i=0; i< nvis; i++)
		error += fabs( gradV[i]-data[i]);
	printf("%f Error\n", error);
#endif

#else
	for(int i=0; i< nhid; i++)
		gradH[i] = 0.0F;

        for( int j=0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
			gradH[j] += data[i]*row[i];
	}//j
//	for( int j=0; j< nhid; j++)
//		hidden[j] = gradH[j]/nvis;
//	return;
// gradient is calculated
// estimate the step size;
	int im,ip;
	im = ip = 0;
	float delta;
	for( int j=1; j< nhid; j++)
	{
		if( gradH[j] > gradH[ip]) ip = j;
		if( gradH[j] < gradH[im]) im = j;
	}
	if( fabs(gradH[im]) > gradH[ip])
		delta =  fabs(1.0F/gradH[im]); // keep the sign
        else
		delta =  1.0F/gradH[ip];
	for( int j=0; j < nhid; j++)
		hidden[j] = gradH[j]*delta;
#endif
}// set_H_values

float dot( float *x, float *y, int n)
{
	float rv;
	rv = 0.0F;
	for( int i=0; i<n; i++)
		rv += x[i]*y[i];
	return rv;
}//dot


float RBM::dot( float *x, float *y, int n)
{
	float rv;
	rv = 0.0F;
	for( int i=0; i<n; i++)
		rv += x[i]*y[i];
	return rv;
}//dot

float *RBM::energy( float *data, int ds)
{
	for( int j=0; j< nhid; j++)
		U[j] = 0.0F;
	set_H_values(data,ds);
	float enorm;
	for( int j=0; j< nhid; j++)
	{
		enorm = 0.0F;
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
		{
			U[j] += row[i]*data[i]*hidden[j];
		}
	}//j
	return U;
}//energy

//graham_schmidt forces the rows of w to be orthogonal
// this can improve accuracy and robustness.
void RBM::graham_schmidt()
{
	if( !all_initialized) return;
	float w2,x2,xw;
	float *w,*x;
	for( int i=0;i < nhid-1; i++)
	{
		w = layers[i];
		w2 = dot(w,w,nvis);
		for( int j=i+1; j< nhid; j++)
		{
			x  = layers[j];
			x2 = dot(x,x,nvis);
			xw = dot(x,w,nvis);
			xw = xw/sqrt(w2*x2);
			for(int k=0; k < nvis; k++)
				x[k] -= xw*w[k];
		}
	}//i	
}// graham_schmidt


float square_error( float *a, float *b, int ds)
{
	float retval;
	for( int i=0; i< ds; i++)
		retval += (a[i]-b[i])*(a[i]-b[i]);
	return retval;
}
float correlation( float *a,float *b, int ds)
{
	float am,bm;
	am = bm = 0;
	for( int i=0; i< ds; i++)
	{ am+= a[i]; bm += b[i];}
	am /= ds; bm /= ds;
	float ccrr,ccro,ccoo;
	ccrr = ccro = ccoo = 0.0F;
	for( int i=0; i< ds; i++)
	{
		ccrr += (a[i]-am)*(a[i]-am);
		ccro += (a[i]-am)*(b[i]-bm);
		ccoo += (b[i]-bm)*(b[i]-bm);
	}
	return ccro /sqrt(ccrr*ccoo);
}
float RBM::score(float *data, int ds)
{
// this is mnist specific and should be moved there.
	float cmax;
	cmax = 0.0F;
	set_H_values(data,ds);

	for(int i=0; i< nvis; i++)
		gradV[i] = 0.0F;

	for( int j=0; j<nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
			gradV[i] += hidden[j]*row[i];
	}

/*
	float error;
	error = 0.0F;
//	for( int i=0; i< nvis; i++)
//		error += fabs(data[i]-gradV[i]);
	for( int i=0; i< nvis; i++)
		error += (data[i]-gradV[i])*(data[i]-gradV[i]);
	return -error;
*/

	return correlation(data,gradV,nvis);
}



float RBM::reconstruct( float *data, int ds, orthog *basis)
{
	set_H_values(data,ds,basis);
//        for( int j=0; j< nhid; j++)
//	printf("%f ",hidden[j]);
//	printf("\n"); fflush(stdout);
////        for( int j=0; j< nhid; j++)
////		if( hidden[j] < 0.99) hidden[j] = 0.0F;

	for(int i=0; i< nvis; i++)
		gradV[i] = 0.0F;
	float *row;
	row = basis->expansion[basis->closest];
	for( int j=0; j < basis->nbasis; j++)
	{
		float *b;
		b = basis->basis[j];
		for( int i=0; i< nvis; i++)
		{
		gradV[i] += b[i]*row[j];
		}//i
	}//j
// gradient is calculated
// estimate the step size;
//	return correlation(data,gradV,nvis);
	float ccoo,ccrr,ccro;
	ccoo = ccrr = ccro = 0.0F;
	
//	return correlation(data,gradV,nvis);
	ccoo = dot(data,data,nvis);
	ccrr = dot(gradV,gradV,nvis);
	ccro = dot(gradV,data,nvis);
	return ccro/sqrt(ccoo*ccrr);
//	return ccro*ccro/(ccoo*ccrr);
}// reconstruct


float RBM::reconstruct( float *data, int ds)
{
	set_H_values(data,ds);
//        for( int j=0; j< nhid; j++)
//	printf("%f ",hidden[j]);
//	printf("\n"); fflush(stdout);
////        for( int j=0; j< nhid; j++)
////		if( hidden[j] < 0.99) hidden[j] = 0.0F;

	for(int i=0; i< nvis; i++)
		gradV[i] = 0.0F;
        for( int j=0; j< nhid; j++)
	{
		float *row;
		row = layers[j];
		for( int i=0; i< nvis; i++)
			gradV[i] += hidden[j]*row[i];
	}//j
// gradient is calculated
// estimate the step size;
//	return correlation(data,gradV,nvis);
	float ccoo,ccrr,ccro;
	ccoo = ccrr = ccro = 0.0F;
	
//	return correlation(data,gradV,nvis);
	ccoo = dot(data,data,nvis);
	ccrr = dot(gradV,gradV,nvis);
	ccro = dot(gradV,data,nvis);
	return ccro/sqrt(ccoo*ccrr);
//	return ccro*ccro/(ccoo*ccrr);
}// reconstruct


void RBM::random_init( int seed)
{
// the code from really_lame_rng
float buff[55];
float xva;
int ip,jp,kp;
for( ip=0; ip< 55; ip++)
{
// a lousy rng to initialize
    seed = (seed*2349+14687)%32767;
    buff[ip] = (float)seed/32767.0F;
    while( buff[ip] > 1.F) buff[ip] -= 1.0F;
    while( buff[ip] < 0.F) buff[ip] += 1.0F;
}
ip = 23; jp = 24; kp = 0;

for( int ih = 0; ih < nhid; ih++)
{
	float *w;
	w = layers[ih];
	for( int iv = 0; iv < nvis; iv++)
	{

	xva = buff[jp]+buff[ip];
	while( xva > 1.F) xva -= 1.F;
	buff[kp] = xva;
	ip = (ip+1)%55;
	jp = (jp+1)%55;
	kp = (kp+1)%55;
	w[iv] = xva;
	}
	initialized[ih] = true;
}

}// end of random_init




Kmeans::Kmeans( int nlong, int ns)
{
	nnodes = ns;
	node_length = nlong;

	nodes = new float*[ns];
	accumulator = new float*[ns];
	initialized = new bool[ns];
	inaccumulate = new float[ns];
	for( int i=0; i< ns; i++)
	{
		nodes[i] = new float[nlong];
		accumulator[i] = new float[nlong];
		initialized[i] = false;
		for( int j=0; j< node_length; j++)
			(accumulator[i])[j] = 0.0F;
		inaccumulate[i] = 0.0F;
	}//i	
	
        all_initialized = false;
}

Kmeans::~Kmeans()
{
	for( int i=0; i< nnodes; i++)
	{
		delete[] accumulator[i];
		delete[] nodes[i];
	}
	delete[] inaccumulate;
	delete[] initialized;
	delete[] accumulator;
	delete[] nodes;
}

float Kmeans::distance( float *a, float *b, int n)
{
	float retval;
	retval = 0.0F;
	for( int i=0; i< n; i++)
		retval += (a[i]-b[i])*(a[i]-b[i]);
	return sqrt(retval/n);
}

void Kmeans::train( float *data, int ds)
{
	
	if( !all_initialized)
	{
		for( int i=0; i< nnodes; i++)
		{
			if(initialized[i]) continue;
			initialized[i] = true;
			for( int j=0; j< node_length; j++)
				(nodes[i])[j] = data[j];
			(nodes[i])[i%node_length] = 1.0F;
			if( i == nnodes-1) all_initialized = true;
			return;
		}
	}
/*
	printf("%d %d\n", nnodes, node_length);
	for( int j=0; j< node_length; j++)
		printf("%f ",data[j]);
	printf("\n");
	for( int i=0; i < nnodes; i++)
	{
		for( int j=0; j< node_length; j++)
			printf("%f ",(accumulator[i])[j]);
		printf("\n");
	}
*/
	float error, emin;
	int imin;
	imin = 0;
	emin = distance( nodes[0],data,node_length);
	for( int i=1; i< nnodes; i++)
	{
	error = distance( nodes[i],data,node_length);
	if( error < emin){ emin = error; imin = i;}
	}//i
//	printf("%f %d\n",emin,imin);
	for( int i=0; i< node_length; i++)
		(accumulator[imin])[i] += data[i];
	inaccumulate[imin] += 1.0F;	

/*
	for( int i=0; i < nnodes; i++)
	{
		for( int j=0; j< node_length; j++)
			printf("%f ",(accumulator[i])[j]);
		printf("\n");
	}
*/

}// Kmeans::train
// this takes the nodes data and updates the centroids based on the
// values in accumulator
void Kmeans::summarize()
{
//	printf("summarize %d %d\n", nnodes, node_length);fflush(stdout);
	for( int i=0; i < nnodes; i++)
	{
//		printf("%d %f\n", i, inaccumulate[i]); fflush(stdout);
		if( inaccumulate[i] < 1.0F) continue;// shouldn't happen, but wth
		float *r, *rr;
		r = nodes[i];
		rr = accumulator[i];
		for( int j=0; j< node_length; j++)
		{
//			printf("%f ",rr[j]);
			r[j] = rr[j]/inaccumulate[i]; 
		}
//		printf("\n");
	}
	for( int i=0; i < nnodes; i++)
	{
		inaccumulate[i] = 0.0F;
		for( int j=0; j < node_length; j++)
			(accumulator[i])[j] = 0.0F;
	}
}

float Kmeans::score( float *data, int ds)
{
	float error, emin;
	int imin;
	imin = 0;
	emin = distance(data, nodes[0], ds);
	for( int i=1; i< nnodes; i++)
	{
	error = distance(data, nodes[i], ds);
	if( error <emin){ imin = i; emin = error;}
	}
	return emin;
}

int Kmeans::which( float *data, int ds)
{
	float error, emin;
	int imin;
	imin = 0;
	emin = distance(data, nodes[0], ds);
	for( int i=1; i< nnodes; i++)
	{
	error = distance(data, nodes[i], ds);
	if( error <emin){ imin = i; emin = error;}
	}
	return imin;
}

Knn::Knn(int w, int maxn)
{
	width = w; max_nodes = maxn;
	nodes = new float* [maxn];
	categories = new int[maxn];
	in_nodes = 0;
	for( int i=0; i< maxn; i++)
		nodes[i] = new float[width];
}

Knn::~Knn()
{
	for(int i=0; i< max_nodes; i++)
		delete [] nodes[i];
	delete [] nodes;
	delete [] categories;
}

float Knn::distance( float *a, float *b, int n)
{
	float retval;
	retval = 0.0F;
	for( int i=0; i< n; i++)
		retval += (a[i]-b[i])*(a[i]-b[i]);
	return sqrt(retval/n);
}

void Knn::train( float *data, int ds, int cat)
{
	if( in_nodes < max_nodes)
	{
		for( int j=0; j< width; j++)
			(nodes[in_nodes])[j] = data[j];
		categories[in_nodes++] = cat;
		return;
	}
	float dmin_same, dmin_any;
	int imin_same, imin_any;
	dmin_same = dmin_any = 10.e10;
	imin_same = -1;
	for( int i=0;i < in_nodes; i++)
	{
		float r;
		r = distance( nodes[i], data, width);
		if( categories[i] == cat && r < dmin_same)
		{
			dmin_same = r;
			imin_same = i;
		}
		if(  r < dmin_any)
		{
			dmin_any = r;
			imin_any = i;
		}
	}//i
	if( dmin_any < dmin_same && imin_same >= 0)
	{
		for(int j=0; j< width; j++)
			(nodes[imin_same])[j] = data[j];
	}
// if I didn't find any make one. (handles minority candidates)
	if( imin_same < 0) 
	{ categories[imin_any] = cat;
		for(int j=0; j< width; j++)
			(nodes[imin_any])[j] = data[j];
	}
// don't update one where the closest is already the correct kind
}

int Knn::category( float *data, int ds, int cat)
{
	float dmin; 
	int imin;
	imin = 0;
	dmin = distance(nodes[0], data, width);
	for( int i=1;i < in_nodes; i++)
	{
		float r;
		r = distance(nodes[i],data,width);
		if( r < dmin){ dmin = r; imin = i;}
	}
	
// improvement would be to do local average, but lets see how this works
	return categories[imin];
}


// nb is the number of basis functions
// n is the length of the basis function (nvis in the rbm)
// nh is the number of expansion vectors (nhidden in the rbm)
orthog::orthog(int nb, int n, int nh)
{
	size = n;
	nbasis = nb;
	nhidden = nh;
	working_expansion = new float[nbasis];
	basis = new float* [nbasis];
	expansion = new float* [nhidden];
	for( int i=0; i< nbasis; i++)
		basis[i] = new float[size];
	for( int i=0; i< nhidden; i++)
		expansion[i] = new float[nbasis];
	normalize = new float[nbasis];
}


orthog::~orthog()
{
	for( int i=0; i < nhidden; i++)
		delete [] expansion[i];
	for( int i=0; i < nbasis; i++)
		delete [] basis[i];
	delete [] expansion;
	delete [] basis;
	delete [] normalize;
	delete [] working_expansion;
}

bool orthog::build( float (*f)(int , int,int ))
{
	for( int i=0; i < nbasis; i++)
	{// for every row
		float *row;
		row = basis[i];
		normalize[i] = 0.F;
		for( int j=0; j< size; j++)
		{
// size -1 makes the range 0 to 1
			row[j] = f(i, j,size);
			normalize[i] += row[j]*row[j];
		}
        if( normalize[i] == 0.0) normalize[i] = 1.e-14;
	}//i
	for( int i=0; i < nhidden; i++)
	{
		float *row;
		row = expansion[i];
		for( int j=0; j< nbasis; j++)
		{
			row[j] = 0.F;
		}
	}//i
	return 1==1;
}

// test is an nbasis long vector
int orthog::select( float *test)
{
	int ic;
	float rc;
	ic = 0;
#ifdef correlation
	float ccd2;
	rc = -10.e10;
	ccd2 = 0.F;
	for( int i=0; i< size; i++)
		ccd2 += test[i]*test[i];
	for( int i=0; i< nhidden; i++)
	{
		float *row;
		float ccn,ccd1;
		ccn = 0.F; ccd1 = 0.F;
		row = expansion[i]; // expansion has the hidden layers
		for( int j=0; j< size; j++)
		{
			ccn += test[j]*row[j];
			ccd1 += row[j]*row[j];
		}//j
		if( ccd1 < 1.e-7)
			continue;
		ccn = ccn *ccn/ccd1/ccd2;
		if( ccn	 > rc)
			{rc = ccn; ic = i;}
	}//i
#else
	rc = 10.e10;
	for( int i=0; i< nhidden; i++)
	{
	   float ccn;
	   float *row;
           row = expansion[i];
           ccn = 0.F; 
	   for( int j=0; j< size; j++)
	       ccn += (test[j]-row[j])*(test[j]-row[j]);	
		
// this is working code	
//	printf("%f %f\n", ccn,rc);
           if( ccn < rc)
	    {rc = ccn; ic = i;}
	}//i

#endif
	
	return ic;
}


void orthog::test_expansion()
{
// a test function
float *data = new float[size];
float *recon = new float[size];
for( int i=0; i< size; i++)
    data[i] = recon[i] = 0.0F;

data[1] = 1.;
data[2] = -1.;

float *b;
for( int j=0; j< nbasis; j++)
for( int i=j; i< nbasis; i++)
{
  printf("%d %d %f dot product test\n", j,i,dot(basis[j],basis[i],size)); 
}

for( int i=0; i< nbasis; i++)
{
   working_expansion[i] = dot(data, basis[i], size)/normalize[i];
}
for( int j=0; j< size; j++)
 	recon[j] = 0.;
for( int i=0; i< nbasis; i++)
{
 	b = basis[i];
	for( int j=0; j< size; j++)
		recon[j] += working_expansion[i]*b[j];
}

for( int i=0; i < size; i++)
   printf("%d %f %f\n", i, data[i], recon[i]);


delete[] recon;
delete[] data;
}
