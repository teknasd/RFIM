#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <random>
#include <crtdbg.h>
#include <Windows.h>
#define WIDTH 7
#define DIV 1024
#define iter 2
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define INFINITE 10000000
#define HIGH INFINITE

/* DEFINE ALL THE PARAMETERS HERE */
#define VER		    5	     //  width of lattice matrix. for ex 6*6 lattice
#define latt_pc     1		 // percentage of lattice points where atom exist
#define upspin_pc   0.5		 // percentage of upspin in lattice
#define w           8        // omega(w) for bimmodal distribution of Bi in eq 6.1    
#define N			VER*VER   // width of edge matrix. for ex 36*36 edges
#define V			N+2       // source + sink

#define tab			"\t"
#define d			" : "
		//delta is multipling factor with Bmat
#define del_beg     0
#define del_end     20
#define del_inc     5
int del = 2;

using namespace std;

/* FUNCTION ALERT : TO CREATE RNDOM LATTICE PTS, IN RATIO TO latt_pc */
int * create_lattice(int * latt)
{
	int sum0 = 0, sum1 = 0;
	int i = 0, var = 0;
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	uniform_int_distribution<int>       distr(0, 100);
	for (i = 0; i < N; i++)
	{
		var = distr(generator);
		if (var % 100 < (latt_pc * 100))
		{
			latt[i] = 1; sum1++;
		}
		else
		{
			latt[i] = 0; sum0++;
		}

	}
	cout << "sum0: " << sum0 << " - sum1: " << sum1 << endl;
	return latt;
}


/* FUNCTION ALERT : TO CREATE BIMODAL DESTRIBUTION FOR LOCAL MAG FIELD*/
int* create_Bmat(int* Bmat)
{
	long i = 0, var = 0, count1 = 0, count2 = 0;
	uniform_int_distribution<int>  distr(0, 1000);
	std::random_device  rand_dev;
	std::mt19937        generator(rand_dev());
	cout << "B mtrix ->" << tab;
	for (i = 0; i < N; i++)
	{
		//Bmat[i] = distr(generator);// floor(rand() / 1000);
		var = distr(generator);
		if (var < 500)
		{
			Bmat[i] = w;
			count1++;
		}
		else
		{
			Bmat[i] = -w;
			count2++;
		}

		//cout << Bmat[i] << d;
	}	cout << endl << "counts : " << count1 << d << count2 << endl;

	return Bmat;
}
/*
*  A DFS based function to find all reachable vertices from s.
*/
int * dfs(int ** F, long s, int *visited)
{

	visited[s] = true;
	for (long i = 0; i < V; i++)
	{
		if (F[s][i] && !visited[i])
		{
			//cout << i<<d;
			dfs(F, i, visited);
		}
	}
	return (visited);
}
void push(const int * const * C, int ** F, int *excess, int u, int v) {
	//cout << "\nflag 2\n";
	int send = MIN(excess[u], C[u][v] - F[u][v]);
	F[u][v] += send;
	F[v][u] -= send;
	excess[u] -= send;
	excess[v] += send;
}

void relabel(const int * const * C, const int * const * F, int *height, int u) {
	int v;
	int min_height = INFINITE;
	for (v = 0; v < V; v++) {
		if (C[u][v] - F[u][v] > 0) {
			min_height = MIN(min_height, height[v]);
			height[u] = min_height + 1;
		}
	}
};

void discharge(const int * const * C, int ** F, int *excess, int *height, int *seen, int u) {
	//cout << "\nflag 2\n";
	while (excess[u] > 0) {
		if (seen[u] < V) {
			int v = seen[u];
			if ((C[u][v] - F[u][v] > 0) && (height[u] > height[v])) {
				push(C, F, excess, u, v);
			}
			else
				seen[u] += 1;
		}
		else {
			relabel(C, F, height, u);
			seen[u] = 0;
		}
	}
}

void moveToFront(int i, int *A) {
	//cout << "\nflag 3\n";
	int temp = A[i];
	int n;
	for (n = i; n > 0; n--) {
		A[n] = A[n - 1];
	}
	A[0] = temp;
}

int pushRelabel(const int * const * C, int ** F, int source, int sink) {
	int *excess, *height, *list, *seen, i, p;
	//cout << "add in fn " << &C[100][70];
	excess = new int[V];
	height = new int[V];
	seen = new int[V];
	list = new int[V - 2];
	for (i = 0; i < V; i++) {
		excess[i] = 0;
		height[i] = 0;
		seen[i] = 0;
		if (i<V - 2)
			list[i] = 0;
	}
	for (i = 0, p = 0; i < V; i++) {
		if ((i != source) && (i != sink)) {
			list[p] = i;
			p++;
		}
	}
	//cout << "\nflag 1\n";
	height[source] = V;
	excess[source] = INFINITE;
	for (i = 0; i < V; i++)
		push(C, F, excess, source, i);

	p = 0;
	while (p < V - 2) {
		int u = list[p];
		int old_height = height[u]; //cout << "\nflag 2\n";
		discharge(C, F, excess, height, seen, u);
		if (height[u] > old_height) {
			moveToFront(p, list);
			p = 0;
		}
		else
			p += 1;
	}
	int maxflow = 0;
	for (i = 0; i < V; i++)
		maxflow += F[source][i];

	delete(list);
	delete(seen);
	delete(height);
	delete(excess);
	cout << "\nflag 2\n";
	return maxflow;
}

void printMatrix(const int * const * M) {
	int i, j;
	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++)
			cout << "\t" << M[i][j];
		cout << "\n";
	}
}

int gotoindex(int A[V - 1], int index)
{
	int k = 0;
	if (A[index] < 0)
	{
		//cout << "\nconflict of" << A[index] << "index changed to" << abs(A[index]) << endl;
		gotoindex(A, abs(A[index]));
	}
	else
	{
		//cout << "\nret: " << index << endl;
		return index;
	}
}

int main(void) {
	

	time_t time_begin, time_end, time_1, time_2;

	long t1, t2, tdiff[100] = { 0 }, l = 0, sum = 0, i, j, cap = 0;
	for (l = 0; l < iter; l++)
	{
		int **flow, **capacities, *visited, **CapacityMat, **Exmat, **sqlat, *Wmat, **Jmat, *latt, *Bmat, *clusstats;

		cout << "\n\n*****ITER:  " << l << "****\n\n";
		//Exmat = (int **)calloc(N, sizeof(int*));
		Exmat = new int *[N];
		Jmat = new int *[N];
		flow = new int *[V];
		sqlat = new int *[VER + 1];
		CapacityMat = new int *[V];
		latt = new int[N];
		Wmat = new int[N];
		Bmat = new int[N];
		visited = new int[V];
		clusstats = new int[V / 2];

		for (i = 0; i < VER + 1; i++)
		{
			sqlat[i] = new int[VER];
		}
		for (i = 0; i < V; i++) {

			flow[i] = new int[V];
			CapacityMat[i] = new int[V];

		}
		for (i = 0; i < N; i++) {
			Exmat[i] = new int[N];
			Jmat[i] = new int[N];

		}


		// some kinnda const
		float J = 1;

		latt = create_lattice(latt);
		//create_lattice(latt);
		for (i = 0; i < N; i++)
		{
			cout << latt[i];
		}
		cout << "latte created" << endl;

		//init
		cout << "matrixex initiated sloly" << endl;
		for (i = 0; i < V; i++)
		{
			for (j = 0; j < V; j++)
			{

				if (i < VER + 1 && j < VER + 1)
				{
					sqlat[i][j] = 0;
				}
				if (i < N && j < N)
				{
					Exmat[i][j] = 0;
					Jmat[i][j] = 1;
				}
				CapacityMat[i][j] = 0;
				flow[i][j] = 0;
			}
			if (i < V / 2)
			{
				clusstats[i] = 0;
			}
			if (i < N)
			{
				Bmat[i] = 0;
				Wmat[i] = 0;
			}visited[i] = 0;

		}

		// pg:111 // existence matrix for edges //  connection from node i(row) to node j(col) 

		cout << "Exmat in making" << endl;
		for (i = 0; i < N; i++)
		{
			if (((i + 1) % VER != 0))
			{

				Exmat[i][i + 1] = 1 * latt[i] * latt[i + 1];
				//cout << Exmat[i][i + 1];
			}
			if (i + VER < N)
			{

				Exmat[i][i + VER] = 1 * latt[i] * latt[i + VER];
			}
			//else 

		}

		cout << "Exmat created" << endl;
		cout << "Exmat printing" << endl;
		/*for (i = 0; i < N; i++)
		{
		for (j = 0; j < N; j++)
		{
		cout << Exmat[i][j]<<" ";
		}
		cout << endl;
		}*/

		// B: local magnetic  fields//fig: 6.5
		cout << "making Bmat" << endl;
		Bmat = create_Bmat(Bmat);
		cout << "created Bmat" << endl;
		cout << "printing Bmat" << endl;
		for (i = 0; i < N; i++)
		{
			//cout << Bmat[i];
		}


		//for (del = del_beg; del < del_end; del += del_inc)
		{

		// W matrix - dont know 

		/*----------------------------------- eq 6.12 pg 110 -------------------------------------*/
		cout << "making CapacityMatrix" << endl;
		for (i = 1; i <= N; i++)
		{
			//cout << "\n";
			for (j = 1; j <= N; j++)
			{
				if (i < j)
				{
					CapacityMat[i][j] = 4 * Jmat[i - 1][j - 1] * Exmat[i - 1][j - 1]*10;

				}cout << CapacityMat[i][j] << " ";
			}
		}
		cout << "created CapacityMat" << endl;
		
	
		//open
		cout << "making Wmat" << endl;
		/*----------------------------------- eq 6.13  pg 110 ---------------------------------*/
		//cout << "w mat from i=1, j=1" << endl;
		for (i = 1; i <= N; i++)
		{
			for (j = 1; j <= N; j++)
			{
				cap += CapacityMat[i][j] - CapacityMat[j][i];
				//cout << cap<<"\t";
			}
			Wmat[i - 1] = -2 * Bmat[i - 1] * del* latt[i - 1] - cap / 2;
			cap = 0;
			cout <<Wmat[i-1]<< endl;
		}

		// ------------------------------- print capacity matrix


		// --------------------------------print Wmatrix
		cout << "w mat from i=1, j=1" << endl;

		/*for (j = 0; j < N; j++)
		{
		cout << Wmat[j] << "\t";
		}*/


		/*----------------------------------- eq 6.14 pg 110 -----------------------------------*/

		for (i = 1; i <= N; i++)
		{
			if (Wmat[i - 1] > 0)
			{
				CapacityMat[0][i] = 0;
				CapacityMat[i][N + 1] = Wmat[i - 1];
			}

			else
			{
				CapacityMat[0][i] = -Wmat[i - 1];
				CapacityMat[i][N + 1] = 0;
			}

		}

		/*----------------------------------------------------------------------------------------*/



		t1 = time(&time_1);  /* get current time;*/
		//cout << "time now:" << t1;
		
		cout << "\nMax Flow:" << pushRelabel(CapacityMat, flow, 0, V - 1);
		t2 = time(&time_2);  /* get current time;*/
		//cout << "\ntime now:" << t2 << "\n\n";

		//printMatrix(flow);

		for (i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				flow[i][j] = CapacityMat[i][j] - flow[i][j];
				//printf("%d\t", capacities[i][j]-flow[i][j]);
			}
			//printf("\n");
		}

		visited = dfs(flow, 0, visited);

		cout << "visited/non visited nodes:";

		//agumented matrix
		for (i = 0; i <= VER; i++)
		{
			sqlat[i][0] = HIGH;
		}
		for (j = 0; j <= VER; j++)
		{
			sqlat[0][j] = HIGH;
		}

		for (i = 1; i <= VER; i++)
		{
			for (j = 1; j <= VER; j++)
			{
				sqlat[i][j] = visited[(i - 1)*VER + (j - 1) + 1];
				if (sqlat[i][j] == 0)sqlat[i][j] = HIGH;
				cout << visited[(i - 1)*VER + (j - 1) + 1];

			}
		}
		cout << "\nsqlat\n";
		for (i = 0; i <= VER; i++)
		{
			for (j = 0; j <= VER; j++)
			{
				if (sqlat[i][j] == HIGH)
					cout << "- ";

				else
					cout << "+ ";
			}cout << endl;
		}

		//clustering

		int m = 0, n = 0, min = 0, a = 0, b = 0;

		for (i = 1; i < VER + 1; i++)
		{

			for (j = 1; j < VER + 1; j++)
			{

				if (sqlat[i][j] == 1)
				{
					a = sqlat[i - 1][j];
					b = sqlat[i][j - 1];
					//cout << "\n\nbw " << a << "(" << i - 1 << "," << j << ") & " << b << " (" << i << "," << j - 1 << " )\n";
					min = MIN(a, b);
					if (a == HIGH && b == HIGH)
					{//rule 1
						//cout << "rule:" << 1 << d;
						m++;
						clusstats[m]++;
						sqlat[i][j] = m;
						//cout << "ret:" << m << d;
					}

					else if ((a == HIGH && b < HIGH) || (a < HIGH && b == HIGH))
					{//rule 2
						//cout << "rule:" << 2 << d;
						min = MIN(a, b);
						n = gotoindex(clusstats, min);
						clusstats[n]++;
						sqlat[i][j] = n;
					}




					else if (a < b)
					{//rule 3
						//cout << "rule:" << 3 << d;
						n = gotoindex(clusstats, a);
						clusstats[n] += clusstats[gotoindex(clusstats, b)] + 1;
						clusstats[b] = -n;
						sqlat[i][j] = n;
					}
					else if (a == b)
					{//rule 4
						//cout << "rule:" << 4 << d;
						n = gotoindex(clusstats, a);
						clusstats[n] += 1;
						sqlat[i][j] = n;
					}
					else
					{//rule 5
						//cout << "rule:" << 5 << d;
						n = gotoindex(clusstats, b);
						clusstats[n] += clusstats[gotoindex(clusstats, a)] + 1;
						clusstats[a] = -n;
						sqlat[i][j] = n;
					}
					/*cout << "\ncluster\n\n";

					for (int ki = 0; ki <= VER; ki++)
					{
					for (int kj = 0; kj <= VER; kj++)
					{
					if (sqlat[ki][kj] == HIGH)
					cout << "- ";

					else
					cout << sqlat[ki][kj] << " ";
					}cout << endl;
					}*/
					/*cout << "clus stats\n";
					for (int k = 0; k < m; k++)
					{
					cout << clusstats[k] << tab;
					}*/
				}
			}

		}

		cout << "\ncluster\n\n";

		for (i = 0; i <= VER; i++)
		{
			for (j = 0; j <= VER; j++)
			{
				if (sqlat[i][j] == HIGH)
					cout << "- ";

				else
					cout << sqlat[i][j] << " ";
			}cout << endl;
		}
		//
		cout << "clus stats\n";
		for (i = 0; i < N; i++)
		{
			cout << clusstats[i] << tab;
		}
		cout << "\n\n\ntime_req : " << t2 - t1;

		tdiff[l] = t2 - t1;
		sum += tdiff[l];

		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		printf(TEXT("\nThere is  %*ld percent of memory in use.\n"),
			WIDTH, statex.dwMemoryLoad);
		printf(TEXT("There are %*I64d total KB of physical memory.\n"),
			WIDTH, statex.ullTotalPhys / DIV);
		printf(TEXT("There are %*I64d free  KB of physical memory.\n"),
			WIDTH, statex.ullAvailPhys / DIV);

		//close
	}
		for (i = 0; i < V; i++) {

			delete(flow[i]);
			delete(CapacityMat[i]);

		}
		for (i = 0; i < N; i++) {
			delete(Exmat[i]);
			delete(Jmat[i]);

		}
		/*for (i = 0; i < VER+1; i++) {
		delete(sqlat[i]);

		}
		*/
		delete(CapacityMat);
		delete(flow);
		delete(visited);
		delete(Exmat);
		delete(Jmat);
		delete(latt);
		delete(Wmat);
		delete(Bmat);
		delete(sqlat);
		delete(clusstats);
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		printf(TEXT("\nThere is  %*ld percent of memory in use.\n"),
			WIDTH, statex.dwMemoryLoad);
		printf(TEXT("There are %*I64d total KB of physical memory.\n"),
			WIDTH, statex.ullTotalPhys / DIV);
		printf(TEXT("There are %*I64d free  KB of physical memory.\n"),
			WIDTH, statex.ullAvailPhys / DIV);

	}
	cout << "\n\n\nAVERAGE TIME for " << VER << " ver : " << sum / iter << endl;

	for (l = 0; l < iter; l++)
	{
		cout << "-" << tdiff[l];
	}


	system("PAUSE");
	return 0;
}