#include "Layout.h"

namespace cpmp {

int Layout::H=0;
bool Layout::save_moves=false;

Layout::Layout(int S, int N){

    unsorted_elements = total_elements = N;
    stacks.resize(S);
    sorted_elements.resize(S);
    for(int j=0;j<N;j++) {
        int s=rand()%S;
        while(stacks[s].size()==H) s=rand()%S;
        stacks[s].push_back(rand()%99+1);
    }

    for(int i=0;i<S;i++){
        if (stacks[i].size() == H) full_stacks++;
        sorted_elements[i] = compute_sorted_elements(stacks[i]);
        unsorted_elements -= sorted_elements[i];
        if (!is_sorted(i))
            unsorted_stacks ++;
    }

}

Layout::Layout(string filename)  {
    //stacks are read from file

    std::fstream myfile(filename, std::ios_base::in);
    int n_stacks;
    myfile >> n_stacks;
    myfile >> total_elements;
    unsorted_elements = total_elements;
    stacks.resize(n_stacks);
    sorted_elements.resize(n_stacks);

    for(int i=0; i<n_stacks; i++){
        int n, el;
        myfile >> n;
        for(int j=0;j<n;j++) {
            myfile >> el;
            stacks[i].push_back(el);
        }
        if (n == H) full_stacks++;
        sorted_elements[i] = compute_sorted_elements(stacks[i]);
        unsorted_elements -= sorted_elements[i];
        if (!is_sorted(i))
            unsorted_stacks ++;
        
    }
}

int Layout::move(int i, int j){
    if(stacks[i].size() == 0 || stacks[j].size()==H){
        print();
        cout << endl << "error: move (" << i << "," << j << ") is not feasible" << endl;
        exit(0);
    }

    if(stacks[i].size() == H) full_stacks--;
    if(stacks[j].size() == H-1) full_stacks++;
    
    int c = stacks[i].back();

    if (is_sorted(i)) {
        sorted_elements[i] --;
        unsorted_elements ++;
        stacks[i].pop_back();
    }else{
        stacks[i].pop_back();
         if (is_sorted(i)) unsorted_stacks --;
    }
    
    if (is_sorted(j)){
        if(gvalue(stacks[j]) >= c) {
            sorted_elements[j] ++;
            unsorted_elements --;
        }
        else unsorted_stacks ++;
    }

    stacks[j].push_back(c);
    steps ++;
    if(save_moves) moves.push_front({i,j});

    return c;
}

//O(H*R)
int Layout::reachable_height(int i) const{
    if (!is_sorted(i)) return 0;
    int top = gvalue(stacks[i]);
    int h = stacks[i].size();
    if (h==H) return h;

    //if all_stacks is true: all the bad located tops can be placed in stack: return H (cannot do better)
    bool all_stacks = true;

    for(int k=0; k<stacks.size(); k++){
        if(k==i || is_sorted(k)) continue;
        auto& stack_k=stacks[k]; int sk=stack_k.size();
        int unsorted = sk-sorted_elements[k];
        int prev=1000;
        for(int j=1;j<unsorted+1;j++){
            if (stack_k[sk-j] <= prev && stack_k[sk-j] <=top){
                h += 1;
                if (h==H) return h;
                prev = stack_k[sk-j];
            }else {
                if (j==1) all_stacks=false;
                break;
            }
        }
    }
    if (all_stacks) return H;
    else return h;
}

}