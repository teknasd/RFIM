//	RFIM for 2D without OPENCV
//	making use of stl vectors instead of new 
//	with clustering implementation
//	for raw visited node output
//
//	author: Teknas

//#define _HAS_ITERATOR_DEBUGGING 0

#include "Headers.h"
#include "Parameters.h"
#include "clustering.h"
#include "PushRelabel.h"
#include "LatticeStuff.h"
//nclude "MPMalgo.h"

using namespace std;

int main(void) {
	
	// storing data into csv file
	ofstream file("log_rfim_2D.csv");
	file << "\niter,del,m=(mag/N)^2" << endl;
	time_t time_begin=0, time_end=0, time_1=0, time_2=0;

	long t1, t2, l = 0, sum = 0, i, j, cap = 0, clusters = 0;
	
	for (l = 0; l < iter; l++)
	{
		cout << "\n\n*****ITER:  " << l << "****\n\n";
			
		
	/* ==================== INITIALISING 2D VECTORS ================*/
		//std::unordered_map<int, std::unordered_map<int, std::unordered_map<std::string, float>>> network;
		vector < vector <int> >   Exmat(N, vector<int>(N, 0))
								, sqlat0(VER + 1, vector<int>(VER + 1, 0))
								, sqlat1(VER + 1, vector<int>(VER + 1, 0));
		vector < vector <float> >   flow(V, vector<float>(V, 0))
								, CapacityMat(V, vector<float>(V, 0));
	/* ==================== INITIALISING 1D VECTORS ================*/
		vector <int>  visited(V, 0), clusstats0(V / 2, 0), clusstats1(V / 2, 0);
		vector<float>  Bmat(N, 0), Wmat(N, 0);
		
	/* ====================== CREATING EXISTANCE MATRIX (Exmat)============= */

		create_Exmat(Exmat);
		//cout << "Exmat created" << endl;
		
	/* ================= CREATING CAPACITY MATRIX (CapacityMat)============= */

		create_CapacityMat(CapacityMat, Exmat);
		//cout << "created CapacityMatrix" << endl;
	/* =====================CREATING BMAT ===================================*/
		create_Bmat_bimodal(Bmat, 1);
	/* =================== LOOP TO ITERATE OVER RANGE OF DELTA ============= */

		for (float del = del_beg; del <= del_end; del += del_inc)
		{
			//open 
			//cout << "\ndEL: " << del << tab;
			
			/* =========================== BETA MATRIX (Bmat)======================= */

			update_Bmat_bimodal(Bmat,del);  // UNCOMMENT FOR BIMODAL DISTRIBUTION
			  //create_Bmat_gaussian(Bmat,del);	  // UNCOMMENT FOR GAUSSIAN DISTRIBUTION
			  //cout << "Bmat created" << endl;
			  //printMatrix(Bmat, N);
			  savedata(Bmat,l,del,"Phi");

			create_Wmat(Wmat, CapacityMat, Bmat, del);

			/* =========== AGUMENTING CAPACITY MATRIX WITH WMAT ============= */

			create_Augumented_CapacityMat(Wmat, CapacityMat);
			//printMatrix(CapacityMat, V);
			t1 = long(time(&time_1));  /* get current time;*/
			//cout << "\ntic toc";
			/*================================================================================================*/
			/*  ladies and gentlemen its honour to present you the most important stuff in this awesome code  */
			/* ===================== CALLING PUSH RELABEL(CapacityMat)======================================= */
			savedata(CapacityMat, l, del, "cap");
			pushRelabel(CapacityMat, flow, 0, V - 1);
			
			// mpm algo =======================================================================================
			/*create_adj_list(CapacityMat,network);
			float found = search_network(8,9,network);
			cout << "found :" << found<<endl;
			print_network(network);
			if (in(8, network)) { cout << " 8 here "; };
			if (!in(30, network)) { cout << " no 30 here "; };
			erase_network(network);
			*/

			/*====================================================================================*/
			t2 = long(time(&time_2));  /* get current time;*/

			/* ====================== CREATE RESIDUAL GRAPH ================== */
			create_Residual_graph(CapacityMat, flow);

			/*======================================================*/
			/*============	CLUSTERIZATION 	======================= */
			/*======================================================*/

			/* ================= DEPTH FIRST SEARCH ON FLOW ================== */
			dfs(flow, 0, visited);
			savedata(visited, l, del, "Si");
			//printMatrix(visited, V);
			/* ============ CREATE AGUMENTED MATRIX AROUND LATTICE =========== */
			createAgumentedMatrix(sqlat0, sqlat1, visited);

			/* ====================== HK CLUSTER ALGO ================== */
	//		cluster(sqlat1, 1, clusstats1);
	//		cluster(sqlat0, 0, clusstats0);

			/* ====================== PRINT CLUSTERS ================== */
				//printCluster(sqlat1, clusstats1);
				//printCluster(sqlat0, clusstats0);

			/* ====================== COUNT CLUSTERS ================== */
	//		clusters = no_of_clusters(clusstats0, clusstats1);
	//		cout << "clusters: " << clusters << "\n";

			/* =============== ROOT MEAN SQUARE MAGNETIZATION ========== */
			float mag = rms_mag(visited);
			cout << "config: "<<l<<"\tdel: "<<del<<"\tmag_per_sq_unit: " << mag <<"\ttime: "<<t2-t1<< "\n";
			
			//copying data to file
			file << "," << mag ;
			
			

			//re_init
			for (i = 0; i < sqlat0.size(); i++)
			{
				for (j= 0; j < sqlat0[i].size(); j++)
				{
					sqlat0[i][j] = 0;
					sqlat1[i][j] = 0;
				}
			}
			for (i = 0; i < flow.size(); i++)
			{
				for (j = 0; j < flow[i].size(); j++)
				{

					flow[i][j] = 0;
				}
			}
			for (i = 0; i < clusstats0.size(); i++)
			{
				clusstats0[i] = 0;
				clusstats1[i] = 0;
			}
			for ( i = 0; i < Wmat.size(); i++)
			{
				Wmat[i] = 0;
			}
			for ( i = 0; i < visited.size(); i++)
			{
				visited[i] = 0;
			}
				
			//Reinit(sqlat0, sqlat1, flow, CapacityMat, visited, clusstats0, clusstats1, Wmat);
			//close
			
		}
		file << endl;
		// clear all 
		ClearAll(sqlat0, sqlat1, flow, CapacityMat, visited, clusstats0, clusstats1, Wmat);
		

	}
	cout << "\n\n\nAVERAGE TIME for " << VER << " ver : " << sum / iter << endl;

	
	system("PAUSE");
	return 0;
}
