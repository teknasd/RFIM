#pragma once

#include "Headers.h"

void push(std::vector <std::vector <float> >const& C1, std::vector <std::vector <float> >& F1, std::vector <float>& excess1, int u, int v);

void relabel(std::vector <std::vector <float> >const & C, std::vector <std::vector <float> >& F, std::vector <int>& height, int u);

void discharge(std::vector <std::vector <float> >const & C, std::vector <std::vector <float> >& F, std::vector <float> & excess, std::vector <int> & height, std::vector <int>& seen, int u);

void moveToFront(int i, std::vector <int>& A);

void pushRelabel(std::vector <std::vector <float> >const& C, std::vector <std::vector <float> >&  F, int source, int sink);
