#pragma once

#include "Headers.h"

void push(std::vector <std::vector <int> >const& C1, std::vector <std::vector <int> >& F1, std::vector <int>& excess1, int u, int v);

void relabel(std::vector <std::vector <int> >const & C, std::vector <std::vector <int> >& F, std::vector <int>& height, int u);

void discharge(std::vector <std::vector <int> >const & C, std::vector <std::vector <int> >& F, std::vector <int> & excess, std::vector <int> & height, std::vector <int>& seen, int u);

void moveToFront(int i, std::vector <int>& A);

void pushRelabel(std::vector <std::vector <int> >const& C, std::vector <std::vector <int> >&  F, int source, int sink);
