//
// Created by bhuwan sapkota on 3/21/16.
//

#include "KMeans.h"
#include "Cluster.h"
#include "Point.h"
#include "Exceptions.h"
#include <fstream>

Clustering::KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
    //file streams
    __iFileName = filename;
    std::ifstream fileStream;
    fileStream.open(__iFileName);
    __dimensionality = dim;

    // number of cluster
    __k = k;
    if(__k == 0)
        throw ZeroClustersEx();


    // allocating clusters and reading file in first cluster
    if(!fileStream.good() || __iFileName =="")
        throw DataFileOpenEx(__iFileName);
    __clusters = new Cluster*[__k];
    for (int i = 0; i < __k; ++i) {
        __clusters[i] = new Cluster(__dimensionality);
    }
    fileStream >> *__clusters[0];


    Point tempP(__dimensionality);
    __initCentroids = new Point*[__k];
    for (int j = 0; j < k; ++j) {
        __initCentroids[j] = new Point(__dimensionality);
    }
    __clusters[0]->pickCentroids(k,__initCentroids); // I don't know what's right anymore.
    for (int l = 0; l < __k; ++l) {
        __clusters[l]->centroid.set(*__initCentroids[l]);
    }


    __maxIter = maxIter;
    __numIter= 0;
    __numNonempty=1;
    __numMovesLastIter=0;
}

Clustering::KMeans::~KMeans() {
    for (int i=0; i<__k; i++)
        delete __clusters[i];
    delete [] __clusters;
    for (int i = 0; i < __k; ++i) {
        delete __initCentroids[i];
    }
    delete [] __initCentroids;

}

unsigned int Clustering::KMeans::getMaxIter() {
    return __maxIter;
}

unsigned int Clustering::KMeans::getNumIters() {
    return __numIter;
}

unsigned int Clustering::KMeans::getNumNonemptyClusters() {
    return __numNonempty;
}

unsigned int Clustering::KMeans::getNumMovesLastIter() {
    return __numMovesLastIter;
}

Clustering::Cluster &Clustering::KMeans::operator[](unsigned int u) {
    return *__clusters[u]; // TODO not sure;
}

const Clustering::Cluster &Clustering::KMeans::operator[](unsigned int u) const {
    return *__clusters[u]; // TODO not sure;
}

std::ostream &Clustering::operator<<(std::ostream &os, const Clustering::KMeans &kmeans) {
    for (int i = 0; i < kmeans.__k;  ++i) {
        os << kmeans[i]; //TODO not sure
    }
    return os;
}

void Clustering::KMeans::run() {

    int moves = 100;
    int iter = 0;
    while(moves > 0 && iter < __maxIter){
        moves = 0;
        for (int i = 0; i < __k; ++i) {
            for (int j = 0; j < __clusters[i]->getSize(); ++j) { // TODO
                int closestClust = 0;
                Point currentP(__dimensionality);
                currentP = __clusters[i]->operator[](j);
                double distance = currentP.distanceTo(__clusters[closestClust]->centroid.get());
                for (int k = 1; k < __k ; ++k) {
                    if(distance > currentP.distanceTo(__clusters[k]->centroid.get())) {
                        distance = currentP.distanceTo(__clusters[k]->centroid.get());
                        closestClust = k;
                    }
                }
                if(i != closestClust) {
                    Cluster::Move(currentP, *__clusters[i], *__clusters[closestClust]);
                    moves++;
                }
            }
        }
        for (int l = 0; l < __k; ++l) {
            if(!__clusters[l]->centroid.isValid())
                __clusters[l]->centroid.compute();
        }
        iter++;
    }
    __numIter = iter;
    __numMovesLastIter = moves;

    for (int m = 1; m <__k ; ++m) {
        if(__clusters[m]->getSize()!= 0)
            __numNonempty++;
    }
}
//done