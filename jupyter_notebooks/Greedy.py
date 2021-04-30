import Layout 
import numpy as np

def SF_move_(layout, pos=0):
    s_o = None
    s_d = None
    min_dif = 10000
    actions = []
    
    
    for i in range(len(layout.stacks)):
        h = len(layout.stacks[i])
        if(layout.is_sorted_stack(i) and h < layout.H):
            #no se consideran stacks con H-1 contenedores
            #si solo hay 3 stacks no llenos
            #print(layout.stacks, layout.full_stacks)
            if h==layout.H-1 and layout.full_stacks >= len(layout.stacks)-3: continue
            
            top = Layout.gvalue(layout.stacks[i])
            for k in range(len(layout.stacks)):
                h = len(layout.stacks[k])
                if k!=i and (not layout.is_sorted_stack(k) or (h==layout.H and layout.full_stacks >= len(layout.stacks)-2)):
                    if layout.stacks[k][-1] <= top :
                        actions.append( (top - layout.stacks[k][-1], k, i))
    actions.sort()
    if len(actions)>pos: 
        min_df, s_o, s_d = actions[pos]
        return (s_o, s_d), len(actions)
    return False, len(actions)

def SF_move(layout, pos=0):
    s, n_act = SF_move_(layout, pos)
    if s==False: return False, n_act
    
    s_o, s_d = s
    layout.move(s_o,s_d)
    return True, n_act


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

def select_dismantling_stack(layout):
    best_ev = 0; s_o=-1
    for i in range(len(layout.stacks)):
        if layout.is_sorted_stack(i) and capacity(layout,i)<3*len(layout.stacks[i]): continue
        
        prom = sum(layout.stacks[i]) / len(layout.stacks[i]) 
        if layout.is_sorted_stack(i):
            ev = 100000 - 1000*len(layout.stacks[i]) - prom
        else:
            ev = 100000 - 1000*len(layout.stacks[i]) + prom
        

        if ev > best_ev:
            best_ev = ev
            s_o = i
    return s_o


def select_dismantling_stacks(layout):
    best_ev = 0
    actions = []
    for i in range(len(layout.stacks)):
        prom = sum(layout.stacks[i]) / len(layout.stacks[i]) 
        ev = 100000 - 100*len(layout.stacks[i]) - prom
        
        actions.append( (-ev, i))
        if ev > best_ev:
            best_ev = ev
            s_o = i
    actions.sort() 
    return actions

def SD_move(layout, pos=0,  stack=-1):
    if len(layout.stacks[stack]) == 0: return False,0

    if stack==-1:
        actions = select_dismantling_stacks(layout)
    else:
        actions = [(0,stack)]
    
    if len(actions)<=pos : return False
    
    ev, s_o = actions[pos]
    while len(layout.stacks[s_o])>0:
        ev, s_o = actions[pos]
        s_d, _ = Layout.select_destination_stack(layout,s_o)
        layout.move(s_o,s_d)
        if Layout.reachable_height(layout,s_o)==layout.H: return True, len(actions)
        
    return True, len(actions)

# #Pixies' get_ranks
# def get_ranks(stack):
#     s = sorted(stack, reverse=True)
#     temp= {}
#     r=1
#     for n in s:
#         temp[n] = r
#         r += 1
#     return temp

def get_ranks(stack):
    return sorted(stack, reverse=True)
#    r=1
#    rank = {}
#    for i in sorted(stack, reverse=True):
#        rank[i] = r
#        r += 1
#    return rank

def fill_stack(layout, s_d, n, ori, rank):
    for i in range(n):
        s_o = Layout.select_origin_stack(layout, s_d, ori, rank)
        if s_o == None: return False
        layout.move(s_o,s_d)
    return True


def capacity(layout, s_o, s_d=None):
    capacity = 0
    for i in range(len(layout.stacks)):
        if i==s_o or i==s_d: continue
        capacity += layout.H - len(layout.stacks[i])
    return capacity        

#busca contenedor con gvalue entre c y c_ub
#si se encuentra mal ubicado lo intenta colocar en s_d
def force_move2(layout, c, c_ub, s_d):
    (s, pos) = Greedy.search_highest(layout, c, c_ub, s_d)
    if layout.sorted_elements[s] > len(layout.stacks[s])+pos: return None, "no_move" #element is sorted
              
    return Greedy.force_move(layout, s, pos, s_d)
    

#intenta mover el contenedor s_o[pos] al stack s_d
def force_move(layout, s_o, pos, s_d): #pixie
    c=layout.stacks[s_o][pos]
    #if capacity(layout,s_o,s_d) < -pos-1 : return c, "failed"
    while pos<-1:
        s_tmp, _ = Layout.select_destination_stack(layout, s_o, black_list=[s_d])
        
        if s_tmp==None: 
            return c, "failed"
        
        layout.move(s_o,s_tmp)
        pos += 1
        
    
    c=layout.move(s_o,s_d)
    return c, "success" 

def search_highest(layout, c, ub, s_d):
    ret = None
    for pos in range(-1,-10,-1):
        max_c = 0
        for s in range(len(layout.stacks)):
            if s==s_d: continue
            ss = layout.stacks[s]
            if len(ss) < -pos: continue
            if ss[pos]>=c and ss[pos]<=ub and ss[pos]>max_c: 
                #if pos >= -1 or ss[pos]==c:
                if layout.sorted_elements[s] <= len(layout.stacks[s])+pos and capacity(layout,s,s_d) >= -pos-1:
                    max_c=ss[pos]
                    ret= (s,pos)
        if ret != None: return ret
    return None


def iter(layout):
    if not SF_move(layout)[0]:
        SD_move(layout)
    if layout.steps>1000: return 1000
    

def greedy_solve(layout):
    while layout.unsorted_stacks>0:
        iter(layout)
    return layout.steps

def lazy_greedy(layout):
    while layout.unsorted_stacks>0:
        success, __ = SF_move(layout)
        if not success: return
        
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