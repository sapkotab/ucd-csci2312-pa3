//
// Created by bhuwan sapkota on 3/21/16.
//

#include "Exceptions.h"

using namespace Clustering;

OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
    __current = c;
    __rhs = r;
    __name = "OutOfBoundsEx";
}

unsigned int OutOfBoundsEx::getCurrent() const {
    return __current;
}

int OutOfBoundsEx::getRhs() const {
    return __rhs;
}

std::string OutOfBoundsEx::getName() const {
    return __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
    os << ex.__current << " " << ex.__rhs << " " << ex.__name << std::endl;
    return os;
}

DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
    __current = c;
    __rhs = r;
    __name = "DimensionalityMismatchEx";
}

unsigned int DimensionalityMismatchEx ::getCurrent() const {
    return __current;
}

unsigned int DimensionalityMismatchEx::getRhs() const {
    return __rhs;
}

std::string DimensionalityMismatchEx::getName() const {
    return __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {
    os << ex.__current << " " << ex.__rhs << " " << ex.__name << std::endl;
    return os;
}

ZeroClustersEx::ZeroClustersEx() {
    __name = "ZeroClustersEx";
}

std::string ZeroClustersEx::getName() const {
    return  __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const ZeroClustersEx &ex) {
    os << ex.__name << std::endl;
    return os;
}

DataFileOpenEx::DataFileOpenEx(std::string filename) {
    __filename = filename;
    __name = "DataFileOpenEx";
}

std::string DataFileOpenEx::getFilename() const {
    return __filename;
}

std::string DataFileOpenEx::getName() const {
    return __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const DataFileOpenEx &ex) {
    os /*<< ex.__filename << " " */<< ex.__name << std::endl;
    return os;
}

ZeroDimensionsEx::ZeroDimensionsEx() {
    __name = "ZeroDimensionsEx";
}

std::string ZeroDimensionsEx::getName() const {
    return __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {
    os << ex.__name << std::endl;
    return os;
}

EmptyClusterEx::EmptyClusterEx() {
    __name = "EmptyClusterEx";
}

std::string EmptyClusterEx::getName() const {
    return __name;
}

std::ostream &Clustering::operator<<(std::ostream &os, const EmptyClusterEx &ex) {
    os << ex.__name  << std::endl;
    return os;
}
