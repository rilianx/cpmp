
#include "Greedy.h"

#include <set>
#include <utility>

using namespace std;

using namespace cpmp;

namespace cpmp {


bool SF_move(Layout& layout, int pos){
    pair<int,int> move = _SF_move(layout, pos);
    if (move.first==-1) return false;

    layout.move(move.first,move.second);
    return true;
}

pair<int,int> _SF_move(Layout& layout, int pos){
    list <pair <int, pair <int,int> > > actions; /* (eval, (s_o,s_d))*/

    for(int i=0; i<layout.size(); i++){
        int h = layout.stacks[i].size();
        if(layout.is_sorted(i) && h < Layout::H){
            
            //stacks with H-1 containers are not considered
            //if there are 3 or less not-full stacks
            if(h==Layout::H-1 && layout.full_stacks >= layout.size()-3) continue;

            int top=Layout::gvalue(layout.stacks[i]);
            for (int k=0; k<layout.size(); k++){
                h = layout.stacks[k].size();
                if (k!=i && (!layout.is_sorted(k) || (h==Layout::H && layout.full_stacks >= layout.size()-2)) ){
                    if (layout.stacks[k].back() <= top) 
                        actions.push_back( make_pair(top - layout.stacks[k].back(), make_pair(k, i)));
                }
            }
        }
    }
    //cout << actions.size() << endl;
    actions.sort();

    if (actions.size()>pos){
        auto it = actions.begin();
        advance(it,pos);
        return it->second;
    }
    
    return make_pair(-1,-1);
}

int select_dismantling_stack(Layout& layout){
    double best_ev = 0.0; int s_o=-1;
    for (int i=0;i<layout.size(); i++){
        if(layout.dismantled_stacks.find(i) != layout.dismantled_stacks.end()) 
            continue; /* the stack was dismantled before */

        double prom = (double) layout.sum_stack(i) / (double) layout.stacks[i].size();
        double ev = 100000.0 - 100*layout.stacks[i].size() - prom;
    
        if (ev > best_ev){
            best_ev = ev;
            s_o = i;
        }
    }

    return s_o;
}

bool SD_move(Layout& layout, int s_o){

    if( layout.dismantled_stacks.find(s_o) != layout.dismantled_stacks.end() ) 
        return false; /* the stack was dismantled before */

    if (s_o==-1) s_o = select_dismantling_stack(layout);

    if (s_o==-1 || layout.stacks[s_o].size() == 0) return false;


    layout.dismantled_stacks.insert(s_o);
    while (layout.stacks[s_o].size()>0){
        int s_d = select_destination_stack(layout,s_o).first;
        layout.move(s_o,s_d);
        if (layout.reachable_height(s_o)==Layout::H) return true; //partial dismantling
    }
        
    return true; //full dismantling
}

pair<int, bool> select_destination_stack(Layout& layout, int orig, set<int> black_list, int max_pos, list<int> rank){
    auto& s_o = layout.stacks[orig];
    int c = s_o.back();
    int best_eval=-1000000;
    int best_dest=-1;
    bool best_xg = false;
    for (int dest=0; dest <layout.size(); dest++){
        if(orig==dest || black_list.find(dest) != black_list.end() ) continue;
        auto& s_d = layout.stacks[dest];
        if(Layout::H == s_d.size()) continue;

        int top_d = Layout::gvalue(s_d);
        int ev=0; bool xg=false;

        if(layout.is_sorted(dest) && c<=top_d){
            //c can be well-placed: the sorted stack minimizing top_d is preferred.
            ev = 100000 - 100*top_d; xg=true;
         }else if (!layout.is_sorted(dest) && c>=top_d){
            //unsorted stack with c>=top_d maximizing top_d is preferred
            ev = top_d;
        }else if(layout.is_sorted(dest)){
            //sorted with minimal top_d
            ev = -100 - s_d.size();
        }else{
            //unsorted with minimal number of auxiliary stacks
            ev = -10000  - 100*s_d.size() - top_d;
        }

        if (layout.H - s_d.size() > max_pos)
            ev -= 100000;

         if (ev > best_eval){
                best_eval=ev;
                best_dest=dest;
                best_xg=xg;
         }  
        
    }

    return make_pair(best_dest, best_xg);
}

void iter_greedy(Layout& layout){
    if (!SF_move(layout))
        SD_move(layout);
}

int greedy_solve(Layout& layout, int step_limit){
    while (layout.unsorted_stacks>0 && layout.steps < step_limit){
        //cout << layout.unsorted_stacks << endl;
        //layout.print(); cout << endl;
        int steps_old=layout.steps;
        iter_greedy(layout);
        if (layout.steps==steps_old) return -1;
    }
    if(layout.steps >= step_limit ) return -1;

    return layout.steps;
}

int lazy_greedy(Layout& layout){
    while (layout.unsorted_stacks>0 && SF_move(layout));
    return layout.steps;
}

bool atomic_move(Layout& layout, int s_o){
    if (layout.stacks[s_o].size() == 0) return false;

    pair<int,bool> s_dd = select_destination_stack(layout, s_o);
    layout.move(s_o, s_dd.first);
    return true;
}

}