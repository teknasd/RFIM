#include "Headers.h"
#include "Parameters.h" // >> contains all the req parameters


void push(std::vector <std::vector <float> >const& C1, std::vector <std::vector <float> >& F1, std::vector <float>& excess1, int u, int v) {
	//std::cout << "\nin push\n";
	float send = std::min(excess1[u], C1[u][v] - F1[u][v]);
	F1[u][v] += send;
	F1[v][u] -= send;
	excess1[u] -= send;
	excess1[v] += send;
}

void relabel(std::vector <std::vector <float> >const & C, std::vector <std::vector <float> >& F, std::vector <int>& height, int u) {
	int v;
	int min_height = INFINITE;
	for (v = 0; v < V; v++) {
		if (C[u][v] - F[u][v] > 0) {
			min_height = std::min(min_height, height[v]);
			height[u] = min_height + 1;
		}
	}
}

//void discharge(const int * const * C, int ** F, int *excess, int *height, int *seen, int u) {
void discharge(std::vector <std::vector <float> >const & C, std::vector <std::vector <float> >& F, std::vector <float> & excess, std::vector <int> & height, std::vector <int>& seen, int u) {

	//std::cout << "\nflag 2\n";
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

void moveToFront(int i, std::vector <int>& A) {
	//std::cout << "\nflag 3\n";
	int temp = A[i];
	int n;
	for (n = i; n > 0; n--) {
		A[n] = A[n - 1];
	}
	A[0] = temp;
}

//int pushRelabel(const int * const * C, int ** F, int source, int sink) {
void pushRelabel(std::vector <std::vector <float> >const& C, std::vector <std::vector <float> >&  F, int source, int sink) {
	//int *excess, *height, *list, *seen, i, p;
	std::vector <int>  height(V, 0), list(V - 2, 0), seen(V, 0);
	std::vector <float> excess(V, 0);
	int i, p;
	//std::cout << "add in fn " << &C[100][70];
	/*excess = new int[V];
	height = new int[V];
	seen = new int[V];
	list = new int[V - 2];
	for (i = 0; i < V; i++) {
	excess[i] = 0;
	height[i] = 0;
	seen[i] = 0;
	if (i<V - 2)
	list[i] = 0;
	}*/
	for (i = 0, p = 0; i < V; i++) {
		if ((i != source) && (i != sink)) {
			list[p] = i;
			p++;
		}
	}
	//std::cout << "\nflag 1\n";
	height[source] = V;
	excess[source] = INFINITE;
	/*std::cout << "excess" << endl;
	printMatrix(excess, V);*/
	for (i = 0; i < V; i++)
		push(C, F, excess, source, i);
	/*std::cout << "excess" << endl;
	printMatrix(excess, V);*/

	p = 0;
	while (p < V - 2) {
		int u = list[p];
		int old_height = height[u]; //std::cout << "\nflag 2\n";
		discharge(C, F, excess, height, seen, u);
		if (height[u] > old_height) {
			moveToFront(p, list);
			p = 0;
		}
		else
			p += 1;
	}
	float maxflow = 0;
	for (i = 0; i < V; i++)
		maxflow += F[source][i];
	std::cout << "maxflow: " << maxflow << tab;
	list.erase(list.begin(), list.end());
	seen.erase(seen.begin(), seen.end());
	height.erase(height.begin(), height.end());
	excess.erase(excess.begin(), excess.end());


	//std::cout << "\nflag 2\n";

}
