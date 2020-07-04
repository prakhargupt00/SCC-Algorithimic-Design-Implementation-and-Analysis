//By Prakhar Gupta 2017A7PS0121H

/* ===============FORD FULKERSON ALGORITHM ================
    The idea of ford-fulkerson algorithm is simple.
    Start with initial flow as 0.
    As long as there is a augmenting path from source to sink .
    Add this path-flow to flow.
    And finally after completion of above loop Return flow.
*/

#include<bits/stdc++.h>
#define ll int
using namespace std ;
using namespace std::chrono;
map< pair<ll,ll>,ll >rg_map;

class Graph{
    ll nodes,edges ;

    public:
        // Num max flow
        ll maxflow ;

        //adjacency list
        vector < vector <pair<ll,ll>> > adj;

        //constructor for the graph
        Graph(ll nodes, ll edges){
            this->nodes = nodes;
            this->edges = edges;
            this->maxflow = 0 ;
            adj.resize(nodes,vector<pair<ll,ll>>()) ;
        }

        //function to add edge
        void addEdge(ll x, ll y, ll capacity){
            adj[x].push_back( {y,capacity} );            // Add y-1 to (x-1)'s list since directed graph
           // adj[y].push_back(make_pair(x,0)) ;
        }

        //Function to intitalise the visited array passed as parameter
        void initialize( vector <bool> &visited) {
            for(ll i = 0;i < nodes;++i)
             visited.push_back(false);
        }

        // BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
        // Graph rg is residual graph
        bool findpath(vector < vector <pair<ll,ll>> >&radj, ll s, ll t, vector <pair<ll,ll>> &parent) ;


        // Main function outputs  the maximum flow from s to t in the given graph
        void ford_Fulkerson(ll s, ll t) ;


        //augument procedure ,finds the bottleneck and updates the residual graph parallely it calculates the flow also
        void augument(ll s, ll t, vector <pair<ll,ll>> &parent, vector < vector <pair<ll,ll>> > &radj) ;

};

// BFS to find a augumenting path from source 's' to sink 't'  and return true if found.Also parent is used to store path
// Graph rg is residual graph
bool Graph::findpath(vector < vector <pair<ll,ll>> > &radj, ll s, ll t, vector <pair<ll,ll>> &parent){

    vector <bool> visited (nodes) ;
    initialize(visited) ;

    queue <ll> q ;
    q.push(s);
    visited[s] =  true ;
    parent[s] = {-1,-1} ;

    //calculate parent of each node and find path
    while(!q.empty()){
        ll p = q.front();
        q.pop() ;
        ll size_v=radj[p].size();

        for (int v=0 ; v< size_v ; v++){
            if ( !visited[radj[p][v].first] && radj[p][v].second > 0){

                visited[radj[p][v].first] = true;
                q.push(radj[p][v].first);
                parent[radj[p][v].first] = {p,radj[p][v].second};
            }
        }
    }

    //If we reached sink in BFS starting from source, then return true, else false
    return visited[t];
}

//Augument procedure ,finds the bottleneck and updates the residual graph parallely it calculates the flow also
void Graph:: augument(ll s, ll t, vector <pair<ll,ll>> &parent, vector < vector <pair<ll,ll>> > &radj){

      while (findpath(radj,s,t, parent)){

        ll bottle_neck = INT_MAX;
        for (ll i=t; i!=s; i=parent[i].first){
            ll u = parent[i].first;
            bottle_neck = min(bottle_neck, parent[i].second);
        }

        //update residual graph
        for (ll i=t; i!=s; i=parent[i].first)
        {
            ll u = parent[i].first;
            ll vec_size=radj[u].size();
            for(ll j=0;j<vec_size;j++){
                if(radj[u][j].first==i){
                    radj[u][j].second-=bottle_neck;
                    break;
                }
            }

            vec_size=radj[i].size();
            for(ll j=0;j<vec_size;j++){
                if(radj[i][j].first==u){
                    radj[i][j].second+=bottle_neck;
                    break;
                }
            }
        }

        // Update overall flow by adding path_flow
        maxflow += bottle_neck;
    }

}

// Main function outputs  the maximum flow from s to t in the given graph
void Graph::ford_Fulkerson(ll s, ll t){

    Graph rg = Graph(nodes,edges) ;

    //Initialising  adjacency list for residual Graph

    for(ll i=0;i<nodes;i++){
        ll vec_size = adj[i].size();

        for(ll j=0;j<vec_size;j++){
            if(rg_map.find({i,adj[i][j].first})!=rg_map.end()){

                ll vec_size=rg.adj[i].size();
                for(ll k=0;k<vec_size;k++){
                    if(rg.adj[i][k].first == adj[i][j].first){
                        rg.adj[i][k].second = adj[i][j].second;
                        rg_map[{i,adj[i][j].first}]= adj[i][j].second;
                        break;
                    }
                }

            }else{
                rg.adj[i].push_back(adj[i][j]);
                rg_map[{adj[i][j].first,i}]=0;

                rg.adj[adj[i][j].first].push_back({i,0});
                rg_map[{i,adj[i][j].first}]=adj[i][j].second;
            }
        }
    }


    //parent array is filled by BFS and to store path
    vector<pair<ll,ll>>parent(nodes);

    // Augment the flow until there is a path from source to sink in residual graph
        augument(s,t,parent,rg.adj) ;


    cout<<"The maximum flow for given flow network is : " << maxflow ;

}


int main(){

    FILE *input, *output;
    // for getting input from input.txt
   input = freopen("moreno_health.txt", "r", stdin);

    int source, sink ;
    cin>>source>>sink ;

    ll x, y, val, nodes, edges;
    cin >> nodes;       //Number of nodes
    cin >> edges;       //Number of edges

    nodes++ ;
    Graph g(nodes,edges) ;

    for(ll i = 0;i < edges;++i){
        cin >> x >> y >> val;
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

