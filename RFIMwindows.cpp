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
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define WIDTH 7
#define DIV 1024
#define iter 1
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define INFINITE 100000
#define HIGH INFINITE

/* DEFINE ALL THE PARAMETERS HERE */
#define VER		    20	     //  width of lattice matrix. for ex 6*6 lattice
#define latt_pc     1		 // percentage of lattice points where atom exist
#define upspin_pc   0.5		 // percentage of upspin in lattice
#define w           1        // omega(w) for bimmodal distribution of Bi in eq 6.1    
#define N			VER*VER   // width of edge matrix. for ex 36*36 edges
#define V			N+2       // source + sink

#define tab			"\t"
#define d			" : "
//delta is multipling factor with Bmat
#define del_beg     0
#define del_end     41
#define del_inc     2
int del = 0;

using namespace cv;
using namespace std;




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
		var = distr(generator,10);
		if (var < 5)
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
	//cout << "\nin push\n";
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
	//cout << "\nflag 2\n";
	return maxflow;
}

void printMatrix(const int * const * M, int len) {
	int i, j;
	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++)
			cout << "\t" << M[i][j];
		cout << "\n";
	}
}

void printMatrix( int const* M, int len) {
	int i;
	for (i = 0; i < len; i++) {
			cout << "\t" << M[i];
		
	}cout << "\n";
}
//function does nothing
int gotoindex(int A[V - 1], int index)
{


	if (A[index] < 0)
	{
		//cout << "\nconflict of" << A[index] << "index changed to" << abs(A[index]) << endl;
		gotoindex(A, abs(A[index]));
	}
	else
	{
		//cout << "\nindex ret: " << index << endl;
		return index;
	}
}
struct RGB {
	uchar blue;
	uchar green;
	uchar red;
};


int main(void) {
	Mat image(VER, VER, CV_8UC3);


	//namedWindow("square image", WINDOW_AUTOSIZE); // Create a window for display.
	//imshow("square image", image); // Show our image inside it.
	
	time_t time_begin, time_end, time_1, time_2;

	long t1, t2, tdiff[iter] = { 0 }, l = 0, sum = 0, i, j, cap = 0;
	for (l = 0; l < iter; l++)
	{
		cout << "\n\n*****ITER:  " << l << "****\n\n";

		int **flow,  *visited, **CapacityMat, **Exmat, *Wmat, *latt, *Bmat, **sqlat0, **sqlat1, *clusstats0, *clusstats1;
		int J = 1;
		// dynamic memory allocated
		latt		 = new int[N];
		Bmat		 = new int[N];
		Exmat		 = new int *[N];
		flow	   	 = new int *[V];
		sqlat0		 = new int *[VER + 1];
		sqlat1		 = new int *[VER + 1];
		CapacityMat	 = new int *[V];
		Wmat		 = new int[N];
		visited		 = new int[V];
		clusstats0	 = new int[V / 2];
		clusstats1	 = new int[V / 2];
		
		//Bmat and latt initiated
		for (i = 0; i < N; i++)
		{
			Bmat[i] = 0;// B: local magnetic  fields//fig: 6.5
			latt[i] = 1;
		}
			
		//cout << "printing latte" << endl;
		//printMatrix(latt, N);
		cout << "latte created" << endl;
		cout << "Bmat creating" << endl;
		Bmat = create_Bmat(Bmat);
		cout << "Bmat created" << endl;
		//cout << "printing Bmat" << endl;
		//printMatrix(Bmat, N);
		
		//flow,visited,CapacityMat,Exmat,Wmat,sqlat0,sqlat1,clusstats0,clusstats1 2d array allocation
		for (i = 0; i < VER + 1; i++)
		{

			sqlat0[i] = new int[VER];
			sqlat1[i] = new int[VER];
		}
		for (i = 0; i < V; i++) {

			flow[i] = new int[V];
			CapacityMat[i] = new int[V];
		}
		for (i = 0; i < N; i++) {
			Exmat[i] = new int[N];
		}

		//init
		cout << "matrixex initiated sloly" << endl;
		for (i = 0; i < V; i++)
		{
			for (j = 0; j < V; j++)
			{

				if (i < VER + 1 && j < VER + 1)
				{

					sqlat0[i][j] = 0;
					sqlat1[i][j] = 0;
				}
				if (i < N && j < N)
				{
					Exmat[i][j] = 0;

				}
				CapacityMat[i][j] = 0;
				flow[i][j] = 0;
			}
			if (i < V / 2)
			{

				clusstats0[i] = 0;
				clusstats1[i] = 0;
			}
			if (i < N)
			{

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
		/*cout << "Exmat printing" << endl;
		printMatrix(Exmat, N);*/

		
		/*----------------------------------- eq 6.12 pg 110 -------------------------------------*/
		cout << "making CapacityMatrix" << endl;
		for (i = 1; i <= N; i++)
		{
			//cout << "\n";
			for (j = 1; j <= N; j++)
			{
				if (i < j)
				{
					CapacityMat[i][j] = 4 * J * Exmat[i - 1][j - 1] * 10;

				}
				else
					CapacityMat[i][j] = 0;
					//cout << CapacityMat[i][j] << " ";
			}
		}
		cout << "created CapacityMatrix" << endl;

		for (del = del_beg; del < del_end; del += del_inc)
		{
			//open 
			cout << "\n\ndEL: " << del << endl;
			
			cout << "making Wmat" << endl;
			/*----------------------------------- eq 6.13  pg 110 ---------------------------------*/
			//cout << "w mat agumenting from i=1, j=1" << endl;
			for (i = 1; i <= N; i++)
			{
				for (j = 1; j <= N; j++)
				{
					cap += CapacityMat[i][j] - CapacityMat[j][i];
					//cout << cap<<"\t"; 
				}
				Wmat[i - 1] = -2 * Bmat[i - 1] * del* latt[i - 1] - cap / 2;
				cap = 0;
				//cout <<Wmat[i-1]<< endl;
			}
			cout << "created Wmat" << endl;
			cap = 0;
			// ------------------------------- print capacity matrix
			//printMatrix(CapacityMat, V);

			// --------------------------------print Wmatrix
			//printMatrix(Wmat, N);



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


			/*================================================================================================*/
			/*  ladies and gentlemen its honour to present you the most important stuff in this awesome code  */
			cout << "\nMax Flow:" << pushRelabel(CapacityMat, flow, 0, V - 1)<<endl;
			/*====================================================================================*/
			t2 = time(&time_2);  /* get current time;*/
			//cout << "\ntime now:" << t2 << "\n\n";
			//printMatrix(flow,V);

			// making reisdual graph by overwriting the flow mat,flow[i][j] = CapacityMat[i][j] - flow[i][j];
			for (i = 0; i < V; i++)
			{
				for (int j = 0; j < V; j++)
				{
					flow[i][j] = CapacityMat[i][j] - flow[i][j];
					//printf("%d\t", capacities[i][j]-flow[i][j]);
				}
				//printf("\n");
			}

			//dfsing over res grph to get accessable and non accessable nodes
			visited = dfs(flow, 0, visited);

			cout << "visited/non visited nodes:";
			//its clustering time
			//agumented matrix
			for (i = 0; i <= VER; i++)
			{

				sqlat0[i][0] = HIGH;
				sqlat1[i][0] = HIGH;
			}
			for (j = 0; j <= VER; j++)
			{

				sqlat0[0][j] = HIGH;
				sqlat1[0][j] = HIGH;
			}
			for (i = 1; i <= VER; i++)
			{
				for (j = 1; j <= VER; j++)
				{
					sqlat0[i][j] = visited[(i - 1)*VER + (j - 1) + 1];
					sqlat1[i][j] = visited[(i - 1)*VER + (j - 1) + 1];

					if (sqlat1[i][j] == 0)sqlat1[i][j] = HIGH;
					if (sqlat0[i][j] == 1)sqlat0[i][j] = HIGH;

					//cout << visited[(i - 1)*VER + (j - 1) + 1];

				}
			}
			//cout << "\nsqlat 0\n";
			//printMatrix(sqlat0, VER);
			//cout << "\nsqlat 1\n";
			//printMatrix(sqlat1, VER);
			//clustering

			//finding clusters of 1
			int m = 0, n = 0, min = 0, a = 0, b = 0;

			for (i = 1; i < VER + 1; i++)
			{

				for (j = 1; j < VER + 1; j++)
				{

					if (sqlat1[i][j] == 1)
					{
						a = sqlat1[i - 1][j];
						b = sqlat1[i][j - 1];
						//cout << "\n\nbw " << a << "(" << i - 1 << "," << j << ") & " << b << " (" << i << "," << j - 1 << " )\n";
						min = MIN(a, b);
						if (a == HIGH && b == HIGH)
						{//rule 1
						 //cout << "rule:" << 1 << d;
							m++;
							clusstats1[m]++;
							sqlat1[i][j] = m;
							//cout << "ret:" << m << d;
						}
						else if ((a == HIGH && b < HIGH) || (a < HIGH && b == HIGH))
						{//rule 2
						 //cout << "rule:" << 2 << d;
							min = MIN(a, b);
							n = gotoindex(clusstats1, min);
							clusstats1[n]++;
							sqlat1[i][j] = n;
						}
						else if (a < b)
						{//rule 3
						 //cout << "rule:" << 3 << d;
							n = gotoindex(clusstats1, a);
							if (a != gotoindex(clusstats1, b))
								clusstats1[n] += clusstats1[gotoindex(clusstats1, b)] + 1;
							else
								clusstats1[n] += 1;
							clusstats1[b] = -n;
							sqlat1[i][j] = n;
						}
						else if (a == b)
						{//rule 4
						 //cout << "rule:" << 4 << d;
							n = gotoindex(clusstats1, a);
							clusstats1[n] += 1;
							sqlat1[i][j] = n;
						}
						else
						{//rule 5
						 //cout << "rule:" << 5 << d;
							n = gotoindex(clusstats1, b);
							if (b != gotoindex(clusstats1, a))
								clusstats1[n] += clusstats1[gotoindex(clusstats1, a)] + 1;
							else
								clusstats1[n] += 1;
							clusstats1[a] = -n;
							sqlat1[i][j] = n;
						}
						
					}
				}

			}

			//finding clusters of 0
			m = 0, n = 0, min = 0, a = 0, b = 0;
			for (i = 1; i < VER + 1; i++)
			{

				for (j = 1; j < VER + 1; j++)
				{

					if (sqlat0[i][j] == 0)
					{
						a = sqlat0[i - 1][j];
						b = sqlat0[i][j - 1];
						//cout << "\n\nbw " << a << "(" << i - 1 << "," << j << ") & " << b << " (" << i << "," << j - 1 << " )\n";
						min = MIN(a, b);
						if (a == HIGH && b == HIGH)
						{//rule 1
						 //cout << "rule:" << 1 << d;
							m++;
							clusstats0[m]++;
							sqlat0[i][j] = m;
							//cout << "ret:" << m << d;
						}
						else if ((a == HIGH && b < HIGH) || (a < HIGH && b == HIGH))
						{//rule 2
						 //cout << "rule:" << 2 << d;
							min = MIN(a, b);
							n = gotoindex(clusstats0, min);
							clusstats0[n]++;
							sqlat0[i][j] = n;
						}
						else if (a < b)
						{//rule 3
						 //cout << "rule:" << 3 << d;
							n = gotoindex(clusstats0, a);
							if (a != gotoindex(clusstats0, b))
								clusstats0[n] += clusstats0[gotoindex(clusstats0, b)] + 1;
							else clusstats0[n] += 1;
							clusstats0[b] = -n;
							sqlat0[i][j] = n;
						}
						else if (a == b)
						{//rule 4
						 //cout << "rule:" << 4 << d;
							n = gotoindex(clusstats0, a);
							clusstats0[n] += 1;
							sqlat0[i][j] = n;
						}
						else
						{//rule 5
						 //cout << "rule:" << 5 << d;
							n = gotoindex(clusstats0, b);
							if (b != gotoindex(clusstats0, a))
								clusstats0[n] += clusstats0[gotoindex(clusstats0, a)] + 1;
							else clusstats0[n] += 1;
							clusstats0[a] = -n;
							sqlat0[i][j] = n;
						}
						
					}
				}

			}

			//cout << "clus stats for 0\n";
			//printMatrix(clusstats0, N);
			
			cout << "clus stats 1\n";
			//printMatrix(clusstats1, N);

			cout << "\ncluster\n\n";
			//uncomment block below to print cluster on console
			//for (i = 0; i <= VER; i++)
			//{
			//	for (j = 0; j <= VER; j++)
			//	{
			//		if (sqlat0[i][j] == HIGH)
			//			//cout << "H ";// << sqlat1[i][j] << " ";
			//						 //cout << gotoindex(clusstats1, sqlat1[i][j]) << " ";
			//		else
			//			//cout << sqlat0[i][j] << " ";
			//			cout << "" << gotoindex(clusstats0, sqlat0[i][j]) << " ";
			//	}//cout << endl;
			//}
			//cout << "\ncluster\n\n";

			//creating cluster image
			for (i = 1; i <= VER; i++)
			{
				for (j = 1; j <= VER; j++)
				{
					RGB& rgb = image.ptr<RGB>(i - 1)[j - 1];
					if (sqlat1[i][j] == HIGH)
					{
						//cout << "H ";// << sqlat1[i][j] << " ";
						rgb.blue = 0; rgb.green = 0; rgb.red = 255;
					}				 //cout << gotoindex(clusstats1, sqlat1[i][j]) << " ";
					else
					{	//cout << sqlat0[i][j] << " ";
						rgb.blue = 255; rgb.green = 0; rgb.red = 0;
						//cout << "" << gotoindex(clusstats1, sqlat1[i][j]) << " ";
					}
				}//cout << endl;
			}

			//namedWindow("square image", WINDOW_AUTOSIZE); // Create a window for display.
			//imshow("square image", image); // Show our image inside it.
			
			string S = "img/cluster-";
			S = S + to_string(VER)+"-"+ to_string(iter) + "-" + to_string(del) + ".bmp";
			imwrite(S, image);
			cout << "cluster image created";
			cout << "\n\n\ntime_req : " << t2 - t1;

			tdiff[l] = t2 - t1;
			sum += tdiff[l];

			//re_init
			for (i = 0; i < V; i++)
			{
				for (j = 0; j < V; j++)
				{

					if (i < VER + 1 && j < VER + 1)
					{

						sqlat0[i][j] = 0;
						sqlat1[i][j] = 0;
					}


					flow[i][j] = 0;
				}
				if (i < V / 2)
				{

					clusstats0[i] = 0;
					clusstats1[i] = 0;
				}
				if (i < N)
				{

					Wmat[i] = 0;
				}visited[i] = 0;

			}
			//close
		}

		// MS mem sts
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		printf(TEXT("\nThere is  %*ld percent of memory in use.\n"),
			WIDTH, statex.dwMemoryLoad);
		printf(TEXT("There are %*I64d total KB of physical memory.\n"),
			WIDTH, statex.ullTotalPhys / DIV);
		printf(TEXT("There are %*I64d free  KB of physical memory.\n"),
			WIDTH, statex.ullAvailPhys / DIV);

		/*cout << "matrixex reinitiated sloly" << endl;
		for (i = 0; i < V; i++)
		{
			for (j = 0; j < V; j++)
			{

				if (i < VER + 1 && j < VER + 1)
				{

					sqlat0[i][j] = 0;
					sqlat1[i][j] = 0;
				}
				
				
				flow[i][j] = 0;
			}
			if (i < V / 2)
			{

				clusstats0[i] = 0;
				clusstats1[i] = 0;
			}
			if (i < N)
			{

				Wmat[i] = 0;
			}visited[i] = 0;

		}*/

		for (i = 0; i < V; i++)
		{
			delete(flow[i]);
			delete(CapacityMat[i]);

		}
		for (i = 0; i < N; i++)
		{
			delete(Exmat[i]);
		}
		delete(CapacityMat);
		delete(flow);
		delete(visited);
		delete(Exmat);
		delete(Wmat);
		delete(sqlat0);
		delete(sqlat1);
		delete(clusstats0);
		delete(clusstats1);
		//	MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		printf(TEXT("\nThere is  %*ld percent of memory in use.\n"),
			WIDTH, statex.dwMemoryLoad);
		printf(TEXT("There are %*I64d total KB of physical memory.\n"),
			WIDTH, statex.ullTotalPhys / DIV);
		printf(TEXT("There are %*I64d free  KB of physical memory.\n"),
			WIDTH, statex.ullAvailPhys / DIV);

		delete(latt);

		delete(Bmat);


	}
	cout << "\n\n\nAVERAGE TIME for " << VER << " ver : " << sum / iter << endl;

	// print time req for each iter
	for (l = 0; l < iter; l++)
	{
		cout << "-" << tdiff[l];
	}


	system("PAUSE");
	return 0;
}
