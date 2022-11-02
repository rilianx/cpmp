
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
    enum neighbourhood{ATOMIC, COMPOUND, MIXED} ;

    void generate_candidates(const Layout& lay,  list<Layout>& C, int type=ATOMIC, int k=2);
    int BSG(Layout& layout, int w, int type, Layout& best_lay);


    static bool PIXIE;
}


#endif