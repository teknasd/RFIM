# RFIM
This project is to find the ground state and to show how ferromagnetic square lattice converts to paramagnetic square lattice with increase in magnetic disorder,and to find the ground state in random field ising model using min-cut maxx-flow theorem (push-relabel) and H.k clustering method.

simulation:1 

Lattice size: 40 X 40 

Other parameters for simulation:-

/* ALL THE PARAMETERS HERE */

#define VER		    40	     //  width of lattice matrix. for ex 6*6 lattice

#define latt_pc     1		 // percentage of lattice points where atom exist

#define upspin_pc   0.5		 // percentage of upspin in lattice

#define w           1        // omega(w) for bimmodal distribution of Bi in eq 6.1    

#define N			VER*VER    // width of edge matrix. for ex 36*36 edges

#define V			N+2       // source + sink


//delta is multipling factor with Bmat

#define del_beg     0

#define del_end     40 // actually 4.0

#define del_inc     2 // actually 0.2


result of simulation 1:

red  stands for upspin

blue stands for downspin



![ScreenShot](/cluster-40-1-0.bmp)
w=0.0

![ScreenShot](/cluster-40-1-2.bmp)
w=0.2

![ScreenShot](/cluster-40-1-4.bmp)
w=0.4

![ScreenShot](/cluster-40-1-6.bmp)
w=0.6

![ScreenShot](/cluster-40-1-8.bmp)
w=0.8

Ferromagnetic domain starts to breaks to smaller domains at external magnetic field w=0.8

![ScreenShot](/cluster-40-1-10.bmp)
w=1.0

![ScreenShot](/cluster-40-1-12.bmp)
w=1.2

![ScreenShot](/cluster-40-1-14.bmp)
w=1.4

![ScreenShot](/cluster-40-1-16.bmp)
w=1.6

![ScreenShot](/cluster-40-1-18.bmp)
w=1.8

![ScreenShot](/cluster-40-1-20.bmp)
w=2.0

after w=2.0 all bigger domains breaks into very tiny domains

![ScreenShot](/cluster-40-1-22.bmp)
w=2.2

![ScreenShot](/cluster-40-1-24.bmp) 
w=2.4 

![ScreenShot](/cluster-40-1-26.bmp) 
w=2.6

![ScreenShot](/cluster-40-1-28.bmp) 
w=2.8

![ScreenShot](/cluster-40-1-30.bmp)
w=3.0

![ScreenShot](/cluster-40-1-32.bmp)
w=3.2

![ScreenShot](/cluster-40-1-34.bmp)
w=3.4

![ScreenShot](/cluster-40-1-36.bmp)
w=3.6

![ScreenShot](/cluster-40-1-38.bmp) 
w=3.8

![ScreenShot](/cluster-40-1-40.bmp)
w=4.0




