
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

//intenta mover el contenedor s_o[pos] al stack s_d
int force_move(Layout& layout, int s_o, int s_d, int gval){
    int pos=layout.stacks[s_o].size()-1;
    int cc=-1;
    while(layout.stacks[s_o][pos] != gval){
        set<int> black_list; black_list.insert(s_d);
        int s_tmp = select_destination_stack(layout, s_o, black_list).first;
        
        if (s_tmp==-1) return -1; //debería volver al etado original
        
        cc=layout.move(s_o,s_tmp);
        pos--;
    }
    
    gval=layout.move(s_o,s_d);
    return gval;
}

pair<int,int> search_highest(Layout& layout, int lb, int ub, int s_d){
    pair<int,int>  ret = make_pair(-1,-1);
    for (int pos=-1; pos>-10; pos--){
        int max_c = 0;
        for (int s=0; s<layout.size(); s++){
            if (s==s_d) continue;
            vector<int>& ss = layout.stacks[s];
            if (ss.size() < -pos) continue;
            int c=ss[ss.size()+pos];
            if (c>=lb && c <=ub && c>max_c){
                if (layout.sorted_elements[s] <= ss.size()+pos && layout.capacity(s,s_d) >= -pos-1){
                    max_c=c;
                    ret = make_pair(s,max_c);
                }
            }
        }
        if (ret.first != -1) return ret;
    }
    return ret;
}

bool SFpp(Layout& layout, int s_d, multimap <int ,int>& gv2index){
    int ub = Layout::gvalue(layout.stacks[s_d]);
    set<int> conts;
    auto it = gv2index.rbegin();
    for (;it!=gv2index.rend();it++){
        int r = it->first;
        //cout << r << endl;
        
        if (r>ub) continue;
        if (conts.find(r) != conts.end()){ 
            conts.erase(r);
            continue;
        }
        
        while (true){
            int c = -1;
            pair<int,int> so_sd = _SF_move(layout);
            int s_o = so_sd.first, s_dd =so_sd.second;

            //goal destination but c >= r
            if (s_o != -1 && s_d == s_dd && layout.stacks[s_o].back() >= r){
                c = layout.move(s_o,s_d);
                ub = c;
            }else if (s_o==-1 || s_d == s_dd){                
                pair<int,int> stack_gval = search_highest(layout, r, ub, s_d);
                int stack = stack_gval.first, gval=stack_gval.second;
                //cout << r << ", " << ub << endl;
                //cout << stack << "," << gval << endl;
                if (stack == -1) {ub = r-1; break;}
                
                //cout << "force_move" << stack << "," << gval <<"," << s_d << endl;
                c = force_move(layout, stack, s_d, gval);
                if (c != gval && ub == r) {cout << "force move: c!=gval" << endl; break;}
                ub = c;
            }else
                c = layout.move(s_o, s_dd);

            if (layout.stacks[s_d].size() == layout.H) return true;
            //if len(layout.stacks[s_d]) == layout.H-1 and layout.full_stacks >= len(layout.stacks)-3: return True
            
            if (c==r) break;
            conts.insert(c);
        }
    }
    return true;
}

int select_dismantling_stack(Layout& layout){
    double best_ev = 0.0; int s_o=-1;
    for (int i=0;i<layout.size(); i++){
        if(layout.dismantled_stacks.find(i) != layout.dismantled_stacks.end()) 
            continue; /* the stack was dismantled before */

        if (layout.is_sorted(i) && layout.capacity(i)<3*layout.stacks[i].size()) continue;

        double prom = (double) layout.sum_stack(i) / (double) layout.stacks[i].size();

        double ev = 0.0;
        if (layout.is_sorted(i))
            ev = 100000 - 1000*layout.stacks[i].size() - prom;
        else
            ev = 100000 - 1000*layout.stacks[i].size() + prom;
    
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


    //layout.dismantled_stacks.insert(s_o);
    while (layout.stacks[s_o].size()>0){
        int s_d = select_destination_stack(layout,s_o).first;
        layout.move(s_o,s_d);
        if (layout.reachable_height(s_o)==Layout::H) return true; //partial dismantling
    }
        
    return true; //full dismantling
}

int select_origin_stack(Layout& layout, int s_d, int blocked, multimap<int, int> gv2index){
    vector<int>& ss_d = layout.stacks[s_d];
    int top_d = Layout::gvalue(ss_d);
    int best_eval=-1000000;
    int best_s=-1;
    
    for (int s_o=0; s_o < layout.size(); s_o++){
            if (s_o==s_d || s_o==blocked) continue;
            vector<int>& ss_o = layout.stacks[s_o];

            if (ss_o.size()==0) continue;        

            int c=Layout::gvalue(ss_o);

            //if the element is in rank (gv2index) and it is located "too low", we try another one
            if(gv2index.find(c)!=gv2index.end() && gv2index.find(c)->second + 1 < Layout::H - ss_d.size())
                continue;

            int ev=0;

            if (layout.is_sorted(s_d) && c<=top_d)
                //c can be well-placed: the sorted stack maximizing c is preferred.
                ev = 10000 + 100*c;
            else if (!layout.is_sorted(s_d) && c>=top_d)
                //unsorted stack with c>=top_d minimizing c is preferred
                ev = -c;
            else
                ev = -100 - c;

            if (ev > best_eval){
                best_eval=ev;
                best_s=s_o;
            }
    }

    return best_s;
}

bool fill_stack(Layout& layout, int s_d, int n, int ori, multimap<int, int>& gv2index){
    for(int i=0; i<n; i++){
        int s_o = select_origin_stack(layout, s_d, ori, gv2index);
        if (s_o == -1) return false;
        layout.move(s_o,s_d);
    }
    return true;
}

void get_sorted_gvalues(vector<int> stack, multimap <int ,int>& gv2index){
    sort(stack.rbegin(), stack.rend()); 
    int i=0, k=0, c_old=-1;
    //same c are associated to the same index in gv2index
    for(int c : stack){
        if(c_old != c) k=i;
        gv2index.insert(make_pair(c,k));
        i++; c_old=c;
    }
}


bool SDpp(Layout& layout, int& s_o, multimap <int ,int>& gv2index){
        if( layout.dismantled_stacks.find(s_o) != layout.dismantled_stacks.end() ) 
            return false; /* the stack was dismantled before */

        if (s_o==-1) s_o = select_dismantling_stack(layout);

        if (s_o==-1 || layout.stacks[s_o].size() == 0) return false; 

        /* ranks: gvalue -> index  */
        get_sorted_gvalues(layout.stacks[s_o], gv2index);

    
        int capac = layout.capacity(s_o); /* espacio libre del layout */

        vector<int>& ss_o = layout.stacks[s_o];

        while (ss_o.size() > 0){
            int top = Layout::gvalue(ss_o);
            int slack = capac-ss_o.size();
            std::pair<int, bool> sd_xg = select_destination_stack(layout, s_o, std::set<int>(), gv2index.find(top)->second +1+slack);
            int s_d = sd_xg.first; bool xg=sd_xg.second;

            /* position of the container if move(s_o,s_d) */
            int pos = Layout::H - layout.stacks[s_d].size();

            /* the stack s_d should be refill if..*/
            if (gv2index.find(top)->second + 1 < pos - slack){ 
               

                bool success = fill_stack(layout, s_d, (pos - slack) - (gv2index.find(top)->second+1), s_o, gv2index);
                if(!success) return false;
            }
            capac --;
            int c = layout.move(s_o,s_d);
            if(xg && gv2index.find(c)!=gv2index.end()) gv2index.erase(gv2index.find(c));

            if (layout.reachable_height(s_o)>=Layout::H) {
                for (int c:ss_o) gv2index.erase(gv2index.find(c));
                return true; //partial dismantling
            }
        }
        return true;
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

void iter_pixie(Layout& layout){
    if (!SF_move(layout)){
        multimap <int ,int> gv2index;
        int s_o=-1;
        SDpp(layout,s_o,gv2index);
        SFpp(layout,s_o,gv2index);
    }
}

int pixie_solve(Layout& layout, int step_limit){
    while (layout.unsorted_stacks>0 && layout.steps < step_limit){
        //cout << layout.unsorted_stacks << endl;
        //layout.print(); cout << endl;
        int steps_old=layout.steps;
        iter_pixie(layout);
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