
#include <map>
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
#include<ctime>
#include "dirent.h"

#include "Layout.h"
#include "Greedy.h"
#include "Bsg.h"

using namespace std;
using namespace cpmp;



int main(int argc, char * argv[]){
    
    /*vector<int> v = {48,47 ,50, 16, 6, 5, 1, 13, 30, 14};
    vector<int> seq;
    for(int k:v) cout << k << " ";
    cout << endl;
    create_seq(v, seq, 1);

    for(int k:v) cout << k << " ";
    cout << endl;
    for(int k:seq) cout << k << "--";
    cout << endl;
    
    exit(0);*/

    Layout::H = atoi (argv[1]);
    Layout L;

    int carg=2;
    if(string(argv[carg++])== "--random") {
        srand(time(NULL));
        cout << "Generating a random instance: S="<<argv[carg] << ", N="<<argv[carg+1] << ", H="<<Layout::H << endl;
        //Layout::save_moves = true;
        L=Layout(atoi(argv[carg]), atoi(argv[carg+1]));
        L.print();
        carg+=2;
    }else{
        //Layout::save_moves = true;
        L=Layout(argv[2]);
    }

    double a = atof (argv[carg++]);
    double b = a/2;
    int k = 0; //atoi (argv[carg++]);
    int  max_steps = atoi(argv[carg++]); 
    
    bool assign=true;

    int beams = atoi (argv[carg++]);
    Layout best_lay = L;
    const clock_t begin_time = clock();
    int steps;
    //if (beams==0) steps = greedy_solve(L,1000);
    int type=ATOMIC;
    if(argc>=carg+1 && string(argv[carg++])== "--compound") type=COMPOUND;
    else if(argc>=carg && string(argv[carg-1])== "--mixed") type=MIXED;
    else if(argc>=carg && string(argv[carg-1])== "--no-assignment") assign=false;
    

    int seed = atoi (argv[carg++]);
    srand(seed);
    
    
    if (beams==0){
        steps = greedy_solve(L,max_steps,a,b,assign);
        
    }else 
        steps = BSG(L, beams, type, best_lay, k);
    /*if (steps==-1) {
        steps=1000;
        cout << argv[2] << endl;
    }*/
    cout << steps <<"\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;

    //L.print();

    int s=L.moves.size();
    /*for(auto m : L.moves){
        L.move(m.second,m.first);
        cout << "State-Action " << s-- << endl;
        L.print();
        cout << m.first <<"," << m.second << endl;
    }*/
    

    //for(int m: best_lay.bsg_moves) cout << m  << " ";
    //cout << endl;
    //recreate(L,best_lay.bsg_moves);

}


// int main(){
//     DIR *dir;
//     struct dirent *ent;

//     /*** CSV instances ***/
//     string folder = "../Instancias/CVS/";
//     if ((dir = opendir (folder.c_str())) != NULL) {
//     double prom=0.0;
//     while ((ent = readdir (dir)) != NULL) {
//         if(ent->d_name[0]=='.') continue;

//         int total_steps=0, count=0;
//         const clock_t begin_time = clock();
//         string instance_set = ent->d_name;
        
//         for(int i=1;i<=40; i++){
//             string path=folder+instance_set+"/data"+instance_set+"-"+to_string(i)+".dat";
//             cout << path << endl;    
//             Layout L(path);
//             Layout::H = L.stacks[0].size()+2;
//             //L.print();

//             //int steps = pixie_solve(L, 1000);
//             //int steps = BSG(L, 1000, SD_MOVE);
//             int steps = BSG(L, 10);
//             if(steps!=-1){
                
//                 total_steps+=steps;   
//                 count ++;
//             }else cout << path << endl;
//         }
//         cout << instance_set << "\t" << (double)total_steps/count <<"\t" << count << "\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;
//         prom +=  (double)total_steps/count;
//     }
//     cout << prom/21 << endl;
//     closedir (dir);

//     } else {
//     /* could not open directory */
//     perror ("");
//     return EXIT_FAILURE;
//     }

//     /*** BF instances ***/
//     for (int i=26; i<=26; i++){
//         const clock_t begin_time = clock();

//         string folder = "../Instancias/BF/BF" + to_string(i) + "/";
//         if( i<=8   || (i>=17 && i<=24))
//             Layout::H=5;
//         else Layout::H=8;


//         if ((dir = opendir (folder.c_str())) != NULL) {

//         /* print all the files and directories within directory */

//         int total_steps=0;
//         while ((ent = readdir (dir)) != NULL) {
//             if(ent->d_name[0]=='c'){
//                 string path=folder+string(ent->d_name);
//                 Layout L(path);

//                 //int steps = greedy_solve(L);
//                 int steps = BSG(L, 1000, SD_MOVE);
//                 //cout << "steps: " << steps << endl;
//                 total_steps+=steps;   
//             }
//         }
//         // do something
        
//         cout << (double)total_steps/20 << "\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;
//         closedir (dir);

//         } else {
//         /* could not open directory */
//         perror ("");
//         return EXIT_FAILURE;
//         }
        
//     }

//     //Layout L("../Instancias/BF/BF32/cpmp_20_8_128_52_96_6.bay", 8);
//     //int steps = greedy_solve(L);
//     //cout << "steps:" << steps << endl;
// }

/*
10-10   148.325 40      39.849
10-6    108     2       0.026
3-3     10.175  40      0.012
3-4     10.175  40      0.031
3-5     10.975  40      0.081
3-6     11.875  40      0.39
3-7     13.2    40      1.683
3-8     13.925  40      5.525
4-4     19.075  40      0.336
4-5     20.325  40      0.594
4-6     21.4    40      3.454
4-7     23.725  40      10.67
5-10    44.825  40      53.171
5-4     30.0556 36      0.42
5-5     29.475  40      1.165
5-6     33.7    40      3.268
5-7     35.475  40      8.588
5-8     39.3    40      16.625
5-9     42.5    40      36.24
6-10    60.65   40      52.933
6-6     46      40      2.781
*/