#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

#include <sstream>
#include <iostream>

#include "CStopWatch.h"

#include <boost/mpi.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/environment.hpp>

namespace mpi = boost::mpi;

using std::vector;

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<int> myDist{1, 10};

#define MASTER 0

int rows = 4;
int cols = 4;
int p = 1;

void vectorGen(vector<int>& x, int size) {
    x.clear();
    for (int i = 0; i < size; i++) {
        x.push_back(myDist(rng));
    }
}

void matrixMult(vector<int> x, vector<int> y, vector<int>& res) {
    //x is a flattened 2d vector and y is a 1d vector
    res.clear();
    for (int i = 0; i < x.size()/cols; i++) {
        int temp = 0;
        for (int j = 0; j < cols; j++) {
            temp += (x[(i*cols)+j] * y[j]);
        }

        res.push_back(temp);
    }
}

void displayMat(vector<int> x) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << std::setw(5) << x[(i * cols) + j];
        }
        std::cout << '\n';
    }
}

void displayVec(vector<int> x) {
    for (int i = 0; i < x.size(); i++) {
        std::cout << std::setw(5) << x[i];
    }
    std::cout << '\n';
}

bool assertMatVecMult(vector<int> x, vector<int> y, vector<int> w) {
    vector<int> z;

    matrixMult(x, y, z);

    for (int i = 0; i < w.size(); i++) {
        if (w[i] != z[i]) {
            return false;
        }
    }
    return true;
}

void parallelMatVecMult() {
    int numProcs, myRank, mySize, my2DSize;

    mpi::communicator world;

    // if (world.rank() == 0) {
    //     rows = dimX;
    //     cols = dimY;
    // }

    world.barrier();

    myRank = world.rank();
    numProcs = world.size();
    my2DSize = (rows/numProcs) * cols;
    mySize = (rows/numProcs);

    CStopWatch comm_timer, calc_timer, total_timer;
    
    double comm_time = 0, calc_time = 0, local_calc_time = 0;
    double scatter_time = 0, gather_time = 0, bcast_time = 0;

    if (myRank == 0) {
        total_timer.startTimer();
    }

    vector<int> x, myX, y, results, myRes;

    x.resize(rows*cols);
    y.resize(cols);
    myX.resize(my2DSize);
    results.resize(rows);
    myRes.resize(mySize);

    if (myRank == 0) {
        //master process
        calc_timer.startTimer();
        vectorGen(x, rows*cols);
        calc_timer.stopTimer();
        local_calc_time += calc_timer.getElapsedTime();

        calc_timer.startTimer();
        vectorGen(y, cols);
        calc_timer.stopTimer();
        local_calc_time += calc_timer.getElapsedTime();
    }

    world.barrier();

    comm_timer.startTimer();
    mpi::broadcast(world, y.data(), y.size(), 0);
    comm_timer.stopTimer();
    comm_time += comm_timer.getElapsedTime();
    bcast_time += comm_timer.getElapsedTime();

    comm_timer.startTimer();
    mpi::scatter(world, x.data(), myX.data(), my2DSize, 0);
    comm_timer.stopTimer();
    comm_time += comm_timer.getElapsedTime();
    scatter_time += comm_timer.getElapsedTime();

    calc_timer.startTimer();
    matrixMult(myX, y, myRes);
    calc_timer.stopTimer();
    calc_time += calc_timer.getElapsedTime();

    comm_timer.startTimer();
    mpi::gather(world, myRes.data(), mySize, results.data(), 0);
    comm_timer.stopTimer();
    comm_time += comm_timer.getElapsedTime();
    gather_time += comm_timer.getElapsedTime();

    if (myRank == 0) {
        //master process
        total_timer.stopTimer();
        std::cout << numProcs << ", " << comm_time << ", " << calc_time << ", "
                  << scatter_time << ", " << gather_time << ", " << bcast_time << ", "
                  << total_timer.getElapsedTime()/numProcs << ", "
                  << total_timer.getElapsedTime() << ", " << rows << ", " << cols 
                  << (assertMatVecMult(x, y, results) ? ", correct" : ", incorrect") << '\n';

    }
}

int main(int argc, char **argv) {
    mpi::environment env;
    rows = atoi(argv[1]);
    cols = atoi(argv[2]);

    parallelMatVecMult();

    return 0;
}
