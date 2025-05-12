class Greedy_heuristic:
    def __init__(self,adj,V):
        self.adj = adj
        self.V = V
        
    
    def compute_weight(self,z,X):
        weight = 0
        for v,w in self.adj[z]:
            if v in X:
                weight += w
        return weight

    def algo(self):
        X = set()
        Y = set()
        U = set()
        max_edge = (None,None)
        for i in self.V:
            U.add(i)
        mx = -1
        seen = set()
        for u in self.adj:
            for v,w in self.adj[u]:
                if (u,v) in seen or (v,u) in seen:
                    continue
                if u < v:
                    if w > mx:
                        mx = w
                        max_edge = (u,v)
                seen.add((u,v))        
        X.add(max_edge[0])
        Y.add(max_edge[1])
        U.remove(max_edge[0])
        U.remove(max_edge[1])

        for z in U:
            wx = self.compute_weight(z,X)
            wy = self.compute_weight(z,Y)
            if wx > wy:
                X.add(z)
            else:
                Y.add(z)
        return X,Y

    def cut_weight(self):
        X,Y = self.algo()
        cut_weight = 0
        for u in X:
            for v,w in self.adj[u]:
                if v in Y:
                    cut_weight += w
        return cut_weight        

