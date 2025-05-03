import A_star_search_algo as astar
import heuristics as h
def inversion_count(arr,n):
    lin = []
    for i in range(n):
        for j in range(n):
            if arr[i][j] == 0:
                continue
            lin.append(arr[i][j])
    inv_count = 0
    #print(lin)
    for i in range(len(lin)):
        for j in range(i+1,len(lin)):
            if lin[i] > lin[j]:
                #print(lin[i],lin[j])
                inv_count += 1
    return inv_count            

def main():
    n = int(input("Enter the Grid Size: "))
    adj = [[0]*n]*n
    print("Enter the grid: ")
    adj = [list(map(int, input().split())) for _ in range(n)]
    goal = [[0 for _ in range(n)] for _ in range(n)]
    ok = 0
    print("Enter your preferred Heuristic:")
    print("1. Hamming Distance")
    print("2. Manhattan Distance")
    print("3. Euclidean Distance")
    print("4. Linear Conflict")
    heuristic = int(input())
    for i in range(n):
        for j in range(n):
            if i == n-1 and j == n-1:
                ok = 1
                break
            goal[i][j] = n*i + j + 1
        if ok == 1:
            break    
    goal[n-1][n-1] = 0
    #print(goal)
    inv_cnt = inversion_count(adj,n)
    #print(inv_cnt)
    if n % 2 != 0:
        if inv_cnt % 2 != 0:
            print("Unsolvable puzzle")
            return
    else:
        row = 0
        for i in range(n):
            for j in range(n):
                if adj[i][j] == 0:
                    row = n-i
                    break
        if (row % 2 == 0 and inv_cnt % 2 == 0) or (row % 2 != 0 and inv_cnt % 2 != 0):
            print("Unsolvable puzzle")
            return
    
    if heuristic == 1:
        obj = astar.N_Puzzle(adj,goal,h.Hamming)
    elif heuristic == 2:    
        obj = astar.N_Puzzle(adj,goal,h.Manhattan)
    elif heuristic == 3:
        obj = astar.N_Puzzle(adj,goal,h.Euclidean)
    elif heuristic == 4:
        obj = astar.N_Puzzle(adj,goal,h.LinearConflict)    
    else:
        print("Invalid Heuristic")
        return
    closed_list,steps,explored_nodes,expanded_nodes = obj.algo()
    print("Minimum number of moves: ",steps)
    # for states in closed_list:
    #     for row in states:
    #         print(*row)
    #     print()    
    
    print("Total number of nodes explored: ",explored_nodes)
    print("Total number of nodes expanded: ",expanded_nodes)    


if __name__ == '__main__':
    main()