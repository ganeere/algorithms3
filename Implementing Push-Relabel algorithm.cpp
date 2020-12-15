//alican aydın
//150110044

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>

using namespace std;

class Graph{
    vector <vector<int>> capacity;
    vector <vector<int>> flow;
    int nodeCount;
    vector <int> excess;
    vector <int> height; 
    public:
    void addCapacity(int, int, int);
    void push(int, int);
    void relabel(int);
    void initializePreflow(int);
    int overflowingVertex();
    int findMaxFlow();
    Graph(int);
    ~Graph();
};

Graph::Graph(int n){
    nodeCount = n;
    capacity.assign(nodeCount, vector<int>(nodeCount, 0));
}

void Graph::addCapacity(int from, int to, int cap){
    capacity[from][to] = cap;
}

int Graph::overflowingVertex(){
		for(int a = 1; a < nodeCount - 1; a++){ 
    		if(excess[a] > 0){
    				return a;
    		}
    }
    return -1;
}

void Graph::initializePreflow(int source){
		height.assign(nodeCount, 0);
		height[source] = nodeCount;
		excess.assign(nodeCount, 0);
		
		flow.assign(nodeCount, vector<int>(nodeCount, 0));
		
		
		
		for(int v = 0; v < nodeCount; v++){
				if(capacity[source][v] != 0){
						flow[source][v] = capacity[source][v];
						flow[v][source] = -1 * capacity[source][v];
						excess[v] = capacity[source][v];
						excess[source] = excess[source]-capacity[source][v];
				}
		}
}

void Graph::push(int u, int v){
		int delta;
		int x=capacity[u][v] - flow[u][v];
		if(excess[u] > x){
				delta = x;
		} else{
				delta = excess[u];
		}
		flow[u][v] = flow[u][v]+ delta;
		flow[v][u] =flow[v][u]- delta;
		excess[u] = excess[u]- delta;
		excess[v] = excess[v] + delta;
}

void Graph::relabel(int u){
		int min_Height = INT_MAX;
		for(int v = 0; v < nodeCount; v++){
				if(capacity[u][v] > flow[u][v]){
						if(min_Height > height[v]){
								min_Height = height[v];
						}
				}
		}
		height[u] = min_Height + 1;
}

int Graph::findMaxFlow(){
		int b = overflowingVertex();
		while(b!= -1){
				bool pushed = false;
				int k=0;
				while(k<nodeCount){
				
						if(height[k] < height[b] && flow[b][k] < capacity[b][k]){
								push(b, k);
								pushed = true;
								break;
						}
						k++;
				}
				if(!pushed){
						relabel(b);
				}
				b= overflowingVertex();
		}
		return excess[nodeCount - 1];
}

Graph::~Graph(){

}

int main(int argumentNumber, char *arguments[]){
    int nodeCount;
    string filename = arguments[1];
    string tempString;
    int temp, from, to, flow;
    
    ifstream file;
    file.open(filename);
    file >> nodeCount;
    Graph graph(nodeCount);
    file >> tempString >> tempString >> tempString;
    while(file.good()){
        file >> from >> to >> flow;
        graph.addCapacity(from, to, flow);
    }
    graph.initializePreflow(0); // 0th node is source
    cout << "Maximum flow: " << graph.findMaxFlow() << endl; 
    return 0;
}
