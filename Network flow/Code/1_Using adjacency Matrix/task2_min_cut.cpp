//By Prakhar Gupta 2017A7PS0121H

/* =============== MIN CUT  ALGORITHM =======================================
        Idea here is simple perform ford fulkerson algorithm
        After the loop ends we get final residual graph
        now perform dfs/bfs to mark true all reachable vertices
        and finally print all edges from reachable to non-reachable vertex in original graph
*/

#include<bits/stdc++.h>
#define ll int
using namespace std ;
using namespace std::chrono;

class Graph{
    ll nodes,edges ;

    public:
        // Num max flow
        ll maxflow ;

        //adjacency list
        vector <vector<ll>> adj;

        //constructor for the graph
        Graph(ll nodes, ll edges){
            this->nodes = nodes;
            this->edges = edges;
            this->maxflow = 0 ;
            for(int i=0;i<nodes;i++){
                vector <ll> v ;
                for(int j=0;j<nodes;j++){
                    v.push_back(0) ;
                }
                adj.push_back(v) ;
            }
        }

        //function to add edge
        void addEdge(ll x, ll y, ll capacity){
            adj[x][y] = capacity ;            // Add y-1 to (x-1)'s list since directed graph
        }

        //Function to intitalise the visited array passed as parameter
        void initialize(bool *visited) {
            for(ll i = 0;i < nodes;++i)
             visited[i] = false;
        }

        // BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
        // Graph rg is residual graph
        bool findpath(vector<vector<int>> &radj, ll s, ll t, ll parent[]) ;


        // Main function implements ford fulkerson algorithm from s to t in the given graph  and calls another function to print min cut edges
        void ford_Fulkerson(ll s, ll t) ;


        //augument procedure ,finds the bottleneck and updates the residual graph parallely it calculates the flow also
        void augument(ll s, ll t, ll parent[], vector<vector<int>> &radj) ;

        //function to print the min cut edges ,
        void min_cut_edges(vector<vector<int>> &rg, int s) ;

        //simple dfs to mark all visited nodes in Residual graph as visited
        void dfs(vector<vector<int>> &radj, int s, bool visited[]) ;

};

// BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
// Graph rg is residual graph
bool Graph::findpath(vector<vector<int>> &radj, ll s, ll t, ll parent[]){

    bool visited[nodes] ;
    initialize(visited) ;

    queue <ll> q ;
    q.push(s);
    visited[s] =  true ;
    parent[s] = -1 ;

    //calculate parent of each node and find path
    while(!q.empty()){
        ll p = q.front();
        q.pop() ;

        for(int i=0 ; i<nodes; i++){
            if(!visited[i] && radj[p][i]>0){
                visited[i] = true ;
                q.push(i) ;
                parent[i] = p ;
            }
        }
    }

    //If we reached sink in BFS starting from source, then return true, else false
    return visited[t];
}

//Augument procedure ,finds the bottleneck and updates the residual graph parallely it calculates the flow also
void Graph:: augument(ll s, ll t, ll parent[], vector<vector<int>> &radj){

    while(findpath(radj,s,t,parent)){
        ll path_flow = INT_MAX ;

        //min residual capcity or bottleneck ;
        for(int i=t;i!=s; i=parent[i]){
                int j = parent[i];
                path_flow = min(path_flow,radj[j][i]) ;
        }

        // update residual graph
        for(int i=t;i!=s ;i=parent[i]){
            int j = parent[i] ;
            radj[j][i] -= path_flow ;  //operations done  in residual graph
            radj[i][j] += path_flow ;
        }

        // Update overall flow by  adding path flow
        maxflow += path_flow;
    }

}

//simple dfs to mark all visited nodes in Residual graph as visited
void Graph::dfs(vector<vector<int>> &radj, int u, bool visited[]){
    visited[u] = true;
    for (int i = 0; i < nodes; i++)
       if (radj[u][i] && !visited[i])
           dfs(radj, i, visited);
}

// Main function calculates  the maximum flow from s to t in the given graph and calls another function to print min cut edges
void Graph::ford_Fulkerson(ll s, ll t){

    Graph rg = Graph(nodes,edges) ;

    for(int i=0; i<nodes; i++){
        for(int j=0; j<nodes; j++){
            rg.adj[i][j] = adj[i][j] ;
        }
    }

    //parent array is filled by BFS and to store path
    ll parent[nodes] ;

    // Augment the flow until there is a path from source to sink in residual graph
    augument(s,t,parent,rg.adj) ;

    min_cut_edges(rg.adj,s) ;

}

//function to print the min cut edges ,here radj is adjacency matrix for residual graph
void Graph::min_cut_edges(vector<vector<int>> &radj, int s){
    //initialise a visited as array as false
    bool visited[nodes];
    initialize(visited) ;

    dfs(radj, s, visited);

    // Print all edges that are from a reachable vertex to non-reachable vertex in the original graph

    for (int i = 0; i < nodes; i++)
      for (int j = 0; j < nodes; j++)
         if (visited[i] && !visited[j] && adj[i][j])
              cout << "Edge : "<<i << "->" << j << endl;
}


int main(){

    FILE *input, *output;
    // for getting input from input.txt
   input = freopen("moreno_health.txt", "r", stdin);

    int source, sink ;
    cin>>source>>sink ;
    //source sink start from zero
    source-- ;
    sink-- ;

    ll x, y, val, nodes, edges;
    cin >> nodes;       //Number of nodes
    cin >> edges;       //Number of edges


    Graph g(nodes,edges) ;

    for(ll i = 0;i < edges;++i){
        cin >> x >> y >> val;
        //nodes start from 0
        x--,y-- ;
        g.addEdge(x,y,val);
    }

    auto start = high_resolution_clock::now();

    g.ford_Fulkerson(source, sink) ;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\nTime taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;

}


