#ifndef GRAPH_H
#define GRAPH_H
#include "vertex.h"

class Graph
{
private:
    vector<Vertex*> vertices;
public:
    Graph(vector<Ort*>);
    ~Graph();

    //initialize the routing table of all vertices with all nodes in graph
    void initializeRoutingsTable();
    void initializeRoutingsTable(Vertex*);

    // create routing table with dijkstra algorithm
    // Notation:
    // c(a,b) : cost from node a to node b (= inf, if a isn't b's neighbor)
    // D(v) : current shortest cost from sourse to destination v
    // N' : amount of all nodes, whoes shortest path is knowed
    void dijkstra(Vertex*);

    // check whether the knowedNodesSet entails the checkedNode
    bool checkKnowedNodesSet(Vertex* checkedNode, vector<unsigned> knowedNodesSet);

    //return a Route whose Destination w isn't in N'(knowedNodesSet) and D(w) is minimal
    RouteInfor* minRoute(Vertex* vertex, vector<unsigned> knowedNodesSet);


    vector<Vertex*> getVertices() const;
    Vertex* getVertex(unsigned id) const;
    void addVertex(Vertex*);
};

#endif // GRAPH_H
