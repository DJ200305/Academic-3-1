import random
random.seed(42)
class SemiGreedy:
    def __init__(self,adj,V,alpha):
        self.adj = adj
        self.V = V
        
        self.alpha = alpha

    def sigma(self,v,X,Y):
        sigma_x = sum(w for u, w in self.adj[v] if u in X)
        sigma_y = sum(w for u, w in self.adj[v] if u in Y)
        return sigma_x,sigma_y
    
    def algo(self):
        X = set()
        Y = set()
        U = set(self.V)
        while U:
            vals = {}
            for v in U:
                sigma_x, sigma_y = self.sigma(v, X, Y)
                vals[v] = max(sigma_x,sigma_y)

            wmin = min(list(vals.values()))
            wmax = max(list(vals.values()))
            miu = wmin + self.alpha * (wmax - wmin)
            rcl = []
            for v,val in vals.items():
                if val >= miu:
                    rcl.append(v)

            sel = random.choice(rcl)   
            sigma_x, sigma_y = self.sigma(sel, X, Y)
            if sigma_x > sigma_y:
                Y.add(sel)
            else:
                X.add(sel)

            U.remove(sel)

        return X,Y

    def cut_weight(self):
        X,Y = self.algo()
        cut_weight = 0
        for u in X:
            for v,w in self.adj[u]:
                if v in Y:
                    cut_weight += w
        return cut_weight                 
    