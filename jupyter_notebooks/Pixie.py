import Layout
import Greedy

def read(path):
    flag = 0
    #Columnas
    columns = 0
    #Total contenedores
    total = 0
    maxSize = 0
    stacks = []
    with open(path) as file:
        for line in file:
            s = line.split()
            #Si es la primera linea entonces se guarda las columnas y el total
            if flag == 0:
                #Actualiza columns
                columns = s[0]
                #Actualiza total
                total = s[1]
                #Actualiza flag para que no pase denuevo
                flag=1
            #En caso contrario sen otros datos de interes
            else:
                #Se ve el size mayor de la columna para definirlo automatico
                aux=int(s[0])
                if maxSize <= aux:
                    maxSize = aux;

                #Se crean los stacks
                temporalStack = []
                for i in range(1,len(s)):
                    temporalStack.append(int(s[i]))
                stacks.append(temporalStack)
    #Se crea el layout
    maxSize += 2
    layout = Layout.Layout(stacks, maxSize)
    
    return layout

#Test
#path="C:\\Users\\Leviathan\\Desktop\\trabajo\\Referencias\\BF\\Instancias\\10-6\\data10-6-1.dat"
#layout=read(path)
#print (layout.stacks)
#print (layout.H)

import copy 

#Se revisa una columna posible que nos generara la menor cantidad de problemas para ordenar
def select_col(layout):

    field=layout.stacks  
    best=-500
    col=-1
    
    for i in field:
        
        sort=copy.deepcopy(i)
        sort.sort(reverse=True)
        copia=copy.deepcopy(i)
        #print("#SEPARADOR------------------")
        
        #print(copia)
        #print(sort)
        
        #print("#SEPARADOR------------------")
        #Esto es para que no tome las columnas ordenadas
        if sort != copia:
            #Mientras no sean iguales
            while sort != copia:
                #Quitamos el superior
                sort.pop(len(sort)-1)
                copia.pop(len(copia)-1)

                if sort == copia:
                    #Se revisa que sea mejor que el anterior
                    if best < len(copia):
                        best=len(copia)
                        col=i
                    break
    #print(col)
    for i in range(0,len(field)-1):
        if layout.stacks[i] == col:
            return i
    #En caso que no queden columnas
    return -1

def ranks(stack):
    stack.sort()
    stack.reverse()
    temp= {}
    r=1
    for n in stack:
        temp[n] = r
        r += 1
    return temp

def get_ranks(stack):
    r=1
    rank = {}
    for i in sorted(stack, reverse=True):
        rank[i] = r
        r += 1
    return rank

def fill_stack(layout, s_d, n, ori, rank):
    for i in range(n):
        s_o = Layout.select_origin_stack(layout, s_d, ori, rank)
        layout.move(s_o,s_d)

import numpy as np
def capacity(layout, s_o):
    capacity = 0
    for i in range(len(layout.stacks)):
        if i==s_o: continue
        capacity += layout.H - len(layout.stacks[i])
    return capacity            

def force_move(layout, s_o, pos, s_d): #pixie
    while pos<-1:
        s_tmp = Layout.select_destination_stack(layout, s_o, black_list=[s_d])
        layout.move(s_o,s_tmp)
        pos += 1
    return layout.move(s_o,s_d)

def search_highest(layout, c, ub, s_d):
    ret = None
    for pos in range(-1,-10,-1):
        max_c = 0
        for s in range(len(layout.stacks)):
            if s==s_d: continue
            ss = layout.stacks[s]
            if len(ss) < -pos: continue
            if ss[pos]>=c and ss[pos]<=ub and ss[pos]>max_c: 
                max_c=ss[pos]
                ret= (s,pos)
        if ret != None: return ret
    return None

def remove_until(column):
    sort=copy.deepcopy(column)
    sort.sort(reverse=True)
    copia=copy.deepcopy(column)
        
    contador=1
    while sort != copia:
        sort.pop(len(sort)-1)
        copia.pop(len(copia)-1)
            
        if sort == copia:
            return contador
            
        contador+=1
    return 0 # el stack está ordenado

def showtime(path):
    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = select_col(layout)
 
    while seleccionada != -1:
        print("SELECCIONADA:", seleccionada)
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        for s in layout.stacks:
            print(s)
        print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        for s in layout.stacks:
            print(s)
        print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = select_col(layout)



        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")
    print("SELECCIONADA:", seleccionada)
    print(len(layout.moves),"\t",layout.unsorted_stacks)

def solve(path):
    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = select_col(layout)
 
    while seleccionada != -1:
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = select_col(layout)

        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")
    print(len(layout.moves),"\t",layout.unsorted_stacks)
 

def SDpp(layout, s_o, rank,quitar):
    capac = capacity(layout,s_o) # espacio libre
    ss_o = layout.stacks[s_o]
    
    #print()
    #print("THE DATA")
    #print(capac)
    #print(ss_o)
    #print()
    
    retirar=len(ss_o)-quitar
    
    while len(layout.stacks[s_o])>retirar:
        top = Layout.gvalue(ss_o)
        #print ("TOP: "+str(top))
        
        slack = capac-len(ss_o) # holgura
        #print ("SLACK: "+str(slack))
        #SEPARADOR############################
        #print("\n instancia: \n")
        #for s in layout.stacks:
            #print (s)
        #SEPARADOR############################
        #print("Columna seleccionada: ")
        #print (s_o)
        #SEPARADOR############################
        max_pos=rank[top]+slack
        #print()
        s_d = Layout.select_destination_stack (layout, s_o, max_pos=rank[top]+slack)
        pos = layout.H - len(layout.stacks[s_d])
        if rank[top] < pos - slack:  # rellenar stack s_d
            #c=layout.stacks[s_o].pop(-1)
            fill_stack(layout, s_d, (pos - slack) - rank[top], s_o, rank)
            #layout.stacks[s_o].append(c)

        capac -= 1
        layout.move(s_o,s_d)
        
def SFpp(layout, s_d, rank, n):
    ub = 100
    cont = 0
    for r in rank:
        while True:
            (s, pos) = search_highest(layout, r, ub, s_d)
            
            if layout.sorted_elements[s] > len(layout.stacks[s])+pos: break #element is sorted
            c=force_move(layout, s, pos, s_d)
            ub=r
            cont+=1
            if cont == n: return
            if r==c: break
    while cont < n:
        Greedy.SF_move_d(layout, s_d)
        cont += 1

#SELECT SEMI RANDOM COLUMN
def SSRC(layout):
    
    size=len(layout.stacks)
    best=1000000000
    select=-1
    
    posibles = []

    for i in range (0,size):

        aux = layout.is_sorted_stack(i)

        if aux == False:
            posibles.append(i)

    #Me di cuenta que con el que mayor problemas tiene tenemos menos problemas (necesita menos espacio al contrario del que tenga menos problemas)

    #Si no hay posibles se retorna -1 
    if len(posibles) == 0:
        return -1    
    
    #En casoq ue si hay posibles
    
    for i in posibles:
        aux = comparacion(layout.stacks[i])
        if aux < best:
            best = aux
            select = i 
    return select   

def SSRC2(layout):
    
    size=len(layout.stacks)
    best=-1000000000
    select=-1
    
    posibles = []

    for i in range (0,size):

        aux = layout.is_sorted_stack(i)

        if aux == False:
            posibles.append(i)

    #Me di cuenta que con el que mayor problemas tiene tenemos menos problemas (necesita menos espacio al contrario del que tenga menos problemas)

    #Si no hay posibles se retorna -1 
    if len(posibles) == 0:
        return -1    
    
    #En casoq ue si hay posibles
    
    for i in posibles:
        aux = comparacion(layout.stacks[i])
        if aux > best:
            best = aux
            select = i 
    return select   
    
def comparacion(stack):
    sort=copy.deepcopy(stack)
    sort.sort(reverse=True)
    copia=copy.deepcopy(stack)

    contador = 0

    while sort != copia:

        sort.pop(len(sort)-1)
        copia.pop(len(copia)-1)
        contador += 1


    return contador


    
    
def showtime2(path):
    layout=read(path)
    print ("Estado inicial: ")
    for s in layout.stacks:
        print(s)
    print()
    seleccionada = SSRC(layout)
 
    while seleccionada != -1:
        print("SELECCIONADA:", seleccionada)
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        for s in layout.stacks:
            print(s)
        print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        for s in layout.stacks:
            print(s)
        print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = SSRC(layout)



        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")
    print("SELECCIONADA:", seleccionada)
    print(len(layout.moves),"\t",layout.unsorted_stacks)
    print()

from timeit import timeit

def solve2(path):
    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = SSRC(layout)
 
    while seleccionada != -1:
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = SSRC(layout)

        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")
    print(str(len(layout.moves))+"\t"+str(layout.unsorted_stacks))

def solve2P5(path):
    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = SSRC2(layout)
 
    while seleccionada != -1:
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = SSRC2(layout)

        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")
    print(str(len(layout.moves))+"\t"+str(layout.unsorted_stacks))



def solve3(path):
    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = SSRC2(layout)
 
    while seleccionada != -1:
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = SSRC2(layout)

        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")

    #De la version 2
    movs = layout.moves
    sort = layout.unsorted_stacks

    layout=read(path)
    #print ("Estado inicial: ")
    #for s in layout.stacks:
    #    print(s)
    #print()
    seleccionada = SSRC(layout)
 
    while seleccionada != -1:
        #Se calcula cuantos contenedores deben retirarse
        columna=copy.deepcopy(layout.stacks[seleccionada])
        retirar = remove_until(columna)

        #Se calculan los ranks
        rank = ranks(layout.stacks[seleccionada])

        #Se retiran los elementos de la columna
        SDpp(layout, seleccionada, rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se quitan los contenedores que se mantuvieron en su lugar para que no tire error la siguiente funcion
        for item in layout.stacks[seleccionada]:
            rank.pop(item, None)


        SFpp(layout,seleccionada,rank,retirar)
        #for s in layout.stacks:
        #    print(s)
        #print("\n")

        #Se vuelve a seleccionar una columna
        seleccionada = SSRC(layout)

        #print(layout.stacks)
    #print("Estado final")
    #for s in layout.stacks:
        #print(s)
    #print()
    #print("Movimientos")

    if sort == 0:

        if layout.unsorted_stacks == 0:

            if layout.moves >= movs:
                print(str(len(layout.moves))+"\t"+str(layout.unsorted_stacks))
            else:
                print(str(len(movs))+"\t"+str(sort))
    else:
        
        print(str(len(layout.moves))+"\t"+str(layout.unsorted_stacks))


def readBF(path):
    flag = 0
    #Columnas
    columns = 0
    #Total contenedores
    total = 0
    maxSize = 0
    stacks = []
    with open(path) as file:
        for line in file:
            s = line.split()
            #Si es la primera linea entonces se guarda las columnas y el total
            if flag == 0:
                #Actualiza columns
                columns = s[0]
                #Actualiza total
                total = s[1]
                #Actualiza flag para que no pase denuevo
                flag=1
            #En caso contrario sen otros datos de interes
            else:
                #Se ve el size mayor de la columna para definirlo automatico
                aux=int(s[0])
                if maxSize <= aux:
                    maxSize = aux;

                #Se crean los stacks
                temporalStack = []
                for i in range(1,len(s)):
                    temporalStack.append(int(s[i]))
                stacks.append(temporalStack)
    #Se crea el layout
    layout = Layout.Layout(stacks, maxSize)
    
    return layout