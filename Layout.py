def compute_sorted_elements(stack):
    if len(stack)==0: return 0
    sorted_elements=1
    while(sorted_elements<len(stack) and stack[sorted_elements] <= stack[sorted_elements-1]):
        sorted_elements +=1
    
    return sorted_elements

def gvalue(stack):
    if len(stack)==0: return 100
    else: return stack[-1]
    
    
class Layout:
    def __init__(self, stacks, H):
        self.stacks = stacks
        self.sorted_elements = []
        self.sorted_stack = []
        self.unsorted_stacks = 0
        self.steps = 0
        self.current_step = 0
        self.moves = []
        self.H = H
        j=0
        
        for stack in stacks:
            self.sorted_elements.append(compute_sorted_elements(stack))
            if not self.is_sorted_stack(j):
                self.unsorted_stacks += 1
                self.sorted_stack.append(False)
            else: self.sorted_stack.append(True)
            j += 1
    
    def move(self,i,j, index=-1):
        c = self.stacks[i][index]
        if self.is_sorted_stack(i):
            self.sorted_elements[i] -= 1
            
        if self.is_sorted_stack(j) and gvalue(self.stacks[j]) >= c:
            self.sorted_elements[j] += 1
            
        self.stacks[i].pop(index)
        self.stacks[j].append(c)
        
        if index!=-1:  self.sorted_elements[i] = compute_sorted_elements(self.stacks[i])
        self.is_sorted_stack(i)
        self.is_sorted_stack(j)
        self.steps += 1
        self.current_step += 1
        self.moves.append((i,j,index))
        
        return c
        
    
        
    def highlighted_print(self, stack_set):
        for stack in self.stacks:
            if stack in stack_set:
                print('\033[94m'+str(stack)+'\033[0m')
            else:
                print(stack)
    
    def next(self, printed=False):
        if self.current_step == self.steps: 
            if printed: self.highlighted_print([])
            return
        i,j = self.moves[self.current_step]
        self.current_step += 1
        c=self.stacks[i].pop()
        self.stacks[j].append(c)
        
        if printed:
            s=[]
            if(self.current_step < self.steps):
                i,j = self.moves[self.current_step]
                s.append(self.stacks[i]), s.append(self.stacks[j])
            self.highlighted_print(s)
                

    def prev(self, printed=False):
        if self.current_step == 0:
            if printed: self.highlighted_print([])
            return
        
        self.current_step -= 1
        j,i = self.moves[self.current_step]
        c=self.stacks[i].pop()
        self.stacks[j].append(c)

        if printed:   
            s=[]
            if(self.current_step-1 > 0):
                i,j = self.moves[self.current_step-1]
                s.append(self.stacks[i]), s.append(self.stacks[j])
            self.highlighted_print(s)
        
    def is_sorted_stack(self, j):
        sorted = len(self.stacks[j]) == self.sorted_elements[j]

        if j<len(self.sorted_stack) and self.sorted_stack[j] != sorted: 
            self.sorted_stack[j] = sorted
            if sorted == True: self.unsorted_stacks -= 1
            else: self.unsorted_stacks += 1
        return sorted

    
def read_file(file, H):
    with open(file) as f:
        S, C = [int(x) for x in next(f).split()] # read first line
        stacks = []
        for line in f: # read rest of lines
            stack = [int(x) for x in line.split()[1::]]
            #if stack[0] == 0: stack.pop()
            stacks.append(stack)
            
        layout = Layout(stacks,H)
    return layout

def select_destination_stack(layout, orig, black_list=[], max_pos=100):
    s_o = layout.stacks[orig]
    c = s_o[-1]
    best_eval=-1000000;
    best_dest=None
    dest=-1;

    for dest in range(len(layout.stacks)):
            if orig==dest or dest in black_list: continue
            s_d = layout.stacks[dest]

            if(layout.H == len(s_d)): continue
            top_d=gvalue(s_d)

            ev=0

            if layout.is_sorted_stack(dest) and c<=top_d:
              #c can be well-placed: the sorted stack minimizing top_d is preferred.
              ev = 10000 - 100*top_d
            elif not layout.is_sorted_stack(dest) and c>=top_d:
              #unsorted stack with c>=top_d maximizing top_d is preferred
              ev = top_d
            elif layout.is_sorted_stack(dest):
              #sorted with minimal top_d
              ev = -100 - top_d
            else:
              #unsorted with minimal numer of auxiliary stacks
              ev = -10000 #+ required_stacks(dest)
            
            if layout.H - len(s_d) > max_pos:
              ev -= 10000

            if ev > best_eval:
                best_eval=ev
                best_dest=dest

    return best_dest

def select_origin_stack(layout, dest, ori, rank):
    s_d = layout.stacks[dest]
    top_d = s_d[-1]
    best_eval=-1000000;
    best_orig=None
    orig=-1;

    for orig in range(len(layout.stacks)):
            if orig==dest or orig ==ori: continue
            s_o = layout.stacks[orig]

            if len(s_o)==0: continue           
            c=gvalue(s_o)
            if c in rank and rank[c] < layout.H-len(s_d): continue

            ev=0

            if layout.is_sorted_stack(dest) and c<=top_d:
                #c can be well-placed: the sorted stack maximizing c is preferred.
                ev = 10000 + 100*c
            elif not layout.is_sorted_stack(dest) and c>=top_d:
                #unsorted stack with c>=top_d minimizing c is preferred
                ev = -c
            else:
                ev = -100 - c 

            if ev > best_eval:
                best_eval=ev
                best_orig=orig

    return best_orig

def reachable_height(layout, i):
    if not layout.is_sorted_stack(i): return -1;
    
    top = gvalue(layout.stacks[i])
    h = len(layout.stacks[i])
    if h==layout.H: return size;
    
    stack=layout.stacks[i]
    all_stacks = True #True: all the bad located tops can be placed in stack
    
    for k in range(len(layout.stacks)):
        if k==i: continue
        if layout.is_sorted_stack(k): continue
            
        stack_k=layout.stacks[k]
        unsorted = len(stack_k)-layout.sorted_elements[k]
        prev = 1000;
        for j in range (1,unsorted+1):
            if stack_k[-j] <= prev and stack_k[-j] <=top:
                h += 1
                if h==layout.H: return h
                prev = stack_k[-j]
            else: 
                if j==1: all_stacks=False
                break
                
    if all_stacks: return layout.H
    else: return h
    
def read_file(file, H):
    with open(file) as f:
        S, C = [int(x) for x in next(f).split()] # read first line
        stacks = []
        for line in f: # read rest of lines
            stack = [int(x) for x in line.split()[1::]]
            #if stack[0] == 0: stack.pop()
            stacks.append(stack)
            
        layout = Layout(stacks,H)
    return layout