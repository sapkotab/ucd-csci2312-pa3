//
// Created by bhuwan sapkota on 2/18/16.
//

#include "Cluster.h"
#include "Exceptions.h"
#include <sstream>
#include <cassert>
#include <iomanip>

using namespace Clustering;
const char Cluster::POINT_CLUSTER_ID_DELIM = ':';
unsigned int Cluster::__idGenerator = 0;

LNode::LNode(const Point &p, LNodePtr n): point(p){
    next = n;
}

void Cluster::__del(){
    while(__points != nullptr){
        LNodePtr temp = __points->next;
        delete __points;
        __points = temp;
        __size--;
    }
    assert(__size==0);
}

void Cluster::__cpy(LNodePtr pts){
    if(pts == nullptr){
        __points = nullptr;
        __size = 0;
        return;
    }
    LNodePtr origCurrent = pts;
    LNodePtr current = __points = new LNode(origCurrent->point,nullptr);
    while (origCurrent->next != nullptr) {
        origCurrent = origCurrent->next;
        current->next = new LNode(origCurrent->point,nullptr);
        current =current->next;

    }
}

bool Cluster::__in(const Point & entryPoint) const
{
    if(this->getDimensionality() != entryPoint.getDims())
        throw DimensionalityMismatchEx(this->getDimensionality(), entryPoint.getDims());
    bool in = false;
    LNodePtr current = __points;
    while (current !=nullptr) {
        if (current->point == entryPoint) {
            in = true;
            break;
        }
        current = current->next;
    }
    return in;
}


Cluster::Centroid::Centroid(unsigned int d, const Cluster &c):__p(Point(d)),__c(c) {
    __dimensions = d;
    __valid = false;
}

// getters/setters
const Point Cluster::Centroid::get() const {
    return __p;
}
void Cluster::Centroid::set(const Point &p){
    __p = p;
    setValid(true);
}
bool Cluster::Centroid::isValid() const{
    return __valid;
}
void Cluster::Centroid::setValid(bool valid){
    __valid = valid;
}

// functions
void Cluster::Centroid::compute(){ // this only average the point and assign to __p, doesn't do anything else

    Point sumOfPoint(__c.getDimensionality());
    Point cntrd(__c.getDimensionality());
    for (int i = 0; i < __c.getSize(); ++i) {
        sumOfPoint +=__c[i];
    }
    cntrd =(sumOfPoint/__c.getSize());
    set(cntrd);

}
bool Cluster::Centroid::equal(const Point & rhs) const{
    bool answer =false;
    if (__dimensions == rhs.getDims()) {
        answer = true;
        for (int i = 0; i<__dimensions; ++i) {
            if(__p.getValue(i)==__p.getValue(i)) { // making sure infinity will not be compared
                                                    // infinity is not equal to itself C++ 11
                if (__p.getValue(i) != rhs.getValue(i)) {
                    answer = false;
                    break;
                }
            }
        }
    }
    return answer;
}
void Cluster::Centroid::toInfinity(){
    for (int i = 0; i < __dimensions; ++i) {
        __p.setValue(i,std::numeric_limits<double>::max());
    }
}

Cluster::Cluster(unsigned int d):centroid(d,*this)
{
    __dimensionality = d;
    __size = 0;
    __points = nullptr;
    __id = __idGenerator;
    __idGenerator++;
}

Cluster::Cluster(const Cluster & entry):centroid(entry.__dimensionality,*this)
{
    if(this != &entry)
    {
        __dimensionality= entry.__dimensionality;
        __size = entry.__size;
        __cpy(entry.__points);
        __id = entry.__id;
        centroid.compute();
    }
}

Cluster &Cluster::operator=(const Cluster &entry) {
if(this->getDimensionality() != entry.getDimensionality())
    throw DimensionalityMismatchEx(this->getDimensionality(),entry.getDimensionality());
    if(this != &entry)
    {
        __del();
        __dimensionality= entry.__dimensionality;
        __size = entry.__size;
        __cpy(entry.__points);
        __id = entry.__id;
        centroid.compute();
    }
    return *this;
}

Cluster::~Cluster()
{
    __del();
}

unsigned int Cluster::getSize() const {
    return __size;
}
unsigned int Cluster::getDimensionality() const{
    return __dimensionality;
}
unsigned int Cluster::getId() const{
    return __id;
}

void Cluster::add(const Point & point){
    // throwing exception for dimentionality mixmatch if there is already
    // a point in cluster.
        if(this->getDimensionality() != point.getDims())
            throw DimensionalityMismatchEx(this->getDimensionality(), point.getDims());


    LNodePtr newNode = new LNode(point,nullptr);
    if (__points == nullptr) {
        __points = newNode;
    }
    else{
        LNodePtr current = __points;
        LNodePtr prev = nullptr;
        while (current !=nullptr) {
            if (current->point > point) {
                break;
            }
            else{
                prev = current;
                current = current->next;
            }
        }
        if (current == __points) {
            newNode->next = __points;
            __points = newNode;
        }
        else{
            newNode->next = current;
            prev->next = newNode;
        }

    }
    __size++;
    centroid.setValid(false);
}

const Point &Cluster::remove(const Point &point) {
    if(this->getDimensionality() != point.getDims())
        throw DimensionalityMismatchEx(this->getDimensionality(), point.getDims());
    LNodePtr prev = nullptr;
    LNodePtr current = __points;
    if (current !=nullptr && current->point == point)
    {
        __points = __points->next;
        delete current;
        __size--;
        centroid.setValid(false);
        return point;
    }
    while (current != nullptr && point != current->point)
    {
        prev =current;
        current = current->next;
    }
    prev->next = current->next;
    delete current;
    __size--;
    centroid.setValid(false);
    return point;
}

bool Cluster::contains(const Point &point) const {
    bool answer = __in(point);
    return answer;
}
void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
    LNodePtr current = __points;
    if(k>=__size){
        for (int i = 0; i < __size; ++i) {
            *pointArray[i]= current->point;
            current = current->next;
        }
        Point temp(getDimensionality());
        for (int l = 0; l < getDimensionality(); ++l) {
            temp[l]=std::numeric_limits<double>::max();
        }
        for (int j =__size; j < k ; ++j) {
            *pointArray[j]= temp;
        }
    }
    else // TODO apply this based on distance later
        for (int i = 0; i < k; ++i) {
            *pointArray[i]=(current->point);
            current = current->next;
        }

}

const Point &Cluster::operator[](unsigned int index) const {
    if(__size == 0)
        throw EmptyClusterEx();// zero member on cluster exception
    if(index > __size-1)
        throw OutOfBoundsEx(__size,index); //out of bound exception
    LNodePtr current = __points;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->point;
}

Cluster &Cluster::operator+=(const Point &point) {
    if(!__in(point)) {
        add(point);
    }
    return *this;
}

Cluster &Cluster::operator-=(const Point &point) {
    if(__in(point))
    {
        remove(point);
    }
    return *this;
}

Cluster &Cluster::operator+=(const Cluster &cluster) {

    LNodePtr currentAdd = cluster.__points;

    while (currentAdd)
    {
        if(!contains(currentAdd->point))
        {
            add(currentAdd->point);
        }
        currentAdd = currentAdd->next;
    }
    return *this;
}

Cluster &Cluster::operator-=(const Cluster &cluster) {
    LNodePtr currentAdd = cluster.__points;
    while (currentAdd != nullptr){
        if(__in(currentAdd->point)){
            remove(currentAdd->point);
        }
        currentAdd = currentAdd->next;
    }
    return *this;
}

// Friends: IO
std::ostream &Clustering::operator<<(std::ostream & output, const Cluster & cluster){
    LNodePtr current =cluster.__points;
    while (current != nullptr){
        output << std::fixed << std::setprecision(5);
        output << current->point <<' ' << Cluster::POINT_CLUSTER_ID_DELIM;
        output << std::setprecision(0) <<' ' << cluster.getId() << std::endl;
        current = current->next;
    }
    return output;
}

std::istream &Clustering::operator>>(std::istream & input, Cluster & cluster){
    std::string s;
    while (getline(input,s) && input.good()) {
        int n = std::count(s.begin(), s.end(), Point::POINT_VALUE_DELIM);
        Point p(n+1);
        std::stringstream ss(s);
        ss >> p;
        if(p.getDims()==cluster.getDimensionality() ) {
            cluster.add(p);
        }
        else
            Point::rewindIdGen();
    }
    return input;

}

// Friends: Comparison

bool Clustering::operator==(const Cluster & lhs, const Cluster & rhs){
    if(lhs.getDimensionality() != rhs.getDimensionality())
        throw DimensionalityMismatchEx(lhs.getDimensionality() ,rhs.getDimensionality() );
    LNodePtr currLhs = lhs.__points;
    LNodePtr currRhs = rhs.__points;
    while (currLhs != nullptr && currRhs != nullptr) {
        if (currLhs->point != currRhs->point) {
            return false;
        }
        currLhs = currLhs->next;
        currRhs = currRhs->next;
    }
    if(currLhs !=nullptr || currRhs != nullptr)
        return false;
    else;
    return true;

}
bool Clustering::operator!=(const Cluster & lhs, const Cluster & rhs){
    return !(lhs == rhs);
}

// Friends: Arithmetic (Cluster and Point)
const Cluster Clustering::operator+(const Cluster & lhs, const Point & rhs){
    Cluster temp(lhs);
    temp +=rhs;
    return temp ;
}
const Cluster Clustering::operator-(const Cluster & lhs, const Point & rhs){
    Cluster temp(lhs);
    temp -=rhs;
    return temp ;
}

// Friends: Arithmetic (two Clusters)
const Cluster Clustering::operator+(const Cluster & lhs, const Cluster & rhs){
    Cluster temp(lhs);
    temp +=rhs;
    return temp;
}
const Cluster Clustering::operator-(const Cluster & lhs, const Cluster & rhs){
    Cluster temp(lhs);
    temp -= rhs;
    return temp ;
}

Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to):__p(p),__from(from),__to(to){
    to.add(p);
    from.remove(p);
    perform();
}

void Cluster::Move::perform() {
    __from.centroid.setValid(false);
    __to.centroid.setValid(false);
}
//done

