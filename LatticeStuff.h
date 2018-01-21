#pragma once

#include "Headers.h"

void create_Bmat_bimodal(std::vector <int>& Bmat);

void create_Bmat_gaussian(std::vector <int>& Bmat,int del);

void create_Exmat(std::vector < std::vector <int> >& Exmat, std::vector <int> & latt);

void create_CapacityMat(std::vector < std::vector <int> >& CapacityMat, std::vector < std::vector <int> >& Exmat);

void create_Wmat(std::vector < int >& Wmat, std::vector < std::vector <int> >& CapacityMat, std::vector <int >& BMat, int del);

void create_Augumented_CapacityMat(std::vector < int >& Wmat, std::vector < std::vector <int> >& CapacityMat);

void create_Residual_graph(std::vector < std::vector <int> >& CapacityMat, std::vector < std::vector <int> >& flow);

void printMatrix(std::vector <std::vector <int> > const & M, int len);

void printMatrix(std::vector<int> const & M, int len);

