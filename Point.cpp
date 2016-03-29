//
// Created by bhuwan sapkota on 2/16/16.
//

#include "Point.h"
#include "Exceptions.h"
#include <cmath>
#include <sstream>



using namespace Clustering;


unsigned int Point::__idGen = 0;
const char Point::POINT_VALUE_DELIM = ',';

void Point::rewindIdGen() {
    __idGen--;
}

Point::Point(unsigned int dim)
{
    if(dim == 0)
        throw ZeroDimensionsEx(); // exception if dimensions is zero
    __id = __idGen;
    ++__idGen;

    __dim = dim;
    __values = new double[__dim];
    for (int i = 0; i < __dim; ++i) {
        __values[i] = 0.0;
    }
}

Point::Point(unsigned int dim, double * array)
{
    __id = __idGen;
    ++__idGen;

    __dim = dim;
    __values = new double[__dim];
    for (int i = 0; i < __dim; ++i) {
        __values[i] = array[i];
    }
}

Point::Point(const Point & entry)
{
    __id = entry.__id;
    __dim = entry.__dim;
    __values = new double[__dim];
    for (int i = 0; i < __dim; ++i)
    {
        __values[i] = entry.__values[i];
    }
}


Point &Point::operator=(const Point & entry)
{
    if(__dim != entry.__dim)
        throw DimensionalityMismatchEx(__dim,entry.__dim);
    if(this != &entry)
    {
        delete[] __values;
        __id = entry.__id;
        __dim = entry.__dim;
        __values = new double[__dim];
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] = entry.__values[i];
        }
    }
    return *this;
}


Point::~Point()
{
    if(__values != nullptr)
    {
        delete [] __values;
    }

}


int Point::getId() const {

    return __id;
}

unsigned int Point::getDims() const {
    return __dim;
}

void Point::setValue(unsigned int index, double value) {
    if(index > __dim-1)
        throw OutOfBoundsEx(__dim,index);
    __values[index]= value;
}

double Point::getValue(unsigned int index) const
{
    if(index > __dim-1)
        throw OutOfBoundsEx(__dim,index);
    return __values[index];
}

double Point::distanceTo(const Point & PointTo) const
{
    if(__dim != PointTo.__dim)
        throw DimensionalityMismatchEx(__dim,PointTo.__dim);
    double SqreOfDistance =0; //S = sqrt(x2-x1)^2+ (y2-y1)^2........
    for (int i = 0; i < __dim; ++i)
    {
        // (x2-x1)^2
        SqreOfDistance +=((PointTo.__values[i] - __values[i]) *  (PointTo.__values[i] - __values[i]));
    }
    return (sqrt(SqreOfDistance));
}

Point & Point::operator *=(double multplier) {
    for (int i = 0; i < __dim; ++i)
        __values[i] = ((__values[i] * multplier));
    return *this;
}

Point & Point::operator /=(double divider) {
    if (divider != 0.0)
    {
        for (int i = 0; i < __dim; ++i)
            __values[i] = (__values[i] / divider);
    }
    return *this;
}

const Point Point::operator*(double multiplier) const {

    for (int i = 0; i < __dim; ++i) {
        __values[i] *=multiplier;
    }
    return *this ;
}

const Point Point::operator/(double divider) const {

    for (int i = 0; i < __dim; ++i) {
        __values[i] /=divider;
    }
    return *this;
}

double& Point::operator[](unsigned int index) {
    if(index > __dim-1)
        throw OutOfBoundsEx(__dim,index);
    return __values[index];
}

const double& Point::operator[](unsigned int index) const {
    if(index > __dim-1)
        throw OutOfBoundsEx(__dim,index);
    return __values[index];
}


Point& Clustering::operator+=(Point& lhs, const Point& rhs)
{
    if(lhs.__dim != rhs.__dim)
        throw DimensionalityMismatchEx(lhs.__dim,rhs.__dim);
    for (int i = 0; i < lhs.__dim; ++i) {
        lhs.__values[i] += rhs.__values[i];
    }
    return lhs;
}

Point& Clustering::operator-=(Point& lhs, const Point& rhs)
{
    if(lhs.__dim != rhs.__dim)
        throw DimensionalityMismatchEx(lhs.__dim,rhs.__dim);
    for (int i = 0; i < lhs.__dim; ++i) {
        lhs.__values[i] -=rhs.__values[i];
    }
    return lhs;
}

Point const Clustering::operator+(const Point& lhs, const Point& rhs)
{
    Clustering::Point temp(lhs);
    temp +=rhs;
    return temp;

}

Point const Clustering::operator-(const Point& lhs, const Point& rhs)
{
    Clustering::Point temp(lhs);
    temp -=rhs;
    return temp;

}

bool Clustering::operator==(const Point &a, const Point &b) {
    if(a.__dim != b.__dim)
        throw DimensionalityMismatchEx(a.__dim,b.__dim);
    bool answer =false;
        if (a.__id == b.__id) {
            if (a.__dim == b.__dim) {
                answer = true;
                for (int i = 0; i<a.__dim; ++i) {
                    if (a.__values[i] != b.__values[i]) {
                        answer = false;
                        break;
                    }
                }
            }
        }
    return answer;
}

bool Clustering::operator!=(const Point& a, const Point &b)
{
    return !(a == b);
}

bool Clustering::operator<(const Point& a, const Point &b){
    if(a.__dim != b.__dim)
        throw DimensionalityMismatchEx(a.__dim,b.__dim);
    if (a != b) {
        if (a.__dim == b.__dim) {
            for (int i = 0; i < a.__dim; ++i) {
                if (a.__values[i] > b.__values[i]) {
                    return false;
                }
                else if (a.__values[i] < b.__values[i]) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Clustering::operator>(const Point& a, const Point &b){
    return (b < a);
}

bool Clustering::operator>=(const Point& a, const Point &b){
    return !(a < b);
}
bool Clustering::operator<=(const Point& a, const Point &b) {
    return !(a > b);
}

std::ostream & Clustering::operator<<(std::ostream & output, const Point & somePoint){

    for (int i = 0; i < somePoint.__dim; ++i) {
        output << somePoint.__values[i];
        if(i !=(somePoint.__dim-1))
            output << ", ";
    }
    return output;
}

std::istream & Clustering::operator >>(std::istream & input, Point & somePoint) {

    std::string temp;
    getline(input,temp);
        // checking file line match the dimensions of Points
        int n = std::count(temp.begin(), temp.end(), Point::POINT_VALUE_DELIM);
        if((n+1)!=somePoint.__dim)
            throw DimensionalityMismatchEx((n+1),somePoint.__dim);

    std::stringstream line(temp);
    for (int i = 0; i < somePoint.__dim; ++i) {
        getline(line, temp, Point::POINT_VALUE_DELIM); // error in the temp but still working
        std::stringstream ss(temp);
        ss >> somePoint.__values[i];
    }
    return input;
}
//done