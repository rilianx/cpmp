
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

    void generate_candidates(const Layout& lay, int type=ATOMIC_MOVE, bool PIXIE=true);
    int BSG(Layout& layout, int w, int type, Layout& best_lay, bool PIXIE=true);

    void recreate(const Layout& lay, list<int>& bsg_moves);

    static bool PIXIE;
}


#endif