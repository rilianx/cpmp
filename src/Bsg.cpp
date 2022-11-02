
#include "Bsg.h"

#include <set>
#include <utility>

using namespace std;

using namespace cpmp;

namespace cpmp {
    void generate_candidates(const Layout& lay, list<Layout>& C, int type, int k){
        
        if(type==ATOMIC || type==MIXED){
            
            for(int s_o=0; s_o < lay.size(); s_o++){
                if (lay.stacks[s_o].size() == 0) continue;
                if (lay.is_sorted(s_o) && lay.reachable_height(s_o)>=Layout::H-1) continue;

                
                list< pair<int, int> > dests;

                for(int s_d=0; s_d < lay.size(); s_d++){
                    if (lay.stacks[s_d].size() == Layout::H) continue;
                    if (s_o==s_d) continue;

                    int ev = ev_dest_stack(lay, s_d, lay.stacks[s_o].back());

                    dests.push_back(make_pair (-ev, s_d ));
                }

                dests.sort();

                bool sorted_o=lay.is_sorted(s_o);
                for(int i=0; i<k && dests.size()>0; i++){
                    int s_d = dests.front().second; dests.pop_front();

                    
                    Layout clay = lay;
                    clay.move(s_o,s_d);
                    
                    /*if(sorted_o || !clay.is_sorted(s_d)){ // not BG_move
                        if(!stop_reduction(clay,s_d)){
                            clay.dismantling_stack=i;
                            //clay.dismantled_stacks.insert(i);
                        }
                    }*/


                    C.push_back(clay);
                }
            }

        }

        if(type==COMPOUND || type==MIXED){
            for(int s_o=0; s_o < lay.size(); s_o++){
                if (lay.stacks[s_o].size() == 0) continue;
                if (lay.is_sorted(s_o) && lay.reachable_height(s_o)>=Layout::H-1) continue;
                
                
                Layout clay = lay;
                reduce(clay,s_o);

                if(clay.steps > lay.steps)
                    C.push_back(clay);
            }

            Layout clay = lay;
            lazy_greedy(clay);
           
            if(clay.steps > lay.steps)
                C.push_back(clay);
                        
        }

        Layout clay = lay;
        iter_greedy(clay);

        if(clay.steps > lay.steps)
            C.push_back(clay);

        
    }

    int BSG(Layout& layout, int w, int type, Layout& best_lay, int k){
        int min_steps=200;
        if (layout.unsorted_elements==0) return 0;

        list<Layout> S;
        S.push_back(layout);
        while(S.size() > 0){
            list<Layout> C;
           
            for(Layout& lay:S) 
                generate_candidates(lay, C, type, k);
            

            multimap< double, Layout* > N; //new level of the beam search tree
            for(Layout& clay : C){
                if(clay.steps >= min_steps) continue;
                int lb = clay.steps + clay.unsorted_elements; /* lower bound for the state*/
                Layout gclay = clay;

                int steps = greedy_solve(gclay, min_steps+10);
                

                
                if(steps != -1 && steps < min_steps){
                    min_steps = steps;
                    best_lay = gclay;
                }

                double eval = steps;
                if(type==COMPOUND) eval -= 0.01*clay.steps; //shorter is better
                else eval += 0.01*clay.steps; 
                if(steps !=-1 && lb < steps) N.insert(make_pair(eval, &clay));
            }

            S.clear(); int i=0;
            for(pair<int, Layout*> lay_p : N ){
                if(i==w) break;
                S.push_back(Layout(*lay_p.second));
                i++;
            }
            
        }
        if (min_steps==200) return -1;
        return min_steps;  
    }

    
}