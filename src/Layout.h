
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


#ifndef _LAYOUT_
#define _LAYOUT_

using namespace std;


namespace cpmp {

class Layout {
    public:
        vector <vector <int>> stacks;

        vector<int> sorted_elements; /*for each stack*/
        int total_elements;
        int unsorted_stacks = 0;
        int unsorted_elements = 0;
        int steps = 0;
        int full_stacks = 0;
       
        list <int> bsg_moves;
        list < pair <int, int> > moves;
        
        set <int> dismantled_stacks;

        multimap <int ,int> gv2index; //for the beamsearch (type:SDPP)
        int last_sd =-1; /*last dismantled stack*/

        static bool save_moves;

        static int H;

    public:

        Layout() {}
        Layout(string filename);
        Layout(int S, int N); //random generator

        int move(int i, int j);



        bool is_sorted(int j) const{
            return (stacks[j].size() == sorted_elements[j]);
        }

        // Optimal: O(1)
        int capacity(int s_o, int s_d=-1){
            int n=1;
            int len=stacks[s_o].size();
            if(s_d!=-1) {len+=stacks[s_d].size(); n++;}

            return ( (Layout::H) * (size()-n) - (total_elements-len) );
        }

        int sum_stack(int i) const{
            int sum =0;
            for(int c : stacks[i]) sum+=c;
            return sum;
        }

        //reachable height of stack i by only performing BG moves
        int reachable_height(int i) const;

        int size() const {return stacks.size();}


        static int compute_sorted_elements(vector<int>& stack){
            if (stack.size()==0) return 0;
            int sorted_elements=1;
            while(sorted_elements<stack.size() && stack[sorted_elements] <= stack[sorted_elements-1] )
                sorted_elements++;
            

            return sorted_elements;
        }

        static int gvalue(vector<int> stack){
            if (stack.size()==0) return 100;
            else return stack.back();
        }

        void print() const{
            for(auto s : stacks){
                cout << "[" ;
                for(auto e : s) cout << e << " ";
                cout << "]\n";
            }
        }





};
}


#endif