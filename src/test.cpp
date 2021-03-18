
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



class Node
{
    Layout * actual;
    list<Layout *>  hijos;
    int level;

    public: 

        void generarHijos(actual)
        {
            //Debo ver la cantidad de hijos (cantidad de stacks que se pueden llenar -1)
            int stacks = actual->stacks.size();

            //Por cada stack
            for (int i=0;i<stacks;i++)
            {
               
                //Se mueve al resto
                for (j=0;j<stacks;j++)
                {
                    //Que no sea el mismo stack
                    if (i != j)
                    {
                        
                        
                        //Si j no esta lleno (agregar)
                        //DeepCopy del anterior
                        Layout * nuevo = new Layout(*actual);
                        nuevo->move(i,j);
                        hijos.insert(*nuevo);
                    }
                }
            }

        }
        
}
class Tree
{
    Node * base;
    int limit;
    list<Layout *>  layouts;
}

int main(int argc, char * argv[])
{

    //se define el layout
    Layout::H = std::atoi ("12");
    string path("..\\Instancias\\CVS\10-6\\data10-6-10.dat");
    int beams = std::atoi ("0");
    Layout L(path), best_lay(path);
    const clock_t begin_time = clock();
    int steps;

    

    
}
