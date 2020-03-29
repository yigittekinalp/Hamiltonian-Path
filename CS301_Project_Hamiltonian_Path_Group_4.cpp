#include <iostream>
#include <cstdlib>
#include <chrono>
#include "math.h"

#define NODE 150     // number of vertices for graph generation
#define NUM 50		 // number of graphs
#define PROB 50		 // probability of connecting the new node to each node for every old node

using namespace std;

int graph[NODE][NODE]; // = {
//0,0,1,1,1,1,0,1,1,
//0,0,1,0,0,0,1,0,0,
//1,1,0,1,1,0,0,0,0,
//1,0,1,0,1,0,0,0,0,
//1,0,1,1,0,0,0,0,0,
//1,0,0,0,0,0,0,0,1,
//0,1,0,0,0,0,0,1,0,
//1,0,0,0,0,0,1,0,1,
//1,0,0,0,0,1,0,1,0};

int path[NODE];

#define NOT_IN_STACK 0
#define IN_STACK 1
bool dfs(int v, int adj[][NODE], int label[NODE], int instack_count, int n){
        if(instack_count == n)
            return true;
        for(int i=0; i<n; i++)
            if(adj[v][i] && label[i] == NOT_IN_STACK){
                label[i]=IN_STACK;
                if(dfs(i, adj, label, instack_count+1, n))
                    return true;
                label[i]=NOT_IN_STACK;
            }
        return false;
}
bool check_using_dfs(int adj[][NODE], int n){
        int label[NODE];
        for(int i=0; i<n; i++)
            label[i]=NOT_IN_STACK;
        for(int i=0; i<n; i++){
            label[i]=IN_STACK;
            if(dfs(i, adj, label, 1, n))
                return true;
            label[i]=NOT_IN_STACK;
        }
        return false;
}

void displayCycle() {
   cout<<"Path: ";

   for (int i = 0; i < NODE; i++)
      cout << path[i] << " ";
   cout << endl;
  // cout << path[0] << endl;      //print the first vertex again
}

bool isValid(int v, int k) {
   if (graph [path[k-1]][v] == 0)   //if there is no edge
      return false;

   for (int i = 0; i < k; i++)   //if vertex is already taken, skip that
      if (path[i] == v)
         return false;
   return true;
}

bool cycleFound(int k) {
   if (k == NODE) {             //when all vertices are in the path
         return true;
   }
   for (int v = 1; v < NODE; v++) {       //for all vertices except starting point
      if (isValid(v,k)) {                //if possible to add v in the path
         path[k] = v;
         break;
      }
	  path[k] = -1;               //when k vertex will not in the solution
   }
   if(path[k] == -1)
	   return false;

   if (cycleFound (k+1) == true)
	   return true;
       
   return false;
}

bool hamiltonianCycle() {
   for (int i = 0; i < NODE; i++)
      path[i] = -1;
   path[0] = 0; //first vertex as 0

   if ( cycleFound(1) == false ) {
     // cout << "Solution does not exist"<<endl;
      return false;
   }
  // displayCycle();
   return true;
}

int rateCalc(int a1[],int a2[], int num = NUM){
    int rate = 0;
    for(int a = 0; a<num; a++){
        
        
        if(a1[a] == a2[a]){
            rate++;
        }
    }
    rate = rate * 100;
    rate = rate / num;
    return rate;
}

int main() {

	int x = 0;
	int a [NUM];
	int b [NUM];
	float runtimes[NUM];

	for (int i = 0; i < NUM; i++)
	{
		a[i] = 0;
	}
	
	for (int i = 0; i < NUM; i++)
	{
		b[i] = 0;
	}

	for(int k = 0; k < NUM; k++)
	{
		for (int i = 0; i < NODE; i++)
		{
			for (int j = i; j < NODE; j++)
			{
				if(i == j){
					graph[i][j] = 0;
				}else{
					x = rand() % 100;
					if(x <= PROB){
						graph[i][j] = graph[j][i] = 1;
					}
					else{
						graph[i][j] = graph[j][i] = 0;
					}
				}
			}
		}
    
		bool check_dfs = check_using_dfs(graph, NODE);
		if(check_dfs) {  // exact solution
			b[k] = 1;
		}
	
		auto start = chrono::system_clock::now();
		bool check_hamiltonian = hamiltonianCycle(); // call heuristic while keeping time
		auto end = chrono::system_clock::now();

		chrono::duration<float> elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); // calculate the runtime

		runtimes[k] = elapsed_seconds.count();  // store the runtime

	    if(check_hamiltonian){
			a[k] = 1;  // heuristic solution
		}
		
		cout << endl;
	}
	// EXPERIMENTAL ANALYSIS
	////////////////////////////////////////////////////////////////////////////////////////////

	float sum = 0, avg, var = 0, stdev = 0, tvalue90 = 1.660, tvalue95 = 1.984;

	for (int i = 0; i < NUM; i++)
	{
		sum += runtimes[i];
	}

	avg = sum/NUM;

	for (int i = 0; i < NUM; i++)
	{
		var += (runtimes[i] - avg)*(runtimes[i] - avg);
	}

	var = var/NUM;
	stdev = sqrt(var);
	
	float stderror = stdev/sqrt(NUM);
	float confint90_1 = avg + tvalue90*stderror, confint90_2 = avg - tvalue90*stderror;
	float confint95_1 = avg + tvalue95*stderror, confint95_2 = avg - tvalue95*stderror;

	cout << endl << "mean = " << avg << "  stdev = " << stdev
		<< "  stderror = " << stderror << endl << "90ci = " << confint90_1 << " - " << confint90_2 
		<< "  95ci = " << confint95_1 << " - " << confint95_2 << endl; 

	////////////////////////////////////////////////////////////////////////////////////////////

	cout << endl << "Approximation Solution" << endl;
	for (int i = 0; i < NUM; i++)  // print the solutions of the approximation algorithm
	{
		cout << a[i];
	}
	cout << endl << endl;
	
	cout << "Exact Solution" << endl;
	for (int i = 0; i < NUM; i++)  // print the solutions of the exact algorithm
	{
		cout << b[i];
	}
	cout << endl << endl << "Success Rate = %" << rateCalc(a, b) << endl;  // calculate the success rated

	//cout << hamiltonianCycle();
}