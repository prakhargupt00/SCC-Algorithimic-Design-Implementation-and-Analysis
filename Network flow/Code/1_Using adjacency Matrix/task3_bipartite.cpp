//By Prakhar Gupta 2017A7PS0121H

/* =============== BIPARTITE MATCHING USING FORD FULKERSON ALGORITHM ================
    We can explain bipartite matching problem using jobs and applicants problem.
    Here must be a source and sink in a flow network.
    So we add a source and add edges from source to all applicants.
    Similarly, add edges from all jobs to sink.
    The capacity of every edge is marked as 1 unit.
    Now we finally apply ford-fulkerson algorithm to calculate the maximum flow
    in the flow network built in task 1. The maximum flow is actually the maximum bipartite matching we are looking for.
*/

#include<bits/stdc++.h>
#define ll long long
using namespace std ;
using namespace std::chrono;

class Graph{
    ll nodes,edges ;

    public:
        // Num max flow
        ll matchings ;

        //adjacency list
        vector <vector<ll>> adj;

        //constructor for the graph
        Graph(ll nodes, ll edges){
            this->nodes = nodes;
            this->edges = edges;
            this->matchings = 0 ;
            for(int i=0;i<nodes;i++){
                vector <ll> v ;
                for(int j=0;j<nodes;j++){
                    v.push_back(0) ;
                }
                adj.push_back(v) ;
            }
        }

        //function to add edge
        void addEdge(ll x, ll y){
            adj[x][y] = 1 ;            // Add y-1 to (x-1)'s list since directed graph
        }

        //Function to intitalise the visited array passed as parameter
        void initialize(bool *visited) {
            for(ll i = 0;i < nodes;++i)
             visited[i] = false;
        }

        // BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
        // Graph rg is residual graph
        bool findpath(Graph rg, ll s, ll t, ll parent[]) ;


        // Main function outputs  the maximum matchings from s to t in the given graph
        void ford_Fulkerson(ll s, ll t) ;


        //augument procedure ,finds the bottleneck and updates the residual graph parallely it calculates the flow also
        void augument(ll s, ll t, ll parent[], Graph rg) ;

};

// BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
// Graph rg is residual graph
bool Graph::findpath(Graph rg, ll s, ll t, ll parent[]){

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
            if(!visited[i] && rg.adj[p][i]>0){
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
void Graph:: augument(ll s, ll t, ll parent[], Graph rg){

    while(findpath(rg,s,t,parent)){
        ll path_flow = INT_MAX ;

        //min residual capcity or bottleneck ;
        for(int i=t;i!=s; i=parent[i]){
                int j = parent[i];
                path_flow = min(path_flow,rg.adj[j][i]) ;
        }

        // update residual graph
        for(int i=t;i!=s ;i=parent[i]){
            int j = parent[i] ;
            rg.adj[j][i] -= path_flow ;  //operations done  in residual graph
            rg.adj[i][j] += path_flow ;
        }
        // Adds path flow to the  overall flow
        matchings += path_flow;
    }

}

// Main function outputs  the maximum flow from s to t in the given graph
void Graph::ford_Fulkerson(ll s, ll t){

    Graph rg = Graph(nodes,edges) ;

    for(int i=0; i<nodes; i++){
        for(int j=0; j<nodes; j++){
            rg.adj[i][j] = adj[i][j] ;
        }
    }

    //parent array is filled by BFS and to store path
    ll parent[nodes] ;

    //// Augment the flow until there is a path from source to sink in residual graph
    augument(s,t,parent,rg) ;


    cout<<"The maximum matchings for given flow network is : " << matchings ;

}


int main(){

     FILE *input, *output;
    // for getting input from input.txt
    input = freopen("moreno_crime.txt", "r", stdin);

    ll  nodes_1, nodes_2, edges;
    //Number of nodes_1 and nodes_2 represent no of elements in respective bipartite sets

    cin >> nodes_1 >> nodes_2 ;
    cin >> edges;                     //Number of edges

    ll vertices = nodes_1 + nodes_2 +2 ;

    //CREATING A ADJACENCY MATRIX OF (nodes_1+nodes_2+2) VERTICES

    Graph g(vertices ,edges) ;

    for(ll i = 0;i < edges;++i){
        ll x, y ;
        cin >> x >> y ;
        x-- , y-- ;
        g.addEdge(x,nodes_1+y);
    }

    //adding edges from source to nodes_1
    for(ll i=0;i<nodes_1;i++){
        g.adj[vertices-2][i] = 1 ;
    }

    //adding edges from nodes_2 to sink
    for(ll i=nodes_1; i<nodes_1+nodes_2; i++){
        g.adj[i][vertices-1] = 1 ;
    }

    ll source =   vertices-2 ;
    ll sink   =   vertices-1 ;

    auto start = high_resolution_clock::now();

    g.ford_Fulkerson(source, sink) ;

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "\nTime taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;

}

