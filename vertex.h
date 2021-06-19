#ifndef VERTEX_H
#define VERTEX_H
#include "ort.h"
#include "routeinfor.h"

class Vertex
{
private:
    Ort* place;
    vector<Vertex*> neighbors;
    vector<RouteInfor*> routingsTable;
public:
    ~Vertex();
    Vertex(Ort*);

    // get route information to this vertex
    // return null if there is no route to this destination
    RouteInfor* getRouteInfor(Vertex*) const;

    void deleteVertex(); //for destructor
    bool checkNeighbor(Vertex*) const;
    void addNeighbor(Vertex*);
    void addRoute(RouteInfor*);

    vector<RouteInfor*> getRoutingTable() const;
    vector<Vertex*>     getNeighbors() const;
    Vertex* getNeighbor(unsigned id) const; //return null if there is no neighbor with this id
    Ort*    getPlace() const;
    Vertex* getVertex(unsigned id); // get Vertex with the id in routing table
};

#endif // VERTEX_H
