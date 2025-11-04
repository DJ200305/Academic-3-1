import ai_chain_reaction_game as game
import heuristics
import sys
import os
def taking_input():
    
    
    with open("input.txt","r") as f:
        lines = f.read().splitlines()

    lines1 = []
    for line in lines:
        if line.startswith("AI Move:"):
            continue
        lines1.append(line)
    grid = []
    for i in range(len(lines1)):
        a = lines1[i].split()
        row = []
        for j in range(len(a)):
            length = len(a[j])
            if(length == 1):
                cell = game.Cell(i,j,0,-1)
            else:
                if a[j].endswith("R"):
                    cell = game.Cell(i,j,int(a[j][0]),0) 
                else:
                    cell = game.Cell(i,j,int(a[j][0]),1) 
            row.append(cell)
        grid.append(row)
    return grid    

def print_grid(grid):
    results = []
    for i in range(len(grid)):
        row = []
        for j in range(len(grid[i])):
            if grid[i][j].owner == -1:
                row.append(str(grid[i][j].count))
            elif grid[i][j].owner == 1:
                row.append(str(grid[i][j].count) + "B")
            else:
                row.append(str(grid[i][j].count) + "R")
        results.append(" ".join(row)) 
    return results

def game_over():
    with open("input.txt", "r") as f:
        lines = f.read().splitlines()
    over = False
    for line in lines:
        if line.startswith("Game Over"):
            over = True
            break
    return over

def main():
    global move
    if not os.path.exists("moves.txt"):
        with open("moves.txt", "w") as f:
            f.write("0")
    with open("moves.txt", "r") as f:
        move = int(f.read())
    if move == 0:
        grid = [[game.Cell(i,j,0,-1) for j in range(6)] for i in range(9)]
        state = game.State(grid, heuristics.stability)
        for i in range(9):
            for j in range(6):
                if state.grid[i][j].count >= state.grid[i][j].cmass:
                    state = game.explosion(state, i, j)
        result = []
        ai_state = game.ai_move(state)
        game.printing_state(ai_state)
        print("From ai_main_1,move 0")
        status = "AI_1 Move:"      
        result = [f"{status}",*print_grid(ai_state.grid)]
        with open("input.txt","w") as f: 
            for line in result:
                f.write(line + "\n")     
                
    else:
        grid = taking_input()    
        state = game.State(grid,heuristics.stability) 
        for i in range(9):
            for j in range(6):
                if state.grid[i][j].count >= state.grid[i][j].cmass:
                    state = game.explosion1(state, i, j)    
        result = []
        game.printing_state(state)
        print(move)
        print(state.isTerminated())
        print("From ai_main_1")
        if move != 0 and state.isTerminated():
            status = "Game Over"
            winner = state.winner() 
            result = [f"{status}", *print_grid(state.grid) ,f"{winner}"]
            with open("input.txt","w") as f: 
                for line in result:
                    f.write(line + "\n")
            if os.path.exists("moves.txt"):
                os.remove("moves.txt")
            if os.path.exists("moves1.txt"):
                os.remove("moves1.txt")            
        else:        
            ai_state = game.ai_move(state)
            status = "AI_1 Move:"
            result = [f"{status}",*print_grid(ai_state.grid)]
    
       
            with open("input.txt","w") as f: 
                for line in result:
                    f.write(line + "\n") 
    if game_over():
            if os.path.exists("moves.txt"):
                os.remove("moves.txt")
            if os.path.exists("moves1.txt"):
                os.remove("moves1.txt")
            return    
    move += 1
    with open("moves.txt", "w") as f:
        f.write(str(move))
if __name__ == "__main__":
    
    main()  