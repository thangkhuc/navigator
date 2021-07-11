#include "vertex.h"

Vertex::~Vertex()
{
    delete place;

    for (unsigned i = 0; i < neighbors.size(); i++)
        delete neighbors[i];

    for (unsigned i = 0; i < routingsTable.size(); i++)
        delete routingsTable[i];
}

void Vertex::deleteVertex()
{
    for (unsigned i = 0; i < routingsTable.size(); i++)
        delete routingsTable[i];
}

Vertex::Vertex(Ort* place) : place(place) {}

bool Vertex::checkNeighbor(Vertex* _nachbar) const
{
    for (unsigned i = 0; i < neighbors.size(); i++) {
        if (neighbors[i]->getPlace()->getID() == _nachbar->getPlace()->getID())
            return true;
    }
    return false;
}

vector<Vertex*> Vertex::getNeighbors() const {return this->neighbors;}

Vertex* Vertex::getNeighbor(unsigned id) const
{
    for (unsigned i = 0; i < this->neighbors.size(); i++) {
        if (neighbors[i]->getPlace()->getID() == id)
            return this->neighbors[i];
    }
    return nullptr;
}

void Vertex::addNeighbor(Vertex* newNeighbor) {this->neighbors.push_back(newNeighbor);}

Ort* Vertex::getPlace() const {return this->place;}

void Vertex::addRoute(RouteInfor* newRoute) {this->routingsTable.push_back(newRoute);}

vector<RouteInfor*> Vertex::getRoutingTable() const {return this->routingsTable;}

void Vertex::popRoute() {this->routingsTable.pop_back();}

RouteInfor* Vertex::getRouteInfor(Vertex* vertex) const
{
    for (unsigned i = 0; i < this->routingsTable.size(); i++) {
        if (routingsTable[i]->getDestination()->getPlace()->getID() == vertex->getPlace()->getID())
            return routingsTable[i];
    }
    return nullptr;
}

Vertex* Vertex::getVertex(unsigned id)
{
    for (unsigned i = 0; i < routingsTable.size(); i++) {
        if (id == routingsTable[i]->getDestination()->getPlace()->getID())
            return routingsTable[i]->getDestination();
    }
    return nullptr;
}
