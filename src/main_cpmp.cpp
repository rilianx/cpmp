
#include <map>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include<ctime>
#include "dirent.h"

#include "Layout.h"
#include "Greedy.h"
#include "Bsg.h"

using namespace std;
using namespace cpmp;

int main(int argc, char * argv[]){
    Layout::H = atoi (argv[1]);
    string path(argv[2]);
    int beams = atoi (argv[3]);
    Layout L(path);
    const clock_t begin_time = clock();
    int steps;
    if (beams==0) steps = greedy_solve(L,1000);
    else steps = BSG(L, beams, SD_MOVE);
    cout << steps <<"\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;

}