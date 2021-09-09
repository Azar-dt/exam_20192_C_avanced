#include <bits/stdc++.h> 

#define max_int 1000000000
using namespace std;

typedef map <string, map<string,double>> Graph; 
typedef map <string, int> st_map; 
typedef pair<double,string> ipair; 

void insert_vertex(Graph& g, string v); 

void insert_edge(Graph& g, string u, string v,double weight);  

Graph get_reverse_graph(Graph g); 

double Dijkstra(Graph g, string start, string end); 

void Prim_mst(Graph g); 