# Question: https://projecteuler.net/problem=96

# Modeling language: ZIMPL (https://zimpl.zib.de/download/zimpl.pdf)
# Solver: SCIP (https://scip.zib.de/index.php#download)

import subprocess

def get_scip_result():
    stdout = subprocess.run(["scip", "-c", "read problem096.zpl optimize display solution quit"], stdout = subprocess.PIPE).stdout.decode('utf-8')

    x1, x2, x3 = 0, 0, 0
    for line in stdout.split("\n"):
        if line.startswith("X#1#1"):
            x1 = int(line[6])
        elif line.startswith("X#1#2"):
            x2 = int(line[6])
        elif line.startswith("X#1#3"):
            x3 = int(line[6])
    return x1, x2, x3

def generate_input(g, filename):
    with open(filename, "w") as f:
        f.write("# row, col, val, 1\n")
        for row in g:
            for col in g[row]:
                val = g[row][col]
                f.write("{} {} {} {}\n".format(row, col, val, 1))

def remove_input(filename):
    subprocess.run(["rm", filename])


FILENAME = "sudoku.txt"
sum = 0
row = 0
with open("inputs/p096_sudoku.txt", "r") as f:
    g = {}
    for row, line in enumerate(f):
        if row % 10 == 0:
            continue
        for col, val in enumerate(list(line.strip())):
            val = int(val)
            if val > 0:
                r = row % 10
                if not r in g:
                    g[r] = {}
                g[r][col+1] = val
        if row % 10 == 9:
            if g:
                generate_input(g, FILENAME)
                x1,x2,x3 = get_scip_result()
                if x1 == 0:
                    x1 = g[1][1]
                if x2 == 0:
                    x2 = g[1][2]
                if x3 == 0:
                    x3 = g[1][3]
                sum = sum + 100 * x1 + 10 * x2 + x3
            g = {}
    remove_input(FILENAME)
print(sum)


