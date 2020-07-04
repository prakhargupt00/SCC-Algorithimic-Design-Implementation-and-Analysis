
//By Prakhar Gupta 2017A7PS0121H

/*=======================ALGORITHM: DIVIDE AND CONQUER STRONG COMPONENTS (DCSC)=====================
    The basic idea is to select an arbitrary vertex v, which we call a pivot vertex,
    and find its descendant and predecessor sets. The intersection of the predecessor
    and descendant sets is SCC(G, v). After outputting SCC(G, v), the vertices in G \
    SCC(G, v) are divided into three sets: Desc(G, v), P red(G, v), and Rem(G, v).
    Any additional strongly connected component must be entirely contained within one of
    these three sets, so we can divide the problem and recurse.
*/

#include <bits/stdc++.h>
#define ll long long
using namespace std ;

//Defining  class graph
class Graph{
    ll nodes,edges ;
    vector <ll> *adj;

    // A recursive function to print DFS of the graph starting from node 'start'
    void dfsRecursive(ll start, bool visited[],vector <ll>&ret);

    //A function to find intersection of predecessors and descendants using hash implementation of a set
    void findIntersection(vector<ll> des,vector<ll> pred,vector<ll> &intersection,vector<ll> &restPred,vector<ll> &restDesc);

    public:
        // Num of strongly connected components
        ll numSCCs ;

        //constructor for the graph
        Graph(ll nodes, ll edges){
            this->nodes = nodes;
            this->edges = edges;
            this->numSCCs = 0 ;
            adj = new vector<ll>[nodes];
        }

        //function to add edge
        void addEdge(ll x, ll y){
            adj[x].push_back(y);            // Add y-1 to (x-1)'s list since directed graph
        }

        //Function to intitalise the visited array passed as parameter
        void initialize(bool *visited) {
            for(ll i = 0;i < nodes;++i)
             visited[i] = false;
        }

        //The main function that finds strongly connected components using divide and conquer
        void getDCSC(vector <ll> vertices,bool * visited) ;

        //A function to find the predecessors of a node
        vector <ll> findPred(ll s) ;

        //A function to find descendants of a node
        vector <ll> findDesc(ll s ) ;

        //A function to get transpose of a graph
        Graph transposeGraph();

};

void Graph::dfsRecursive(ll s,bool * visited,vector <ll>&ret){
        visited[s] = true;
        // cout << s << " ";
        ret.push_back(s) ;

        // Recursively traverse for all the nodes adjacent to this vertex while printing
        for(ll i = 0;i < adj[s].size();++i){
         if(visited[adj[s][i]] == false)
             dfsRecursive(adj[s][i], visited,ret);
        }
}

vector<ll> Graph::findDesc(ll s){
    vector<ll>v;
    bool visited[nodes] ;
    initialize(visited) ;

    //Performing dfs to get the descendants
    dfsRecursive(s,visited,v) ;

    return v ;
}

vector<ll> Graph::findPred(ll s){
    vector<ll>v;
    bool visited[nodes] ;
    initialize(visited) ;

    //reversing the graph and performing dfs to get the predecessors
    Graph gt = transposeGraph() ;
    gt.dfsRecursive(s,visited,v) ;

    return v ;
}

//A function to find intersection of predecessors and descendants using hash implementation of a set
void Graph::findIntersection(vector<ll> desc,vector<ll> pred, vector<ll>&intersection, vector<ll> &restPred, vector<ll> &restDesc){

    unordered_set <ll> s ;

    for(ll i=0 ;i<desc.size();i++){
        s.insert(desc[i]) ;
    }

    // calculating intersection and  predecessors not in intersection
    for(ll i=0;i<pred.size();i++){
        if(s.find(pred[i]) != s.end()){
            intersection.push_back(pred[i]) ;
        }else{
              restPred.push_back(pred[i]) ;
        }
    }

    //  calculating descendants not in intersection
    unordered_set <ll> t ;

    for(ll i=0 ;i<pred.size();i++){
        t.insert(pred[i]) ;
    }

    for(ll i=0;i<desc.size();i++){
        if( t.find(desc[i]) == t.end()){
            restDesc.push_back(desc[i]) ;
        }
    }


}

void Graph::getDCSC(vector <ll> vertices, bool * visited){
    //base condition
    if(vertices.size() == 0 )
        return ;

    vector <ll> randNodes ,desc ,pred,rem, intersection ,restPred, restDesc;
    //Randomised  vertex selection

    for(ll i=0;i<vertices.size();i++){
        if(visited[ vertices[i] ] == false){
            randNodes.push_back(vertices[i]) ;
        }
    }

    if(randNodes.size() == 0)
        return ;

    //Generate a random number from 0 to size - 1
    ll randIndex = rand() % (randNodes.size()) ;
    ll randnode = randNodes[randIndex] ;
    //finding ancestors and descendants of randnode
    desc = findDesc(randnode) ;
    pred = findPred(randnode) ;
    findIntersection(desc, pred, intersection,restPred, restDesc) ;

    //calculating the remainder of the graph
    unordered_set <ll> desPred ;
    for(ll i=0;i<desc.size();i++)
        desPred.insert(desc[i]) ;

    for(ll i=0;i<pred.size();i++)
        desPred.insert(pred[i]) ;

    for(ll i=0 ;i<vertices.size() ; i++){
        if(desPred.find(vertices[i]) == desPred.end())
            rem.push_back(vertices[i]) ;
    }

    // Displaying the strongly connected components
    for(ll i=0;i<intersection.size();i++){
        visited[intersection[i]] = true ;
    }

    for(ll i=0;i<intersection.size();i++){
        cout<<intersection[i]<<" " ;
    }
    numSCCs++ ;

     cout<<endl<<"=================================================="<<endl ;


      getDCSC(restPred,visited) ;
      getDCSC(restDesc,visited) ;
      getDCSC(rem,visited) ;

}


Graph Graph::transposeGraph()
{
    Graph gt(nodes,edges);
    for (ll n = 0; n < nodes; n++)
    {   // Recurse for all the nodes adjacent to this vertex and push this vertex in these node's list
        for(auto i = adj[n].begin(); i != adj[n].end(); ++i)
        {
            gt.adj[*i].push_back(n);
        }
    }
    return gt;
}

int main(){

     FILE *input, *output;
    // for getting input from input.txt
    input = freopen("email-Eu-core.txt", "r", stdin);
    output = freopen("cd_output.txt", "w", stdout);

    ll x, y, nodes, edges;
        cin >> nodes;       //Number of nodes
        cin >> edges;       //Number of edges

    Graph g(nodes,edges) ;

    for(ll i = 0;i < edges;++i){
        cin >> x >> y;
        g.addEdge(x,y);
    }

    bool visited[nodes] ;

    vector <ll> vertices ;
    for(ll i=0;i<nodes;i++){
        vertices.push_back(i) ;
        visited[i] = false ;
    }

    g.getDCSC(vertices, visited) ;
    cout<<"The number of strongly connected components are :"<<g.numSCCs<<endl ;

    cout<<"==============================END================================" ;

    return 0 ;
}



