# Question: https://projecteuler.net/problem=345

# Modeling language: ZIMPL (https://zimpl.zib.de/download/zimpl.pdf)
# Solver: SCIP (https://scip.zib.de/index.php#download)

import subprocess

def get_scip_result():
    stdout = subprocess.run(["scip", "-c", "read problem345.zpl optimize display solution quit"], stdout = subprocess.PIPE).stdout.decode('utf-8')
    for line in stdout.split("\n"):
        if line.startswith("objective value:"):
            break
    return int(line.split(':')[1].strip())

def generate_input(g, filename):
    with open(filename, "w") as f:
        f.write("# row, col, val\n")
        for row in g:
            for col in g[row]:
                val = g[row][col]
                f.write("{} {} {}\n".format(row, col, val))

def remove_input(filename):
    subprocess.run(["rm", filename])

FILENAME = "input.txt"
sum = 0
row = 0
with open("inputs/p345_matrix.txt", "r") as f:
    g = {}
    for row, line in enumerate(f):
        for col, val in enumerate(list(line.strip().split())):
            val = int(val)
            if not row+1 in g:
                g[row+1] = {}
            g[row+1][col+1] = val
    generate_input(g, FILENAME)
    result = get_scip_result()
    remove_input(FILENAME)
print(result)


