import heapq
class LocalSearch:
    def __init__(self,adj,V):
        self.adj = adj
        self.V = V
    
    def init_partition(self,X,Y):
        self.X = X
        self.Y = Y
        
    def calc_delta(self,v):
        sigma_x = 0
        sigma_y = 0
        for u,w in self.adj[v]:
            if u in self.X:
                sigma_x += w
            else:
                sigma_y += w
        if v in self.X:
            return sigma_x - sigma_y
        else:
            return sigma_y - sigma_x     
    
    def cut_weight(self):
        X,Y = self.algo()
        cut_weight = 0
        for u in X:
            for v,w in self.adj[u]:
                if v in Y:
                    cut_weight += w
        return cut_weight 

    def algo(self):
        dict1 = {}

        while True:
            for v in self.V:
               delta = self.calc_delta(v)
               dict1[v] = delta
           
            max_key = max(dict1,key=dict1.get)
            if dict1[max_key] <= 0:
                break

            if max_key in self.X:
                self.X.remove(max_key)
                self.Y.add(max_key)
            elif max_key in self.Y:
                self.Y.remove(max_key)
                self.X.add(max_key)
        return self.X,self.Y