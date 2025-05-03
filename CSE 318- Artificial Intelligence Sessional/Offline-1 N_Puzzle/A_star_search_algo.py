import heapq
import copy
class N_Puzzle:
    def __init__(self,initial,goal,heuristic):
        self.board = initial
        self.goal = goal
        self.n = len(initial[0])
        self.heuristic = heuristic
        self.exp_node = 0
        self.expa_node = 0
        self.prev = []
        self.g = 0
        self.f = 0
    
    def is_goal(self,board):
        return self.goal == board
    
    def generate_neighbors(self,curr):
        neighbors = []
        for i in range(self.n):
            for j in range(self.n):
                if curr[i][j] == 0:
                    x,y = i,j
                    break      
        if x+1 < self.n:
            temp = copy.deepcopy(curr)
            temp[x][y],temp[x+1][y] = temp[x+1][y],temp[x][y]
            neighbors.append(temp)
        if x-1 >= 0:
            temp = copy.deepcopy(curr)
            temp[x][y],temp[x-1][y] = temp[x-1][y],temp[x][y]
            neighbors.append(temp)
        if y+1 < self.n:
            temp = copy.deepcopy(curr)
            temp[x][y],temp[x][y+1] = temp[x][y+1],temp[x][y]
            neighbors.append(temp)
        if y-1 >= 0:
            temp = copy.deepcopy(curr)
            temp[x][y],temp[x][y-1] = temp[x][y-1],temp[x][y]
            neighbors.append(temp)
        return neighbors             

    def algo(self):
        open_list = []
        closed_list = []
        self.f = self.heuristic(self.board,self.goal) + self.g
        heapq.heappush(open_list,(self.f,self.g,self.heuristic(self.board,self.goal),self.board))
        self.exp_node += 1
        while open_list:
            f,g,h,curr = heapq.heappop(open_list)
            if self.is_goal(curr):
                closed_list.append(curr)
                return closed_list,self.g,self.exp_node,self.expa_node
            closed_list.append(curr)
            self.expa_node += 1
            neighbors = self.generate_neighbors(curr)
            for neighbor in neighbors:
                if neighbor in closed_list:
                    continue
                self.g = g + 1
                self.f = self.heuristic(neighbor,self.goal) + g
                if neighbor not in open_list:
                    heapq.heappush(open_list,(self.f,self.g,self.heuristic(neighbor,self.goal),neighbor))
                    self.exp_node += 1
        return closed_list,self.g,self.exp_node,self.expa_node            
    


