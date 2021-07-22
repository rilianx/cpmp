
#include <map>
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

#include "Layout.h"

#ifndef _GREEDY_
#define _GREEDY_

using namespace std;
using namespace cpmp;


namespace cpmp {

    bool SF_move(Layout& layout, int pos=0);
    pair<int,int> _SF_move(Layout& layout, int pos=0);
    bool SD_move(Layout& layout, int stack=-1);
    int select_dismantling_stack(Layout& layout);
    pair<int, bool> select_destination_stack(Layout& layout, int orig, set<int> black_list=set<int>(), int max_pos=100, list<int> rank=list<int>());
    void iter_greedy(Layout& layout);
    int greedy_solve(Layout& layout, int step_limit=1000);
    int lazy_greedy(Layout& layout);
    bool atomic_move(Layout& layout, int s_o);

    /** Pixie components **/

    /* Stack dismantling++:
    * Attempts to place elements of s_o in other stacks in an "intelligent way"
    * gvalues of removed elements are placed in gv2index, in order to refill s_o with the same element values
    */
    bool SDpp(Layout& layout, int& s_o, multimap <int ,int>& gv2index);
    bool SFpp(Layout& layout, int s_d, multimap <int ,int>& gv2index);
    void iter_pixie(Layout& layout);
    int pixie_solve(Layout& layout, int step_limit);

}


#endif