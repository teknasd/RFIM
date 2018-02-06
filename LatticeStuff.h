#pragma once

#include "Headers.h"

void create_Bmat_bimodal(std::vector <float>& Bmat,float del);

void create_Bmat_gaussian(std::vector <float>& Bmat, float del);

void create_Exmat(std::vector < std::vector <int> >& Exmat, std::vector <int> & latt);

void create_CapacityMat(std::vector < std::vector <float> >& CapacityMat, std::vector < std::vector <int> >& Exmat);

void create_Wmat(std::vector < float >& Wmat, std::vector < std::vector <float> >& CapacityMat, std::vector <float >& BMat, float del);

void create_Augumented_CapacityMat(std::vector < float >& Wmat, std::vector < std::vector <float> >& CapacityMat);

void create_Residual_graph(std::vector < std::vector <float> >& CapacityMat, std::vector < std::vector <float> >& flow);

void printMatrix(std::vector <std::vector <int> > const & M, int len);

void printMatrix(std::vector<int> const & M, int len);

void savedata(std::vector< int > & Mat, int l, float del, std::string s);

void savedata(std::vector< float > & Mat, int l, float del, std::string s);