import random
random.seed(42)
class Randomized_1_heuristic:
    def __init__(self,adj,V,n):
        self.adj = adj
        self.V = V
        self.n = n
    def algo(self):
        total_cut_weight = 0
        for i in range(self.n):
            X = set()
            Y = set()
            for j in self.V:
                r = random.random()
                if r >= 0.5:
                    X.add(j)
                else:
                    Y.add(j)
            seen = set()        
            for u in self.adj:
                for v,w in self.adj[u]:
                    if (u,v) in seen or (v,u) in seen:
                        continue
                    if (u in X and v in Y) or (u in Y and v in X):
                        total_cut_weight += w
                    seen.add((u,v))
                            

        return total_cut_weight // self.n,X,Y                    