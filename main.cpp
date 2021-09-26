#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include <iomanip>
using namespace std;

class Graph{

private:
    map<string, vector<string>> adjList;
    map<string, float> rank;
    int vertexCount;
    map<string, float> CalcRank(map<string, float>& rankSum);
    void PrintRank();

public:
    Graph(){ vertexCount = 0; }
    void InsertEdge(string from, string to);
    void PageRank(int p);
};

void Graph::InsertEdge(string from, string to)
{
    //case for first instance of a particular website to be entered linking to another website
    if(adjList.find(from) == adjList.end())
        ++vertexCount;
    //case for first instance of a particular website being linked to, hence ot doesn't link to any other website
    if(adjList.find(to) == adjList.end()){
        adjList[to] = {};
        ++vertexCount;
    }

    adjList[from].push_back(to);
}

void Graph::PageRank(int p)
{
    map<string, float> rankSum;
    //initialize rank r(0)
    for(auto& iter : adjList)
        rank[iter.first] = 1.0/vertexCount;

    //performs p-1 iterations of CalcRank and updates the rank after every iteration as the updated rank is used in
    //the calculation of the proceeding iteration
    for(int i = 1; i < p; i++)
        rank = CalcRank(rankSum);

    PrintRank();
}

map<string, float> Graph::CalcRank(map<string, float>& rankSum)
{
    //first I initialize the map used to store the new rank so that the sum of products is 0
    for(auto& iter : adjList)
        rankSum[iter.first] = 0;

    //for sake of simplicity the following explanation will follow as though the matrix multiplication is performed on
    //a 2D array of size V x V where V is the number of webpages, arranged such that the columns are "from" and
    //the rows are "to" (how it is expressed in the spec sheet)

    //The entire adjacency list can be though of as a V x V matrix and can be broken down into V, V x 1 matrices, that
    //have the value 0 or 1/outdegree (1.0/iter.second.size()). These matrices are multiplied by the V x 1 rank matrix
    //which can be broken down into V, 1 x 1 matrices (rank[iter.first]). The product of the V x 1 and 1 x 1 represents
    //the effect of one "from" vertex to all the "to" vertices it points to and adds the weight to the rank sum of the
    //"to" vertices. After doing the same iteration for all "from" vertices and adding the weight to the "to" vertices,
    //the final rank is returned
    for(auto& iter : adjList){
        for(int i = 0; i < iter.second.size(); i++)
            rankSum[iter.second[i]] += (1.0/iter.second.size()) * rank[iter.first];
    }
    return rankSum;
}

void Graph::PrintRank()
{
    //using setprecision to round decimal to the hundredth place
    for(auto& iter : rank){
        cout << iter.first << " ";
        cout << fixed << std::setprecision(2) << iter.second;
        cout << endl;
    }
}

int main()
{
    int no_of_lines, power_iterations;
    string from, to;
    cin >> no_of_lines;
    cin >> power_iterations;
    Graph g;
    for(int i=0;i< no_of_lines;i++){
        cin >> from;
        cin >> to;
        g.InsertEdge(from, to);
    }
    g.PageRank(power_iterations);

    return 0;
}