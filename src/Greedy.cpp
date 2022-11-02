
#include "Greedy.h"

#include <set>
#include <stack>
#include <utility>
#include <numeric>      // std::iota
#include <algorithm> 

using namespace std;

using namespace cpmp;

namespace cpmp {


bool SF_move(Layout& layout, double a, double b){
    pair<int,int> move = _SF_move(layout, a, b);
    if (move.first==-1) return false;
    layout.move(move.first,move.second);
    return true;
}

pair<int,int> best_BG_move(Layout& layout, double a, double b){
    int best_ev = 1000;
    pair<int,int> best_move = make_pair(-1,-1);
    for(int i=0; i<layout.size(); i++){
        int h = layout.stacks[i].size();
        if(layout.is_sorted(i) && h < Layout::H){          
            //stacks with H-1 containers are not considered
            //if there are 3 or less not-full stacks
            double avH=layout.total_elements/layout.size();

            if(h>=Layout::H-1 && a*avH >= layout.size()-layout.full_stacks ) continue;
            if(h>=Layout::H-1 && layout.size()-layout.full_stacks < 3 ) continue;


            int top=Layout::gvalue(layout.stacks[i]);
            for (int k=0; k<layout.size(); k++){
                h = layout.stacks[k].size();
                if (k!=i && (!layout.is_sorted(k))  || (h>=Layout::H &&  b*avH >= layout.size()-layout.full_stacks ) ||  
                (h>=Layout::H && layout.size()-layout.full_stacks < 3 ) ) {
                    if (layout.stacks[k].back() <= top) {
                        int ev = top - layout.stacks[k].back();
                        if(best_ev>ev){
                            best_ev=ev;
                            best_move = make_pair(k,i);
                        }
                    }
                }
            }
        }
    }
    return best_move;    
}

void get_SF_actions(Layout& layout, list <pair <double, pair <int,int> > >& actions, double a, double b ){

    for(int i=0; i<layout.size(); i++){
        int h = layout.stacks[i].size();
        if(layout.is_sorted(i) && h < Layout::H){          
            //stacks with H-1 containers are not considered
            //if there are 3 or less not-full stacks
            double avH=layout.total_elements/layout.size();

            if(h>=Layout::H-1 && a*avH >= layout.size()-layout.full_stacks ) continue;
            if(h>=Layout::H-1 && layout.size()-layout.full_stacks < 3 ) continue;


            int top=Layout::gvalue(layout.stacks[i]);
            for (int k=0; k<layout.size(); k++){
                h = layout.stacks[k].size();
                if (k!=i && (!layout.is_sorted(k))  || (h>=Layout::H &&  b*avH >= layout.size()-layout.full_stacks ) ||  
                (h>=Layout::H && layout.size()-layout.full_stacks < 3 ) ) {
                    if (layout.stacks[k].back() <= top) 
                        actions.push_back( make_pair(top - layout.stacks[k].back(), make_pair(k, i)));
                }
            }
        }
    } 
}

pair<int,int> _SF_move(Layout& layout, double a, double b){
    list <pair <double, pair <int,int> > > actions; /* (eval, (s_o,s_d))*/
    get_SF_actions(layout, actions, a, b);

    //cout << actions.size() << endl;
    actions.sort();

    if (actions.size()>0)
        return actions.front().second;
    else   
        return make_pair(-1,-1);
}




int select_dismantling_stack(Layout& layout){
    double best_ev = 0.0; int s_o=-1;
    for (int i=0;i<layout.size(); i++){
        if(layout.dismantled_stacks.find(i) != layout.dismantled_stacks.end()) 
            continue; /* the stack was dismantled before */

        //if (layout.is_sorted(i) && layout.capacity(i)<=2*layout.stacks[i].size()+1) continue;
        //if (layout.is_sorted(i) && layout.stacks[i].size()>2) continue;

        double prom = (double) layout.sum_stack(i) / (double) layout.stacks[i].size();

        double ev = 0.0;
        if (layout.is_sorted(i))
            ev = 100000 - 1000*layout.stacks[i].size() - prom ;
        else
            ev = 100000 - 1000*layout.stacks[i].size() + prom;
    
        if (ev > best_ev){
            best_ev = ev;
            s_o = i;
        }
    }

    return s_o;
}

void LDS(const std::vector<int>& v, std::vector<int>& lds)
{
    if (v.size() == 0) // boundary case
        return;
 
    std::vector<int> tail(v.size(), 0);
    int length = 1; // always points empty slot in tail
    lds[v.size()-1] = length;
 
    tail[0] = v[v.size()-1];
 
    for (int i = v.size()-2; i >=0; i--) {
 
        // Do binary search for the element in
        // the range from begin to begin + length
        auto b = tail.begin(), e = tail.begin() + length;
        auto it = lower_bound(b, e, v[i]);
 
        // If not present change the tail element to v[i]
        if (it == tail.begin() + length){
            tail[length++] = v[i];
        }else
            *it = v[i];
        lds[i] = length;
    }
}


bool create_seq(vector<int> v, vector<int>& seq, int min_sz){

    std::vector<int> lds(v.size());
    LDS(v, lds);
    if (lds[0]<min_sz) return false;



    bool finish = false;

    while(!finish){
        
        //idx <- argsort (v)
        std::vector<int> idx(v.size());
        // initialize original index locations
        iota(idx.begin(), idx.end(), 0);
        stable_sort(idx.begin(), idx.end(),
        [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});

        int i_ = -1;
        finish=true;
        for (int j=0; j<idx.size(); j++){
            int i=idx[j];
            if(i < i_) continue;
            if (lds[i] + seq.size() < min_sz) {
                //se elimina el item y se recalcula el vector LDS
                v.erase(v.begin()+i);
                if (i_!=-1) v.erase(v.begin(),v.begin()+i_+1);
                LDS(v, lds);
                finish=false;
                break;
            }
            
            seq.push_back(v[i]);
            i_=i;
        }
               
    }
    return true;
}




void smart_assignation(Layout& layout, int is_o, map< int, int >& assignation, set<int>& blocked_stacks ){
    assignation.clear();
    blocked_stacks.clear();

    vector<int>& s_o= layout.stacks[is_o];
    vector<int> ordered_stack = s_o;
    sort(ordered_stack.begin(),ordered_stack.end(), greater<int>());

    int ordered_items=0;
    for(;ordered_items<ordered_stack.size();ordered_items++)
        if(ordered_stack[ordered_items]!=s_o[ordered_items]) break;
    
    //reduction_items
    int items = s_o.size()-ordered_items;

    vector<int> available_slots(layout.stacks.size(),0);
    int total_slots=0;
    for(int i=0;i<layout.stacks.size();i++){
        if (i==is_o) continue;
        available_slots[i] = Layout::H-layout.stacks[i].size();
        total_slots+=available_slots[i];
    }
    int slack=total_slots-items;
    //vector<int> s=s_o;
    vector<int> s2(s_o.begin()+ordered_items,s_o.end());

    
    while( s2.size()>0 && items > layout.size()-layout.full_stacks-blocked_stacks.size()-1){
        //calculate a sequence with size in [min_items, max_items]
        int min_sz=Layout::H;
        for(int i=0;i<layout.stacks.size();i++){
            if (available_slots[i]==0) continue;
            if (min_sz > max(available_slots[i]-slack,1)) 
                    min_sz = max(available_slots[i]-slack,1);
        }

        vector<int> seq;

        bool ret = create_seq(s2, seq, min_sz);

        if(ret){
            //selection of destination stack
            int sz=seq.size();
            int s_d=0;
            double ev_s=0;
            for(int i=0;i<layout.stacks.size();i++){
                if (available_slots[i]==0) continue;
                if (sz < available_slots[i]-slack) continue;

                double ev=0;
                int szz = sz;
                //prioritize available>sz
                if (sz == available_slots[i]) ev = 1000000;
                
                if (sz > available_slots[i]) szz = available_slots[i];

                ev+= (double)ev_dest_stack(layout, i, seq[szz-1])  + (double)rand()/(double)RAND_MAX;

                if(ev > ev_s){ s_d=i; ev_s=ev; }
          
            }

            //truncate seq
            if (seq.size() > available_slots[s_d]) seq.resize(available_slots[s_d]);

            sz=seq.size();

            //assignation of destintation to containers
            for (int k=seq.size()-1; k>=0; k--){
                assignation[seq[k]]=s_d; // c -> s_d
                remove(s2.begin(),s2.end(),seq[k]);
                s2.pop_back();
                items--;
            }
            

            slack-= (available_slots[s_d]-sz);
            available_slots[s_d]=0;
            blocked_stacks.insert(s_d);
            
        }else {
            return;
        }
        

    }
    
    //exit(0);


}



pair<int, int> reduction_move(Layout& layout, int s_r){
    list< pair<int, pair < int, int> > > actions;
    pair < int, int> move = make_pair(-1,-1);
    if(s_r == -1) {
        s_r = select_dismantling_stack(layout);
        if(s_r == -1){ 
            layout.dismantled_stacks.clear();
            s_r = select_dismantling_stack(layout);
        }
        if(layout.stacks[s_r].size() > layout.size()-layout.full_stacks-1){
            smart_assignation(layout,s_r, layout.assignation, layout.blocked_stacks);
        }
        layout.dismantling_stack=s_r;
    }

    //cout << "1:" << s_r << endl;
    int c=layout.stacks[s_r].back();
    int s_d = -1;

    if (layout.assignation.find(c)!=layout.assignation.end())
        move = make_pair(s_r, layout.assignation[c]);
    else {
        int s_d=get_destination_stack(layout, s_r, layout.blocked_stacks);
        if (s_d!=-1) move = make_pair(s_r, s_d);
    }
    
    return move;
}

bool stop_reduction(Layout& layout, int s_o){
    return layout.stacks[s_o].size()==0 || layout.reachable_height(s_o)>=Layout::H-1;
}

void get_SD_actions(Layout& layout, list< pair<double, pair < int, int> > >& actions){
    int s_o=-1;

    if(layout.dismantling_stack!=-1) s_o=layout.dismantling_stack; //continue dismantling
    else{

        s_o = select_dismantling_stack(layout);
        if(s_o==-1){
            layout.dismantled_stacks.clear();
            s_o = select_dismantling_stack(layout);
        }
        
          

        if (s_o==-1 || layout.stacks[s_o].size() == 0) return; 

        if(layout.stacks[s_o].size() > layout.size()-layout.full_stacks-1)
            smart_assignation(layout,s_o, layout.assignation, layout.blocked_stacks);
        

    }

    if (layout.stacks[s_o].size()>0 && (layout.dismantling_stack==-1 || layout.reachable_height(s_o)<Layout::H-1)){
        int c=layout.stacks[s_o].back();
        //int i=layout.stacks[s_o].size()-1;
        int s_d = -1;

        if (layout.assignation.find(c)!=layout.assignation.end())
            actions.push_back(make_pair(-100,make_pair(s_o, layout.assignation[c])));
            //actions.push_back(make_pair(-100,make_pair(s_o,layout.assignation[c])));
        else 
           eval_destination_stacks(layout, s_o, actions, layout.blocked_stacks);

    } 

}


bool SD_move(Layout& layout, int s_o){


    if( layout.dismantled_stacks.find(s_o) != layout.dismantled_stacks.end() ) 
        return false; /* the stack was dismantled before */

    if (s_o==-1) {
        s_o = select_dismantling_stack(layout);
        if(s_o==-1){
            layout.dismantled_stacks.clear();
            s_o = select_dismantling_stack(layout);
        }
    }

    if (s_o==-1 || layout.stacks[s_o].size() == 0) return false;


    map< int, int > assignation;
    set<int> blocked_stacks;

    //cantidad de elementos del stack es mayor a los stack disponibles
    if(layout.stacks[s_o].size() > layout.size()-layout.full_stacks-1)
        smart_assignation(layout,s_o, assignation, blocked_stacks);
    

    while (layout.stacks[s_o].size()>0){
        int c=layout.stacks[s_o].back();
        //int i=layout.stacks[s_o].size()-1;
        int s_d = -1;

        if (assignation.find(c)!=assignation.end()) s_d = assignation[c];
        else s_d = get_destination_stack(layout, s_o, blocked_stacks);

        if(s_d==-1) return true;

        layout.move(s_o,s_d);
        if (layout.reachable_height(s_o)>=Layout::H-1) return true; //partial dismantling
    }
        
    return true; //full dismantling
}

int ev_dest_stack(const Layout& layout, int dest, int c){
    int ev;
    auto& s_d = layout.stacks[dest];
    int top_d = Layout::gvalue(s_d);
    if(layout.is_sorted(dest) && c<=top_d){//  && (s_d.size()<Layout::H-2 || layout.full_stacks < layout.size()-2) ){
        //c can be well-placed: the sorted stack minimizing top_d is preferred.
        ev = 30000 - top_d; 
    }else if (!layout.is_sorted(dest) && c>=top_d){
        //unsorted stack with c>=top_d maximizing top_d is preferred
        ev = 20000 + top_d;
    }else if(layout.is_sorted(dest)){
        ev = 10000  + s_d.size();
    }else{
        //unsorted with minimal number of unblocked containers
        //ev = - 10000 - 100*layout.n_unblocked(dest) + rand()%20;
        ev = 10000  - 200*s_d.size()  - top_d;
    }
    //cout << dest << ":" << ev << endl;
    return ev;
}

int get_destination_stack(Layout& layout, int orig, set<int> black_list){
    auto& s_o = layout.stacks[orig];
    int c = s_o.back();
    double best_ev=0; int best_stack=-1;
    for (int dest=0; dest <layout.size(); dest++){
        if(orig==dest || black_list.find(dest) != black_list.end() ) continue;
        auto& s_d = layout.stacks[dest];
        if(Layout::H == s_d.size()) continue;

        double ev = (double) ev_dest_stack(layout, dest, c) + (double)rand()/(double)RAND_MAX;
        if(best_ev<ev){
            best_ev=ev;
            best_stack = dest;
        }
    }
    return best_stack;
}

void eval_destination_stacks(Layout& layout, int orig, list< pair<double, pair < int, int> > >& actions, set<int> black_list){
    auto& s_o = layout.stacks[orig];
    int c = s_o.back();
    for (int dest=0; dest <layout.size(); dest++){
        if(orig==dest || black_list.find(dest) != black_list.end() ) continue;
        auto& s_d = layout.stacks[dest];
        if(Layout::H == s_d.size()) continue;

        double ev = (double)ev_dest_stack(layout, dest, c)  + (double)rand()/(double)RAND_MAX;;
        actions.push_back(make_pair (-ev, make_pair(orig,dest) ));
    }
}

int select_destination_stack(Layout& layout, int orig, set<int> black_list){
    list< pair<double, pair < int, int> > > actions;
    eval_destination_stacks(layout, orig, actions, black_list);

    actions.sort();
    if (actions.size()>0)
        return actions.front().second.second;
    else return -1;
}

void atomic_iter_greedy(Layout& layout, double a, double b){
    list< pair<double, pair < int, int> > > actions;
    greedy_eval(layout, actions, a, b);
    if(actions.size()==0) return;

    int ev = actions.front().first;
    pair<int,int> move= actions.front().second;

    layout.move(move.first,move.second, ev<0);
}



bool iter_greedy(Layout& layout, double a, double b){
    int& s_r=layout.dismantling_stack;
    pair<int,int> move;

    bool reduction = false;
    if(s_r !=-1 || (move = best_BG_move(layout, a, b)).first ==-1 ){
        //if(s_r==-1) cout << "reduction" << endl;
        move = reduction_move(layout, s_r);
        reduction=true;
    } 
    

    if (move.first==-1) return false;
    layout.move(move.first,move.second, reduction);
    if (reduction && stop_reduction(layout,s_r)) {
        layout.dismantled_stacks.insert(s_r);
        //cout << "end reduction" << endl;
        s_r=-1;
        
    }

    return reduction;

    
}


// retorna (eval, (so,sd))
void greedy_eval(Layout& layout, list < pair < double , pair <int, int> > >& actions, double a, double b){
    if (layout.unsorted_stacks>0){
        get_SF_actions(layout, actions, a, b);
        if (actions.size()==0 || layout.dismantling_stack!=-1 ) 
            get_SD_actions(layout, actions);
    }
    actions.sort();

    //for (auto a:actions)
      //  cout << a.first << ": (" << a.second.first << "," << a.second.second << ")" << endl;
    
}


int greedy_solve2(Layout& layout, int step_limit, double a, double b){
    bool red = false;
    while (layout.unsorted_stacks>0 && (layout.steps < step_limit || !red)){
        int steps_old=layout.steps;
        red = iter_greedy(layout, a, b);
        
        if (layout.steps==steps_old) return -1;
    }
    if(layout.steps >= step_limit ) return -1;

    return layout.steps;
}


int greedy_solve(Layout& layout, int step_limit, double a, double b){
    while (layout.unsorted_stacks>0 && layout.steps < step_limit){
        int steps_old=layout.steps;
        iter_greedy(layout, a, b);
        
        if (layout.steps==steps_old) return -1;
    }
    if(layout.steps >= step_limit ) return -1;

    return layout.steps;
}

int lazy_greedy(Layout& layout){
    while (layout.unsorted_stacks>0 && SF_move(layout));
    return layout.steps;
}

pair<int,int> atomic_move(Layout& layout, int s_o){
    int s_dd = select_destination_stack(layout, s_o);
    return make_pair(s_o,s_dd);

}



list <pair <int,int> > atomic_moves(Layout& layout, int s_o, int k){
    list< pair<double, pair < int, int> > > actions;

    eval_destination_stacks(layout, s_o, actions, set<int>());
    
    list< pair < int, int> > moves;
    actions.sort();
    moves.push_back(actions.front().second);

    actions.pop_front();
    while(actions.size()>0&&moves.size()<k){
        if (actions.front().first > 20000)
            moves.push_back(actions.front().second);
        else break;
        moves.pop_front();
    }
    return moves;
}



void reduce(Layout& layout, int s_r){
    if(layout.dismantling_stack != s_r){
        layout.dismantling_stack = -1;
        layout.assignation.clear();
        layout.blocked_stacks.clear();
    }

    do{
        pair<int,int> move = reduction_move(layout, s_r);
        if (move.first==-1) break;
        layout.move(move.first,move.second, true);
        //if(move.first==move.second) layout.print();

    }while(!stop_reduction(layout,s_r));    
    layout.dismantled_stacks.insert(s_r);
    layout.dismantling_stack = -1;
    layout.assignation.clear();
    layout.blocked_stacks.clear();
    

}

}