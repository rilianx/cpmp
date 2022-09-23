
#include "Greedy.h"

#include <set>
#include <stack>
#include <utility>

using namespace std;

using namespace cpmp;

namespace cpmp {


bool SF_move(Layout& layout, double a, double b){
    pair<int,int> move = _SF_move(layout, a, b);
    if (move.first==-1) return false;
    layout.move(move.first,move.second);
    return true;
}

void get_SF_actions(Layout& layout, list <pair <int, pair <int,int> > >& actions, double a, double b ){

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
    list <pair <int, pair <int,int> > > actions; /* (eval, (s_o,s_d))*/
    get_SF_actions(layout, actions, a, b);

    //cout << actions.size() << endl;
    actions.sort();

    if (actions.size()>0)
        return actions.front().second;
    else   
        return make_pair(-1,-1);
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
            ev = 100000 - 1000*layout.stacks[i].size() - 100 - prom ;
        else
            ev = 100000 - 1000*layout.stacks[i].size() + 100 + prom ;
    
        if (ev > best_ev){
            best_ev = ev;
            s_o = i;
        }
    }

    return s_o;
}

bool recursive_create_seq(const vector<int>& s_o, stack<int>& seq, 
                set<int> feasible_seq, set<int>& bl){

    //filtered sequence s
    vector<int> ord_s;
    for (auto it:s_o) if (seq.empty() || it <=seq.top()) ord_s.push_back(it);

    if(ord_s.size()==0){
        for(int k=seq.size(); k>0; k--){
            if(feasible_seq.find(k)!=feasible_seq.end()){
                while(seq.size()!=k) seq.pop();
                return true;
            }
        }
        return false;
    }
       

    sort(ord_s.begin(),ord_s.end(), greater<int>());

    for(auto gv:ord_s){
        if(bl.find(gv)!=bl.end()) continue;
        bl.insert(gv);

        seq.push(gv);

        if (gv==s_o.back()) {
            for(int k=seq.size(); k>0; k--){
                if(feasible_seq.find(k)!=feasible_seq.end()){
                    while(seq.size()!=k) seq.pop();
                    return true;
                }
            }
        }else{
            int k=0;
            for(;s_o[k]!=gv;k++); k++;         

            if(recursive_create_seq(vector<int>(s_o.begin()+k,s_o.end()), seq, 
                feasible_seq, bl))
                return true;
        }
        seq.pop();
    }
    return false;

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
        //cout << "available_slots[" << i << "]:" << available_slots[i] <<endl;
    }
    int slack=total_slots-items;
    vector<int> s=s_o;

    while(s.size()-ordered_items > layout.size()-layout.full_stacks-blocked_stacks.size()-1){
        //calculate a sequence with size in [min_items, max_items]
        set<int> feasible_seq;
        for(int i=0;i<layout.stacks.size();i++){
            if (available_slots[i]==0) continue;
            for(int k=std::max(available_slots[i]-slack,1); k<=available_slots[i];k++)
                feasible_seq.insert(k);
        }

        stack<int> seq;
        set<int> bl;

        bool ret=recursive_create_seq(vector<int>(s.begin()+ordered_items,s.end()), seq, feasible_seq, bl);

        if(ret){
            //selection of destination stack
            int sz=seq.size();
            int s_d=0;
            int min_av_sl=100;
            for(int i=0;i<layout.stacks.size();i++){
                if (available_slots[i]==0) continue;
                if (available_slots[i] < min_av_sl 
                        && sz>=available_slots[i]-slack && sz<=available_slots[i]){
                    s_d=i; min_av_sl=available_slots[i];
                }
            }

            //assignation of destintation to containers
            while(!seq.empty()){
                s.erase(find(s.begin(),s.end(),seq.top()));
                assignation[seq.top()]=s_d;
                seq.pop();
            }

            slack-= (available_slots[s_d]-sz);
            available_slots[s_d]=0;
            blocked_stacks.insert(s_d);
            
        }else return;

    }
    
    //exit(0);


}



void get_SD_actions(Layout& layout, list< pair<int, pair < int, int> > >& actions){
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
        int s_d = -1;

        if (layout.assignation.find(c)!=layout.assignation.end()) 
            actions.push_back(make_pair(-100,make_pair(s_o,layout.assignation[c])));
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
        int s_d = -1;

        if (assignation.find(c)!=assignation.end()) s_d = assignation[c];
        else s_d = select_destination_stack(layout, s_o, blocked_stacks);

        if(s_d==-1) return true;

        layout.move(s_o,s_d);
        if (layout.reachable_height(s_o)>=Layout::H-1) return true; //partial dismantling
    }
        
    return true; //full dismantling
}


void eval_destination_stacks(Layout& layout, int orig, list< pair<int, pair < int, int> > >& actions, set<int> black_list){
    auto& s_o = layout.stacks[orig];
    int c = s_o.back();
    for (int dest=0; dest <layout.size(); dest++){
        if(orig==dest || black_list.find(dest) != black_list.end() ) continue;
        auto& s_d = layout.stacks[dest];
        if(Layout::H == s_d.size()) continue;

        int top_d = Layout::gvalue(s_d);
        int ev;

        if(layout.is_sorted(dest) && c<=top_d){//  && (s_d.size()<Layout::H-2 || layout.full_stacks < layout.size()-2) ){
            //c can be well-placed: the sorted stack minimizing top_d is preferred.
            ev = 1000000 - top_d - 100*(s_d.size()>Layout::H-2); 
        }else if (!layout.is_sorted(dest) && c>=top_d){
            //unsorted stack with c>=top_d maximizing top_d is preferred
            ev = 100000 + top_d;
        }else if(layout.is_sorted(dest)){
            ev = 10000  + s_d.size();
        }else{
            //unsorted with minimal number of unblocked containers
            //ev = - 10000 - 100*layout.n_unblocked(dest) + rand()%20;
            ev = 10000  - 200*s_d.size()  - top_d;
        }

        actions.push_back(make_pair (-ev, make_pair(orig,dest) ));
        
    }


}

int select_destination_stack(Layout& layout, int orig, set<int> black_list){
    list< pair<int, pair < int, int> > > actions;
    eval_destination_stacks(layout, orig, actions, black_list);

    actions.sort();
    if (actions.size()>0)
        return actions.front().second.second;
    else return -1;
}

void atomic_iter_greedy(Layout& layout, double a, double b){
    list< pair<int, pair < int, int> > > actions;
    greedy_eval(layout, actions, a, b);
    if(actions.size()==0) return;

    int ev = actions.front().first;
    pair<int,int> move= actions.front().second;

    if(ev<0){ // Reducing stack
        layout.dismantling_stack=move.first;
        layout.dismantled_stacks.insert(move.first);
    }else{ // BG move
        layout.dismantling_stack=-1;
        layout.assignation.clear();
        layout.blocked_stacks.clear();
    }

    layout.move(move.first,move.second);
    //layout.print();
}

void iter_greedy(Layout& layout, double a, double b){
    if (!SF_move(layout, a, b)){
        while(true){
            list< pair<int, pair < int, int> > > actions;
            get_SD_actions(layout, actions);
            actions.sort();
            if(actions.size()>0){
                pair<int,int> move= actions.front().second;
                layout.move(move.first,move.second);

                layout.dismantling_stack=move.first;
                layout.dismantled_stacks.insert(move.first);
            } else {
                layout.dismantling_stack=-1;
                layout.assignation.clear();
                layout.blocked_stacks.clear();
                break;
            }
        }
    }
}

// retorna (eval, (so,sd))
void greedy_eval(Layout& layout, list < pair < int , pair <int, int> > >& actions, double a, double b){
    if (layout.unsorted_stacks>0){
        get_SF_actions(layout, actions, a, b);
        if (actions.size()==0 || layout.dismantling_stack!=-1 ) 
            get_SD_actions(layout, actions);
    }
    actions.sort();

    //for (auto a:actions)
      //  cout << a.first << ": (" << a.second.first << "," << a.second.second << ")" << endl;
    
}

int greedy_solve(Layout& layout, int step_limit, double a, double b){
    while (layout.unsorted_stacks>0 && layout.steps < step_limit){
        //cout << layout.unsorted_stacks << endl;
        //layout.print(); cout << endl;
        int steps_old=layout.steps;

        atomic_iter_greedy(layout, a, b);

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

    int s_dd = select_destination_stack(layout, s_o);
    layout.move(s_o, s_dd);
    return true;
}

}