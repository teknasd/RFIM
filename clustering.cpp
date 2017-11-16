#include "Headers.h"
#include "Parameters.h" // >> contains all the req parameters

///*
//*  A DFS based function to find all reachable vertices from s.
//*/
void dfs(std::vector <std::vector <int> >& F, long s, std::vector <int>& visited)
{
	visited[s] = true;
	for (long i = 0; i < V; i++)
	{
		if (F[s][i] && !visited[i])
		{
			//std::cout << i << d;
			dfs(F, i, visited);
		}
	}

}


int gotoindex(std::vector <int>& A, int index)
{
	if (index == HIGH)
	{
		return index;
	}
	else if (A[index] < 0)
	{
		//std::cout << "\nconflict of" << A[index] << "index changed to" << abs(A[index]) << endl;
		gotoindex(A, abs(A[index]));
	}
	else
	{
		//std::cout << "\nindex ret: " << index << endl;
		return index;
	}
}

//
//
int cluster(std::vector<std::vector<int> >& lat, int key, std::vector <int>& clusstats)
{

	//finding clusters of 1
	int m = 0, n = 0, min = 0, a = 0, b = 0, i=0, j=0, k=0;
	std::vector <int> arr;
	for (i = 1; i < VER + 1; i++)
	{

		for (j = 1; j < VER + 1; j++)
		{

			if (lat[i][j] == key)
			{
				a = lat[i - 1][j];
				b = lat[i][j - 1];
				arr.push_back(gotoindex(clusstats, a));
				arr.push_back(gotoindex(clusstats, b));
				sort(arr.begin(), arr.end());
				min = arr[0];

				//std::cout << "\n\nbw " << a << "(" << i - 1 << "," << j << ") & " << b << " (" << i << "," << j - 1 << " )\n";
				//min = MIN(gotoindex(clusstats, a),gotoindex(clusstats, b));

				if (min == HIGH)
				{
					//std::cout << "rule:" << 1 << d;
					m++;
					clusstats[m] = 1;
					lat[i][j] = m;
					//std::cout << "ret:" << m << d;
				}
				else
				{
					clusstats[min]++;
					lat[i][j] = min;
					if (min != HIGH && arr[1] != HIGH)
					{
						if (min != arr[1])
						{
							clusstats[min] += clusstats[arr[1]];
							clusstats[arr[1]] = -min;
						}
					}
				}
				arr.clear();

			}
		}

	}

	return 0;
}


void printCluster(std::vector<std::vector<int> >& lat, std::vector <int>& clusstats)
{

	//uncomment block below to print cluster on console
	for (int i = 0; i <= VER; i++)
	{
		for (int j = 0; j <= VER; j++)
		{
			if (lat[i][j] == HIGH)
			{
				std::cout << ".\t";// << sqlat1[i][j] << " ";
								 //std::cout << gotoindex(clusstats1, sqlat1[i][j]) << " ";
			}
			else
			{	//std::cout << sqlat0[i][j] << " ";
				//std::cout << "" << gotoindex(clusstats, lat[i][j]) << "\t";
				std::cout << "" << lat[i][j] << "/" << gotoindex(clusstats, lat[i][j]) << "\t";
			}
		}std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < clusstats.size(); i++)
	{
		std::cout << clusstats[i] << ":";
	}std::cout << std::endl; std::cout << std::endl;

}

void createAgumentedMatrix(std::vector < std::vector <int> >& sqlat0, std::vector < std::vector <int> >& sqlat1, std::vector <int> & visited)
{
	int i = 0, j = 0;
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

			//std::cout << visited[(i - 1)*VER + (j - 1) + 1];

		}
	}
}

long no_of_clusters(std::vector <int>& clusstats0, std::vector <int>& clusstats1)
{

	long i = 0,counter=0;

	for (i = 0; i < V / 2; i++)
	{
		
		if ((clusstats0[i] > 0) || (clusstats1[i] > 0))
		{
			counter++;
		}
	}

	return counter;
}