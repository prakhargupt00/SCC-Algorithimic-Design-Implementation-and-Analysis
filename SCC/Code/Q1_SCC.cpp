// Strongly connected components in the directed graph DFS implementation

//By Prakhar Gupta 2017A7PS0121H


//Importing the header files
#include <bits/stdc++.h>
#include <iostream>
#include <stack>
#include <vector>
#define ll long long
using namespace std ;

//Defining  class graph
class Graph{
    ll nodes,edges ;
    vector <ll> *adj;

    // A recursive function to print DFS of the graph starting from node 'start'
    void dfsRecursive(ll start, bool visited[]);

    // A recursive function to push nodes in the stack in the increasing order of finishing times with the top element
    // of stack having the maximum finishing time
    void fillStack(ll node, bool visited[], stack<ll> &Stack);

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

        // The Function to  finds and prints the strongly connected components in the directed graph
        void getSCCs();

        // Function to get the transpose of the graph
        Graph transposeGraph();

};

void Graph::fillStack(ll s, bool visited[], stack<ll> &Stack)
{
    // Mark the current node as visited and print it
    visited[s] = true;

    // Recur for all the nodes adjacent to this vertex
    for(auto i = adj[s].begin(); i != adj[s].end(); ++i)
        if(!visited[*i])
            fillStack(*i, visited, Stack);

    // All nodes reachable from v are processed by now, push v
    Stack.push(s);
}

void Graph::dfsRecursive(ll s,bool * visited){
        visited[s] = true;
        cout << s << " ";

        // Recursively traverse for all the nodes adjacent to this vertex while printing
        for(ll i = 0;i < adj[s].size();++i){
         if(visited[adj[s][i]] == false)
             dfsRecursive(adj[s][i], visited);
        }
}


void Graph::getSCCs()
{
    stack<ll> Stack;

    // Mark all the nodes as not visited (For first DFS)
    bool *visited = new bool[nodes];

    initialize(visited) ;

    // Fill nodes in stack according to their finishing times
    for(ll i = 0; i < nodes; i++)
        if(visited[i] == false)
            fillStack(i, visited, Stack);

    // Create a transposed graph
    Graph gt = transposeGraph();

    // For second DFS again mark all the nodes as not visited
    initialize(visited) ;

    // Now process all nodes in order defined by Stack
    while (Stack.empty() == false)
    {
        // Pop a vertex from stack
        ll n = Stack.top();
        Stack.pop();

        // Print Strongly connected component of the popped vertex
        if (visited[n] == false)
        {
            gt.dfsRecursive(n, visited);
            numSCCs++ ;
            cout << endl <<"============================================================="<<endl;
        }
    }
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

int main()
{
    FILE *input, *output; ;
    // for getting input from input.txt
   // input = freopen("email-Eu-core.txt", "r", stdin);
   // output = freopen("S_output.txt", "w", stdout);

    ll x, y, nodes, edges;
        cin >> nodes;       //Number of nodes
        cin >> edges;       //Number of edges

    Graph g(nodes,edges) ;
    for(ll i = 0;i < edges;++i){
        cin >> x >> y;
        g.addEdge(x,y);
    }

    cout << endl <<"===================ALL STRONGLY CONNECTED COMPONENTS======================================"<<endl;

    g.getSCCs() ;

    cout<<"The number of strongly connected components are :"<<g.numSCCs ;

    return 0;
}





