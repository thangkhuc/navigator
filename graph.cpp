#include "graph.h"

Graph::Graph(vector<Ort*> navidaten)
{
    for (unsigned i = 0; i < navidaten.size(); i++) {
        Vertex* newVertex = new Vertex(navidaten[i]);
        vertices.push_back(newVertex);
    }
}

Graph::~Graph()
{
    for (unsigned i = 0; i < vertices.size(); i++) {
        delete  vertices[i];
    }
}

void Graph::initializeRoutingsTable()
{
    // delete all existent Route in Routing table of all vertices
    for (unsigned j = 0; j < vertices.size(); j++) {
        unsigned size = vertices[j]->getRoutingTable().size();
        for (unsigned i = 0; i < size; i++)
            vertices[j]->popRoute();
    }

    for (unsigned j = 0; j < vertices.size(); j++) {
        Vertex* vertex = vertices[j];
        for (unsigned i = 0; i < vertices.size(); i++) {
            //distance to source is 0 and the predecessor of source is null
            if (vertices[i]->getPlace()->getID() == vertex->getPlace()->getID()) {
                RouteInfor* routeInfor = new RouteInfor(vertex, 0);
                routeInfor->setPredecessor(nullptr);
                vertex->addRoute(routeInfor);
            }

            //intitialize route to all neighbors of vertex and the predecessor of neighbor is source node
            else if (vertex->checkNeighbor(vertices[i])) {
                double distance = vertex->getPlace()->entfernung(vertices[i]->getPlace());

                RouteInfor* routeInfor = new RouteInfor(vertices[i], distance);
                routeInfor->setPredecessor(vertex->getRouteInfor(vertex));
                vertex->addRoute(routeInfor);
            }

            // distance to non-neighbor node is infinitiv
            else {
                RouteInfor* routeInfor = new RouteInfor(vertices[i], INFINITY);
                vertex->addRoute(routeInfor);
            }
        }
    }
}

void Graph::initializeRoutingsTable(Vertex* vertex)
{
    for (unsigned i = 0; i < vertices.size(); i++) {
        //distance to source is 0 and the predecessor of source is null
        if (vertices[i]->getPlace()->getID() == vertex->getPlace()->getID()) {
            RouteInfor* routeInfor = new RouteInfor(vertex, 0);
            routeInfor->setPredecessor(nullptr);
            vertex->addRoute(routeInfor);
        }

        //intitialize route to all neighbors of vertex and the predecessor of neighbor is source node
        else if (vertex->checkNeighbor(vertices[i])) {
            double distance = vertex->getPlace()->entfernung(vertices[i]->getPlace());

            RouteInfor* routeInfor = new RouteInfor(vertices[i], distance);
            routeInfor->setPredecessor(vertex->getRouteInfor(vertex));
            vertex->addRoute(routeInfor);
        }

        // distance to non-neighbor node is infinitiv
        else {
            RouteInfor* routeInfor = new RouteInfor(vertices[i], INFINITY);
            vertex->addRoute(routeInfor);
        }
    }
}

RouteInfor* Graph::minRoute(Vertex* vertex, vector<unsigned int> knowedNodesSet)
{
    int minValue = INT_MAX;
    vector<RouteInfor*> routingTable = vertex->getRoutingTable();
    RouteInfor* minRout = nullptr;

    for (unsigned i = 0; i < routingTable.size(); i++) {
        if (!this->checkKnowedNodesSet(routingTable[i]->getDestination(), knowedNodesSet) && routingTable[i]->getDistance() <= minValue) {
            minValue = routingTable[i]->getDistance();
            minRout = routingTable[i];
        }
    }
    return minRout;
}

bool Graph::checkKnowedNodesSet(Vertex* checkedNode, vector<unsigned> knowedNodesSet)
{
    for (unsigned i = 0; i < knowedNodesSet.size(); i++) {
        if (checkedNode->getPlace()->getID() == knowedNodesSet[i])
            return true;
    }
    return false;
}

void Graph::dijkstra(Vertex* vertex)
{
    vector<RouteInfor*> routingTable = vertex->getRoutingTable();

    // to make sure that the routing table entails vertices
    if (routingTable.size() == 0)
        throw std::underflow_error("(graph.cpp - line 72) There is no Element in routing table");
    else {
        // saves ID of nodes, whoes shortes path is knowed
        // viz. the nodes is in N'
        vector<unsigned> knownedNodesSet;

        //Loop Phase
        // finde den kurzten Pfad zu allen Knoten
        for (unsigned i = 0; i < routingTable.size(); i++) {

            // finde zuerst die Knote w mithilfe minDistance(), die nicht in N' ist und D(w) ist minimum
            RouteInfor* w = minRoute(vertex, knownedNodesSet);

            //fügt die Knote w in N' hinzu
            if (w == nullptr)
                return;
            else
                knownedNodesSet.push_back(w->getDestination()->getPlace()->getID());

            //aktualisiert Entfernung D(v) von Quelle zu allen Knoten (v), die Nachbar von w und nicht in N' ist
            for (unsigned j = 0; j < routingTable.size(); j++) {
                Vertex* v = routingTable[j]->getDestination();

                //aktualisiert nur wenn D(w) + c(w,v) < D(v)
                double distanceToW = w->getDistance();
                double distanceToV = vertex->getRouteInfor(v)->getDistance();
                double distance_From_W_to_V = w->getDestination()->getRouteInfor(v)->getDistance();

                if (!checkKnowedNodesSet(v, knownedNodesSet) && w->getDestination()->checkNeighbor(v)
                    && distanceToW + distance_From_W_to_V < distanceToV)
                {
                    double updateDistance = distanceToW + distance_From_W_to_V;
                    vertex->getRouteInfor(v)->setDistance(updateDistance);

                    //and the predessecor of v is now w
                    vertex->getRouteInfor(v)->setPredecessor(w);
                }
            }
        }
    }
}

Vertex* Graph::getVertex(unsigned int id) const
{
    for (unsigned i = 0; i < vertices.size(); i++) {
        if (id == vertices[i]->getPlace()->getID())
            return vertices[i];
    }
    return nullptr;
}

vector<Vertex*> Graph::getVertices() const {return this->vertices;}

void Graph::addVertex(Vertex* newVertex) {this->vertices.push_back(newVertex);}
