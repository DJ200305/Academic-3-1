import Semi_Greedy_heuristic_2105084 as SG
import Local_Search_2105084 as LS
class GRASP:
    def __init__(self,adj,V,n,alpha):
        self.adj = adj
        self.V = V
        self.n = n
        self.alpha = alpha

    def algo(self):
        w_x1 = -1
        best_soln = (set(),set())
        for i in range(self.n):    
            obj = SG.SemiGreedy(self.adj,self.V,self.alpha)
            X,Y = obj.algo()
            obj2 = LS.LocalSearch(self.adj,self.V)
            obj2.init_partition(X,Y)
            cut_weight = obj2.cut_weight()

            if i == 0:
                w_x1 = cut_weight
                best_soln = X,Y
            elif cut_weight > w_x1:
                w_x1 = cut_weight
                best_soln = X,Y
        
        return best_soln,w_x1