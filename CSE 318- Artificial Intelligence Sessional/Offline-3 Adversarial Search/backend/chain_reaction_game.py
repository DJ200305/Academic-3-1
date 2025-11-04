from collections import deque
import copy

class Cell:
    def __init__(self,r,c,count,owner):  
        self.r = r
        self.c = c
        self.count = count
        self.owner = owner             #initially no one owns the cell
        self.cmass = self.critical_mass()  

    def pos(self):
        if self.r in [0, 8] and self.c in [0, 8]:
            return "corner",self.r, self.c
        elif self.r in [0, 8] or self.c in [0, 8]:
            return "edge",self.r, self.c
        return "normal", self.r, self.c    

    def critical_mass(self):
        if self.r == 0 and self.c == 0 or self.r == 0 and self.c == 5 or self.r == 8 and self.c == 0 or self.r == 8 and self.c == 5:
            return 2
        elif self.r in [0, 8] or self.c in [0, 5]:
            return 3
        else:
            return 4

class State:
    def __init__(self,grid,heuristic):
        self.grid = grid
        self.heuristic = heuristic  
        self.player1 = 0
        self.player2 = 0

    def isTerminated(self):
        cnt = 0
        cnt1 = 0
        for i in range(9):
            for j in range(6):
                if self.grid[i][j].owner == 1:
                    cnt += 1
                elif self.grid[i][j].owner == 0:
                    cnt1 += 1

        if cnt == 0 or cnt1 == 0:
            return True
        return False             
       
    def winner(self):
        for i in range(9):
            for j in range(6):
                if self.grid[i][j].owner == 1 :
                    self.player1 += 1
                else:
                    self.player2 += 1

        if self.player1 == 0:
            return "player2"
        else:
            return "player1"
max_depth = 1



def explosion(state,i,j):
    
    q = deque()
    q.append((i,j))  
    lim = 0
    while q and lim < 2: 
        x,y = q.popleft() 
        if state.grid[x][y].count >= state.grid[x][y].cmass:
            for dx, dy in [(-1,0),(1,0),(0,-1),(0,1)]:
                    if (0 <= x+dx < 9) and (0 <= y+dy < 6):
                        nx, ny = x + dx, y + dy
                        state.grid[nx][ny].count += 1
                        state.grid[nx][ny].owner = state.grid[x][y].owner
                        if state.grid[nx][ny].count >= state.grid[nx][ny].cmass and state.grid[nx][ny].owner != -1:
                            q.append((nx, ny))

            state.grid[x][y].count = 0
            state.grid[x][y].owner = -1     
        lim += 1             


    return state  

def explosion1(state,i,j):
    
    q = deque()
    q.append((i,j))  
    lim = 0
    while q: 
        x,y = q.popleft() 
        if state.grid[x][y].count >= state.grid[x][y].cmass:
            for dx, dy in [(-1,0),(1,0),(0,-1),(0,1)]:
                    if (0 <= x+dx < 9) and (0 <= y+dy < 6):
                        nx, ny = x + dx, y + dy
                        state.grid[nx][ny].count += 1
                        state.grid[nx][ny].owner = state.grid[x][y].owner
                        if state.grid[nx][ny].count >= state.grid[nx][ny].cmass and state.grid[nx][ny].owner != -1:
                            q.append((nx, ny))

            state.grid[x][y].count = 0
            state.grid[x][y].owner = -1     
                         

 
    return state

def printing_state(state):
    for i in range(9):
        row = []
        for j in range(6):
            if state.grid[i][j].owner == -1:
                row.append(str(state.grid[i][j].count))
            elif state.grid[i][j].owner == 0:
                row.append(str(state.grid[i][j].count) + "B")
            else:
                row.append(str(state.grid[i][j].count) + "R")
        print(" ".join(row))                  

def algo(state,depth,MAX,alpha,beta):
    if depth == max_depth:
        return state.heuristic(state.grid),None,None
    x,y = None, None
    if MAX:
        m_eval = -100000
         
        for i in range(9):  
            for j in range(6):
                if (state.grid[i][j].owner == 0) or (state.grid[i][j].owner == -1):
                    nstate = copy.deepcopy(state)
                    nstate.grid[i][j].owner = 0
                    nstate.grid[i][j].count += 1
                    
                    
                    if nstate.grid[i][j].count >= nstate.grid[i][j].cmass:
                        nstate = explosion(nstate, i, j)
                    
                    pre_eval,_,_ = algo(nstate, depth + 1, False, alpha, beta)
                    
                    if pre_eval >= m_eval:
                        m_eval = pre_eval
                        x,y = i,j
                    alpha = max(alpha, m_eval)    
                    if beta <= alpha:
                        break
        return m_eval,x,y        

    else:
        m_eval = 100000
        
        for i in range(9):
            for j in range(6):
                if (state.grid[i][j].owner == 1) or (state.grid[i][j].owner == -1):
                    nstate = copy.deepcopy(state)
                    nstate.grid[i][j].owner = 1
                    nstate.grid[i][j].count += 1
                    
                    if nstate.grid[i][j].count >= nstate.grid[i][j].cmass:
                       
                       nstate = explosion(nstate, i, j)
                    
                    pre_eval,_,_ = algo(nstate, depth + 1, True, alpha, beta)
                    if pre_eval <= m_eval:
                        m_eval = pre_eval
                        x,y = i,j
                    beta = min(beta, m_eval)    
                    if beta <= alpha:
                        break         
        return m_eval,x,y
                   
def ai_move(state):
    
    best_eval, x, y = algo(state, 0, True, -100000, 100000)

    if x is not None and y is not None:
        state.grid[x][y].owner = 0 
        state.grid[x][y].count += 1
        
        if state.grid[x][y].count >= state.grid[x][y].cmass:
            
            state = explosion1(state, x, y)
    return state                   
                     
                  

        


        