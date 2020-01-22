import os
import scipy.stats

directory = ["random", "random-binary", "image"]
tags = ["tsp", "l_spanning_tree", "l_2_closest", "l_1_closest", "u_spanning_tree", "u_greedy_path", "combined"]
evaluation = ["l_spanning_tree", "l_2_closest", "l_1_closest", "u_spanning_tree", "u_greedy_path", "combined"]


for ind in range(len(directory)):
    results_file = directory[ind] + "/results.txt"
    f = open(results_file, "r")
    f.readline()

    stats = list()

    line = f.readline()
    while(line != ""):
        line = ' '.join(line.split())    # remove spaces
        line = list(map(float, line.split(" ")))
        stats.append(line)
        line = f.readline()

    f.close()

    """
    for f in range(len(stats)):
        for j in range(len(stats[0])):
            print(stats[i][j], end = " ")
        print()
    """


    optimal = [stats[j][0] for j in range(len(stats))] 
    
    output_file = directory[ind] + "/statistics.txt" 
    f = open(output_file, "w+")

    f.write("{:<10}".format(""))
    for j in range(len(tags)):
        f.write("{:^19}".format(tags[j]));
        if(j != len(tags) - 1):
            f.write("|")
    f.write("\n")


    results = [[0 for i in range(len(tags))] for j in range(3)]
    for i in range(len(tags)):
        current = [stats[j][i] for j in range(len(stats))] 

        results[0][i] = scipy.stats.pearsonr(optimal, current)[0]
        results[1][i] = scipy.stats.spearmanr(optimal, current)[0]
        results[2][i] = scipy.stats.kendalltau(optimal, current)[0]


    for i in range(3):
        if(i == 0):
            f.write("{:<10}".format("pearson:"))
        
        if(i == 1):
            f.write("{:<10}".format("spearman:"))

        if(i == 2):
            f.write("{:<10}".format("kendall:"))


        for j in range(len(tags)):
            f.write("{:^20.5f}".format(results[i][j]));
        f.write("\n")

f.close()