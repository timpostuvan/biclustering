import os

def read_data(file_path):
    f = open(file_path, "r")
    value = float(f.readline())
    f.close()

    return value



number_of_iterations = 100
directory = ["random", "random-positive", "random-binary", "image", "biology"]
tags = ["tsp", "l_spanning_tree", "l_2_closest", "l_1_closest", "u_spanning_tree", "u_greedy_path", "combined"]
evaluation = ["l_spanning_tree", "l_2_closest", "l_1_closest", "u_spanning_tree", "u_greedy_path", "combined"]

for i in range(3, 4):
    output_file = directory[i] + "/results.txt" 
    f = open(output_file, "w+")

    for j in range(len(tags)):
        f.write("{:^19}".format(tags[j]));
        if(j != len(tags) - 1):
            f.write("|")
    f.write("\n")


    for it in range(number_of_iterations):
        results = list()

        command = "./distance " + str(i)
        os.system(command)

        command = "gurobi.sh tsp.py"
        result_file = "result_tsp.txt" 
        os.system(command)
        results.append(read_data(result_file))


        for j in range(len(evaluation)):
            command = "./approximation " + str(j)
            result_file = "result_" + evaluation[j] + ".txt" 
            os.system(command)
            results.append(read_data(result_file))

        for j in range(len(results)):
            f.write("{:^20.5f}".format(results[j]));
        f.write("\n")

    f.close()
    

# calculate coefficients
os.system("python3 coefficients.py")