
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
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

    bool SF_move(Layout& layout, double a=1.2, double b=0.6);
    void get_SF_actions(Layout& layout, list <pair <double, pair <int,int> > >& actions, double a=1.2, double b=0.6);
    pair<int,int> _SF_move(Layout& layout, double a=1.2, double b=0.6);
    bool SD_move(Layout& layout, int stack=-1);
    int select_dismantling_stack(Layout& layout);
    int select_destination_stack(Layout& layout, int orig, set<int> black_list=set<int>());
    int get_destination_stack(Layout& layout, int orig, set<int> black_list);

    void eval_destination_stacks(Layout& layout, int orig, list< pair<double, pair < int, int> > >& actions, set<int> black_list);
    int ev_dest_stack(const Layout& layout, int dest, int c);
    
    bool iter_greedy(Layout& layout, double a=1.2, double b=0.6);
    void atomic_iter_greedy(Layout& layout, double a=1.2, double b=0.6);
    int greedy_solve(Layout& layout, int step_limit=1000, double a=1.2, double b=0.6);
    int greedy_solve2(Layout& layout, int step_limit=1000, double a=1.2, double b=0.6);
    void greedy_eval(Layout& layout, list < pair < double , pair <int, int> > >& actions, double a=1.2, double b=0.6);
    int lazy_greedy(Layout& layout);
    pair<int,int> atomic_move(Layout& layout, int s_o);
    list <pair <int,int> > atomic_moves(Layout& layout, int s_o, int k=1);
    void reduce(Layout& layout, int s_r);
    bool stop_reduction(Layout& layout, int s_o);

    //coumpute lds[i] of the large of the longest decreasing sequence for each sequence [i..n]
    void LDS(const std::vector<int>& v, std::vector<int>& lds);
    bool create_seq(vector<int> v, vector<int>& seq, int min_sz);


}


#endif