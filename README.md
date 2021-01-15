# continuous-Boltzmann-Machine
C++ code for the continuous versions of the boltzmann machine

This version uses an orthogonal basis to greatly enhance the performance.   It will work with the Fourier basis as well as a delta function basis (One hot).  The Chebeshev basis in the original code was good enough for classification, but not great otherwise. 

The orthog structure is improved and corrected. The RBM actually works in it rather than the storage in rbm.  
