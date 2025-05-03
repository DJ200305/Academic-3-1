def Hamming(arr,goal):
    n = len(arr[0])
    cnt = 0
    for i in range(n):
        for j in range(n):
            if arr[i][j] != goal[i][j]:
                cnt += 1
    return cnt

def Manhattan(arr,goal):
    dict = {}
    n = len(arr[0])
    for i in range(n):
        for j in range(n):
            dict[goal[i][j]] = (i,j)
    #print(dict)        
    dist = 0
    for i in range(n):
        for j in range(n):
            x,y = dict[arr[i][j]]
            dist += abs(x-i) + abs(y-j)
    return dist

def Euclidean(arr,goal):
    dict = {}
    n = len(arr[0])
    for i in range(n):
        for j in range(n):
            dict[goal[i][j]] = (i,j)
    dist = 0
    for i in range(n):
        for j in range(n):
            x,y = dict[arr[i][j]]
            dist += ((x-i)**2 + (y-j)**2)**0.5
    return dist

def LinearConflict(arr,goal):
    mandist = Manhattan(arr,goal)
    n = len(arr[0])
    conflicts = 0
    for i in range(n):
        for j in range(n):
            for k in range(j+1,n):
                if arr[i][j] != 0 and arr[i][k] != 0:
                    if(goal[i][j] // n == i and goal[i][k] // n == i) and (goal[i][j] % n > goal[i][k] % n and arr[i][j] > arr[i][k]):
                        conflicts += 1
    for j in range(n):
        for i in range(n):
            for k in range(i+1,n):
                if arr[i][j] != 0 and arr[k][j] != 0:
                    if(goal[i][j] % n == j and goal[k][j] % n == j) and (goal[i][j] // n > goal[k][j] // n and arr[i][j] > arr[k][j]):
                        conflicts += 1
    return mandist + 2*conflicts