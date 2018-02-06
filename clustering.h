#pragma once

#include "Headers.h"

void dfs(std::vector <std::vector <float> >& F, long s, std::vector <int>& visited);
//
int cluster(std::vector<std::vector<int> >& lat, int key, std::vector <int>& clusstats);
//
void createAgumentedMatrix(std::vector < std::vector <int> >& sqlat0, std::vector < std::vector <int> >& sqlat1, std::vector <int> & visited);
//
int gotoindex(std::vector <int>& A, int index);
//
void printCluster(std::vector<std::vector<int> >& lat, std::vector <int>& clusstats);

//
long no_of_clusters(std::vector <int>& clusstats0, std::vector <int>& clusstats1);

//
float rms_mag(std::vector <int> & visited);