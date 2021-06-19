#ifndef ROUTEINFOR_H
#define ROUTEINFOR_H
#include <vector>

class Vertex;

class RouteInfor
{
private:
    Vertex* destination;
    double distance;
    RouteInfor* predecessor;
public:
    RouteInfor(Vertex*, double);

    void setDistance(double);
    void setDestination(Vertex*);
    void setPredecessor(RouteInfor*);

    double getDistance() const;
    Vertex* getDestination() const;
    RouteInfor* getPredecessor() const;
};

#endif // ROUTEINFOR_H
