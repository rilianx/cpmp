
#include <set>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "Greedy.h"

#ifndef _BSG_
#define _BSG_

using namespace std;
using namespace cpmp;


namespace cpmp {
    enum neighbourhood{ATOMIC_MOVE, SD_MOVE} ;

    void generate_candidates(const Layout& lay, int type=ATOMIC_MOVE);
    int BSG(Layout& layout, int w=4, int type=ATOMIC_MOVE);
}


#endif