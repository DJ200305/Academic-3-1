from collections import defaultdict
import Randomized_1_heuristic_2105084 as R1
import Greedy_heuristic_2105084 as G1
import Semi_Greedy_heuristic_2105084 as SG1
import grasp_algo_2105084
import Local_Search_2105084 as LS
import os
import csv
KNOWN_BEST_VALUES = {
    'G1': 12078, 'G2': 12084, 'G3': 12077,
    'G11': 627, 'G12': 621, 'G13': 645,
    'G14': 3187, 'G15': 3169, 'G16': 3172,
    'G22': 14123, 'G23': 14129, 'G24': 14131,
    'G32': 1560, 'G33': 1537, 'G34': 1541,
    'G35': 8000, 'G36': 7996, 'G37': 8009,
    'G43': 7027, 'G44': 7022, 'G45': 7020,
    'G48': 6000, 'G49': 6000, 'G50': 5988,
}

def read_file(filepath):
    with open(filepath, 'r') as file:
        lines = file.readlines()
        n, m = map(int, lines[0].split())
        adj = defaultdict(list)
        vertices = set()
        for line in lines[1:]:
            u, v, w = map(int, line.split())
            vertices.add(u)
            vertices.add(v)
            adj[u].append((v, w))
            adj[v].append((u, w))
    return n,m,adj,vertices

def process_files(filepath):
    n,m,adj,vertices = read_file(filepath)
    fn = os.path.basename(filepath)
    name = fn.split('.')[0].upper()
    results = []

    r1 = R1.Randomized_1_heuristic(adj,vertices,5)
    g1 = G1.Greedy_heuristic(adj,vertices)
    sg = SG1.SemiGreedy(adj,vertices,0.5)
    ls = LS.LocalSearch(adj,vertices)
    grasp = grasp_algo_2105084.GRASP(adj,vertices,30,0.5)

    cut_weight_r1,_,_ = r1.algo()
    cut_weight_g1 = g1.cut_weight()
    cut_weight_sg = sg.cut_weight()
    best_soln, cut_weight_grasp = grasp.algo()
    
    
    bests = KNOWN_BEST_VALUES.get(name, "N/A")
    
    for i in range(2):
        _,X,Y = r1.algo() 
        ls.init_partition(X,Y)
        cut_weight_ls = ls.cut_weight()
        results.append(cut_weight_ls)
    
    cut_weight_local = sum(results) // len(results)

    row = [
        name,              # Problem name
        n,                 # |V|
        m,                 # |E|
        cut_weight_r1,     # Randomized-1
        cut_weight_g1,     # Greedy-1
        cut_weight_sg,     # Semi-Greedy-1
        2,                 # Local-1: No. of iterations
        cut_weight_local,  # Local-1: Average value
        25,                # GRASP-1: No. of iterations
        cut_weight_grasp,  # GRASP-1: Best value
        bests         # Known best value
    ]
    return row


def main():
    # n,m = map(int, input().split())
    # adj = defaultdict(list)
    # vertices = set()
    # for i in range(m):
    #     u,v,w = map(int, input().split())
    #     vertices.add(u)
    #     vertices.add(v)
    #     adj[u].append((v,w))
    #     adj[v].append((u,w))

    # filepath = 'graph_GRASP/set1/g5.rud'
    # n,m,adj,vertices = read_file(filepath)

    # r1 = R1.Randomized_1_heuristic(adj,vertices,2)
    # g1 = G1.Greedy_heuristic(adj,vertices)
    # sg = SG1.SemiGreedy(adj,vertices,0.5)
    # cut_weight_r1,_,_ = r1.algo()
    # print(f"Randomized: {cut_weight_r1}")
    # cut_weight_g1 = g1.cut_weight()
    # print(f"Greedy: {cut_weight_g1}")
    # cut_weight_sg = sg.cut_weight()
    # print(f"Semi-Greedy: {cut_weight_sg}")

    # grasp = grasp_algo.GRASP(adj,vertices,50,0.5)
    # best_soln, cut_weight = grasp.algo()
    # print(cut_weight)
    
    # r1 = R1.Randomized_1_heuristic(adj,vertices,n)
    # ls = LS.LocalSearch(adj,vertices)
    # locals = []
    # for i in range(5):
    #     _,X,Y = r1.algo() 
    #     ls.init_partition(X,Y)
    #     cut_weight = ls.cut_weight()
    #     locals.append(cut_weight)
    
    # avg = sum(locals) // len(locals)
    # print(f"Local Search: {avg}")


    output_csv = "2105084.csv"

    # if os.path.exists(output_csv):
    #     os.remove(output_csv)


    # with open(output_csv, 'a',newline="") as f:
    #     writer = csv.writer(f)
    #     writer.writerow(["Problem","","","Constructive algorithm","","","Local search","", "GRASP","", "Known best solution or upper bound"])
    #     writer.writerow(["Name", "|V| or n", "|E| or m", "Simple Randomized or Randomized-1", "Simple Greedy or Greedy-1", "Semi-greedy-1", "Simple local or local-1","","GRASP-1","",""])
    #     writer.writerow(["", "", "", "", "", "", "No. of iterations", "Average value", "No. of iterations", "Best value", ""])

    #     for i in range(1, 19):
    #         filepath = f'graph_GRASP/set1/g{i}.rud'
    #         if os.path.exists(filepath):
    #             row = process_files(filepath)
    #             writer.writerow(row)
    #         else:
    #             print(f"File {filepath} does not exist.")


if __name__ == "__main__":
    main()          
                