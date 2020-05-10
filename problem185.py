# Question: https://projecteuler.net/problem=185

import subprocess

with open("inputs/p185_constraints.txt", "r") as f:
    constraints = []
    for line in f:
        line = line.strip().split(" ;")
        line[1] = line[1].split(" correct")[0]
        constraints.append((line[0], line[1]))

n_cols = len(constraints[0][0])
lines = []
lines.append("set digits := {{0..9}};")
lines.append("set cols := {{0..{}}};".format(n_cols - 1))
lines.append("")
lines.append("var X[cols * digits] binary;")
lines.append("var Y binary;")
lines.append("")
lines.append("minimize nothing: Y;")
lines.append("")
for i, constraint in enumerate(constraints):
    lines.append("subto constraint{}:".format(i))
    digits, n_corrects = constraint
    var = []
    for col, digit in enumerate(digits):
        var.append("X[{}, {}]".format(col, digit))
    lines.append("  {} == {};".format(' + '.join(var), n_corrects))
    lines.append("")
lines.append("")
lines.append("subto one_digit_per_col:")
lines.append("    forall <col> in cols:")
lines.append("        sum <digit> in digits:")
lines.append("            X[col, digit]")
lines.append("        == 1;")
lines.append("")


MODEL_FILENAME = "problem185.zpl"

with open(MODEL_FILENAME, "w") as f:
    f.write('\n'.join(lines))

stdout = subprocess.run(["scip", "-c", "read problem185.zpl optimize display solution quit"], stdout = subprocess.PIPE).stdout.decode('utf-8')
subprocess.run(["rm", MODEL_FILENAME])

ans = [None] * n_cols
for line in stdout.split('\n'):
    if line.startswith("X#"):
        line = line.split(' ')[0]
        line = line.split('#')
        ans[int(line[1])] = line[2]
ans = ''.join(ans)
print(ans)