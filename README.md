# A constructive heuristic for the Container Pre-marshalling Problem (CPMP)

Implementation of a Filling-Emptying Greedy algorithm (**FEG**) for the CPMP. It is based on two procedures: (1) *filling* ordered stacks by moving directly badly placed containers into ordered stacks such that the containers become well placed and *emptying* one small stack, by moving its elements into other stacks, in order to have more available slots for well placing containers.

The main differences between **FEG** and other heuristic algorithms are:

### Compilation

Enter to the `src` folder and compile the C++ code:

````
g++ Greedy.cpp Layout.cpp Bsg.cpp main_cpmp.cpp -o feg
````

### Execution
The command of exceution is:
````
.\fog.exe H INSTANCE nb [--GreedyVariant] [--BSvariant]
````

where:
* `H` is the maximum height of stacks
* `nb` is the number of beams of the Beam Search strategy. If `nb=0`, then only the greedy algorithm is applied
* `GreedyVariant` (`FEG` or `FERG`). Correspond to the subyacent greedy algorithm variant.
* `BSvariant` (`single_moves` or `compound_moves`). Correspond to the beam search variant.

Example:
````
.\ferg 12 ..\Instancias\CVS\10-6\data10-6-10.dat 0 --FERG
120 0.001
````

The output of the method (`120 0.001` in the example) corresponds to the minimum number of steps found by the strategy and the spend CPU time in seconds.





