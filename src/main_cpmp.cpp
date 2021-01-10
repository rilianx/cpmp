
#include <map>
#include <vector>
#include <list>
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

int main(){


    DIR *dir;
    struct dirent *ent;

    /*** CSV instances ***/
    string folder = "../Instancias/CVS/";
    if ((dir = opendir (folder.c_str())) != NULL) {
    while ((ent = readdir (dir)) != NULL) {
        if(ent->d_name[0]=='.') continue;

        int total_steps=0, count=0;
        const clock_t begin_time = clock();
        string instance_set = ent->d_name;
        
        for(int i=1;i<=40; i++){
            string path=folder+instance_set+"/data"+instance_set+"-"+to_string(i)+".dat";
            //cout << path << endl;    
            Layout L(path);
            Layout::H = L.stacks[0].size()+2;
            //L.print();

            //int steps = greedy_solve(L, 100);
            int steps = BSG(L, 1000);
            if(steps!=-1){
                total_steps+=steps;   
                count ++;
            }
        }
        cout << instance_set << "\t" << (double)total_steps/count <<"\t" << count << "\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;
    }
    closedir (dir);

    } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
    }

    /*** BF instances ***/
    for (int i=1; i<=32; i++){
        const clock_t begin_time = clock();

        string folder = "../Instancias/BF/BF" + to_string(i) + "/";
        if( i<=8   || (i>=17 && i<=24))
            Layout::H=5;
        else Layout::H=8;


        if ((dir = opendir (folder.c_str())) != NULL) {

        /* print all the files and directories within directory */

        int total_steps=0;
        while ((ent = readdir (dir)) != NULL) {
            if(ent->d_name[0]=='c'){
                string path=folder+string(ent->d_name);
                Layout L(path);

                //int steps = greedy_solve(L);
                int steps = BSG(L, 1000);
                //cout << "steps: " << steps << endl;
                total_steps+=steps;   
            }
        }
        // do something

        cout << (double)total_steps/20 << "\t" << (float( clock () - begin_time ) /  CLOCKS_PER_SEC) << endl;
        closedir (dir);

        } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
        }
    }

    //Layout L("../Instancias/BF/BF32/cpmp_20_8_128_52_96_6.bay", 8);
    //int steps = greedy_solve(L);
    //cout << "steps:" << steps << endl;
}