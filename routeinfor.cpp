#include "routeinfor.h"

RouteInfor::RouteInfor(Vertex* destination, double distance) : destination(destination), distance(distance) {}

Vertex* RouteInfor::getDestination() const {return this->destination;}

double RouteInfor::getDistance() const {return this->distance;}

void RouteInfor::setDistance(double distance) {this->distance = distance;}

RouteInfor* RouteInfor::getPredecessor() const {return this->predecessor;}

void RouteInfor::setPredecessor(RouteInfor* precedessor) {this->predecessor = precedessor;}
