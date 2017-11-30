# RFIM
This project is to find the ground state and to show how ferromagnetic square lattice converts to paramagnetic square lattice with increase in magnetic disorder,and to find the ground state in random field ising model using min-cut maxx-flow theorem (push-relabel) and H.k clustering method.


## how to run ?
g++ -o main RFIM_2D_main.cpp LatticeStuff.cpp PushRelabel.cpp clustering.cpp -Wall


## how to change parameters?
edit the parameters (lattice size, no of configurations, range of delta's) in parameters.h file.


### Here's GIF of the clusters of size 40 x 40.
(this result is produced openCV)
![ScreenShot](/clusters-40X40.gif)




