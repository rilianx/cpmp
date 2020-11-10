from . import Layout 

def SF_move(layout, pos=0):
    s_o = None
    s_d = None
    min_dif = 10000
    actions = []
    for i in range(len(layout.stacks)):
        if(layout.is_sorted_stack(i) and len(layout.stacks[i]) < layout.H):
            top = Layout.gvalue(layout.stacks[i])
            for k in range(len(layout.stacks)):
                if k!=i and not layout.is_sorted_stack(k):
                    if layout.stacks[k][-1] <= top :
                        actions.append( (top - layout.stacks[k][-1], k, i))
    actions.sort()
    if len(actions)>pos: 
        min_df, s_o, s_d = actions[pos]
        layout.move(s_o,s_d)
        return True, len(actions)
    return False, len(actions)

def SF_move_d(layout, s_d):
    s_o = None
    min_dif = 10000
    if(layout.is_sorted_stack(s_d) and len(layout.stacks[s_d]) < layout.H):
        top = Layout.gvalue(layout.stacks[s_d])
        for k in range(len(layout.stacks)):
            if k!=s_d and not layout.is_sorted_stack(k):
                if layout.stacks[k][-1] <= top and (top - layout.stacks[k][-1]) < min_dif:
                    min_dif = top - layout.stacks[k][-1]
                    s_o = k
    if s_o != None: 
        layout.move(s_o,s_d)
        return True
    return False


def SD_move(layout, pos=0):
    best_ev = 0
    actions = []
    for i in range(len(layout.stacks)):
        prom = sum(layout.stacks[i]) / len(layout.stacks[i]) 
        ev = 10000 - 100*len(layout.stacks[i]) - prom
        
        actions.append( (-ev, i))
        if ev > best_ev:
            best_ev = ev
            s_o = i
    
    actions.sort()
    if len(actions)<=pos : return False
    while len(layout.stacks[s_o])>0:
        ev, s_o = actions[pos]
        s_d = Layout.select_destination_stack(layout,s_o)
        layout.move(s_o,s_d)
        if Layout.reachable_height(layout,s_o)==layout.H: return True, len(actions)
        
    return True, len(actions)
        
def greedy_solve(layout):
    while layout.unsorted_stacks>0:
        if not SF_move(layout)[0]:
            SD_move(layout)
        if layout.steps>1000: return 1000
    return layout.steps

def lazy_greedy(layout):
    while layout.unsorted_stacks>0:
        if not SF_move(layout): return
        
def solve_file(file,H):
    layout = read_file(file,H)
    greedy_solve(layout)
    return layout

def simulation(layout, actions, min_action=0):
    nb_actions=0
    for a in actions:
        a -= min_action
        if layout.unsorted_stacks==0: break
        
        ret, nb_actions = SF_move(layout,a)
        if ret==False and nb_actions>0: return False
        
        if not ret and nb_actions==0:
            ret, nb_actions = SD_move(layout,a)
            if ret ==False: return False
    
    greedy_solve(layout)
    return layout.steps, nb_actions