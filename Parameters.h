#pragma once

//* DEFINE ALL THE PARAMETERS HERE */
#define iter		10
#define VER		    10		 //  width of lattice matrix. for ex 6*6 lattice
#define DIM         2		// dimension of lattice // here 2; square lattice
#define latt_pc     1		 // percentage of lattice points where atom exist
#define upspin_pc   0.5		 // percentage of upspin in lattice
#define w           1        // omega(w) for bimmodal distribution of Bi in eq 6.1    
#define N			VER * VER   // width of edge matrix. for ex 36*36 edges
#define V			N+2       // source + width of edge matrix + sink
#define J			1		
#define INFINITE 100000
#define HIGH INFINITE

//delta is multipling factor with Bmat
#define del_beg     0
#define del_end     40
#define del_inc     2




// text substitute
#define tab			"\t"
#define d			" : "

// used in push relabel
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))