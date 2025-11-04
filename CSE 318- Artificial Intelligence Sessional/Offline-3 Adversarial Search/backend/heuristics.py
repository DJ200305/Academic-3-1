from collections import deque
import copy

def explosion(grid,i,j):
        cnt = 0
        time = 0
        captured = 0
        ngrid = copy.deepcopy(grid)
        q = deque()
        q.append((i,j))
        while q and time < 2:
            x,y = q.popleft() 
            if ngrid[x][y].count >= ngrid[x][y].cmass:
                for dx, dy in [(-1,0),(1,0),(0,-1),(0,1)]:
                        if (0 <= x+dx < 9) and (0 <= y+dy < 6):
                            nx, ny = x + dx, y + dy
                            ngrid[nx][ny].count += 1
                            if ngrid[nx][ny].owner != ngrid[x][y].owner:
                                captured += 1
                            ngrid[nx][ny].owner = ngrid[x][y].owner
                            if ngrid[nx][ny].count >= ngrid[nx][ny].cmass and ngrid[nx][ny].owner != -1:
                               q.append((nx, ny))
                            cnt += 1
                ngrid[x][y].count = 0
                ngrid[x][y].owner = -1                  
                
            time += 1
        return cnt,captured

def CellControl(grid):
    player1 = 0
    player2 = 0
    c = 0
    e = 0
    ce = 0
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j].owner == 1:
                player2 += 1
            elif grid[i][j].owner == 0:
                position,_,_ = grid[i][j].pos()
                if position == "corner":
                    c += 1
                elif position == "edge":
                    e += 1
                else:
                    ce += 1        
                player1 += 1
    return ((player1 - player2)/(player1-player2+0.5))+(c*1.5+ce*0.5+e)

def ReactionPotential(grid):
    cnt1 = 0
    cnt2 = 0
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 1:
                cnt1 += 1/(0.9+grid[i][j].cmass-grid[i][j].count)
            elif grid[i][j].owner == 0:
                cnt2 += 1/(0.9+grid[i][j].cmass-grid[i][j].count)
    return cnt2 - cnt1 * 0.5

def EndgameProximity(grid):
    cnt1 = 0
    cnt2 = 0
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 1:
                cnt1 += grid[i][j].count
            elif grid[i][j].owner == 0:
                cnt2 += (grid[i][j].count)
    
    
    potential = 0
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 1 and grid[i][j].count >= grid[i][j].cmass:
                a,b = explosion(grid,i,j)
                potential += 10*a + 50*b
    return (1/(cnt1+1))+potential - (1/(cnt2+1))            

def stability(grid):
    stables = 0
    opps_cell = set()
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 1 and grid[i][j].count == grid[i][j].cmass-1:
                opps_cell.add((i,j))
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 0:
                stables += grid[i][j].cmass-grid[i][j].count            
    cnt = 0
    ok = True
    while(len(opps_cell) != 0 and ok):
        for dx,dy in [(-1,0),(1,0),(0,-1),(0,1)]:
            if len(opps_cell) == 0:
                ok = False
                break
            x,y = opps_cell.pop()
            if (0 <= x+dx < 9) and (0 <= y+dy < 6):
                cnt += 1

    return stables-cnt 

def criticalcell(grid):
    cnt1 = 0
    cnt2 = 0
    for i in range(9):
        for j in range(6):
            if grid[i][j].owner == 1:
                cnt1 += (1/grid[i][j].cmass)
            else:
                cnt2 += (1/grid[i][j].cmass)        
    return cnt2-cnt1*0.5 

def new_heuristic(grid):
    cnt1 = 0
    r = -1
    c = -1
    mx,rchoose = (-1,-1)
    q = deque()
    p = deque()
    for i in range(9):
        for j in range(6):
           if grid[i][j].owner == 0:
              if grid[i][j].count >= cnt1:
                  cnt1 = grid[i][j].count
                  q.append(i)
                  p.append(j)

    mx1 = -1
    check = 0
    while q:
        x = q.popleft()
        if x > mx1:
            mx1 = x

    return mx1*100                      

