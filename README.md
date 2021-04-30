# A constructive heuristic for the Container Pre-marshalling Problem (CPMP)

Implementation of a Filling-Emptying Greedy algorithm (**FEG**) for the CPMP. It is based on two procedures: (1) *filling* ordered stacks by moving directly badly placed containers into ordered stacks such that the containers become well placed and *emptying* one small stack, by moving its elements into other stacks, in order to have more available slots for well placing containers.


### Compilation

Enter to the `src` folder and compile the C++ code:

````
g++ Greedy.cpp Layout.cpp Bsg.cpp main_cpmp.cpp -o feg -std=c++11
````

### Execution

The command of exceution is:
````
.\feg H INSTANCE nb [--GreedyVariant] [--BSvariant]
````

where:
* `H` is the maximum height of stacks
* `nb` is the number of beams of the Beam Search strategy. If `nb=0`, then only the greedy algorithm is applied
* `GreedyVariant` (`FEG` or `FERG`). Correspond to the subyacent greedy algorithm variant.
* `BSvariant` (`single_moves` or `compound_moves`). Correspond to the beam search variant.


Example:
````
.\feg 12 ..\Instancias\CVS\10-6\data10-6-10.dat 0 --FERG
120 0.001
````

The output of the method (`120 0.001` in the example) corresponds to the minimum number of steps found by the strategy and the spend CPU time in seconds.

### Solving random instances

The algorithm also allows one to generate and solve random instances. The command is:
````
.\feg H --random S N nb [--GreedyVariant] [--BSvariant]
````
where `S` is the number of stacks and `N`, the number of containers.

Example:
````
.\feg 5 --random 3 6 1000 --FEG --compund_moves

Generating a random instance: S=3, N=6, H=5
[59 ]
[35 ]
[70 31 49 27 ]
2       0
State-Action 2
[59 ]
[35 27 ]
[70 31 49 ]
2,0
State-Action 1
[59 ]
[35 ]
[70 31 49 27 ]
2,1
````
