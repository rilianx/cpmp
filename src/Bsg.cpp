
#include "Bsg.h"

#include <set>
#include <utility>

using namespace std;

using namespace cpmp;

namespace cpmp {
    void generate_candidates(const Layout& lay, list<Layout>& C, int type){
        
        if(type==ATOMIC){
            
            for(int s_o=0; s_o < lay.size(); s_o++){
                if (lay.stacks[s_o].size() == 0) continue;
                if (lay.is_sorted(s_o) && lay.reachable_height(s_o)>=Layout::H-1) continue;
                
                Layout clay = lay;
                bool success = atomic_move(clay,s_o);
                if (success)
                    C.push_back(clay);
            }

            Layout clay = lay;
            iter_greedy(clay);

            if(clay.steps > lay.steps)
                C.push_back(clay);
        }

        else if(type==COMPOUND){
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
            
            
            clay = lay;
            iter_greedy(clay);
            
            if(clay.steps > lay.steps)
                C.push_back(clay);
            

        }

        else if(type==MIXED){
            for(int s_o=0; s_o < lay.size(); s_o++){
                if (lay.stacks[s_o].size() == 0) continue;
                if (lay.is_sorted(s_o) && lay.reachable_height(s_o)>=Layout::H-1) continue;
                
                Layout clay = lay;
                bool success = atomic_move(clay,s_o);
                if (success)
                    C.push_back(clay);
                
                clay = lay;
                reduce(clay,s_o);

                if(clay.steps > lay.steps)
                    C.push_back(clay);

            }
            Layout clay = lay;
            
          
            lazy_greedy(clay);
           
            
            if(clay.steps > lay.steps)
                C.push_back(clay);
            
            
            clay = lay;
            iter_greedy(clay);
            
            if(clay.steps > lay.steps)
                C.push_back(clay);
        }

        
    }

    int BSG(Layout& layout, int w, int type, Layout& best_lay){
        int min_steps=200;
        if (layout.unsorted_elements==0) return 0;

        list<Layout> S;
        S.push_back(layout);
        while(S.size() > 0){
            list<Layout> C;
           
            for(Layout& lay:S) 
                generate_candidates(lay, C, type);
            

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