# Question: https://projecteuler.net/problem=424

import subprocess

def generate_input(board, n, v, h, model_filename):
    line = []

    abc = "ABCDEFGHIJ"

    abc_map = {}

    line.append("set digits := {{0..9}};")
    line.append("set abc := {{ \"{}\" }};".format("\",\"".join(list(abc))))

    line.append("var X[digits * abc] binary;")

    line.append("var Y binary;")

    empty_tiles = []

    non_zeros = set()

    for i in range(n):
        for j in range(n):
            if board[i][j] == 'O':
                prefix = "row_{}_col_{}".format(i, j)
                st = []
                for k in range(10):
                    st.append("{}[{}] * {}".format(prefix, k, k))
                abc_map[(i,j)] = "(" + (" + ".join(st)) + ")"
                line.append("var {}[digits] binary;".format(prefix))
                empty_tiles.append((i,j))
            elif board[i][j] and board[i][j] != 'X':
                non_zeros.add(board[i][j])
                st = []
                for k in range(10):
                    st.append("X[{}, \"{}\"] * {}".format(k, board[i][j], k))
                abc_map[(i,j)] = "(" + (" + ".join(st)) + ")"

    for c in abc:
        st = []
        for k in range(10):
            st.append("X[{}, \"{}\"] * {}".format(k, c, k))
        abc_map[c] = "(" + (" + ".join(st)) + ")"
        # A = sum_{i = 0 to 9}(X[i, "A"] * i)

    line.append("")
    line.append("minimize nothing: Y;")
    line.append("")

    count = 0


    # vertical sums
    for k in v.keys():
        tiles = []
        x, y = k
        sum = v[k]
        sum = reversed(list(sum))
        sum_st = []
        for i, c in enumerate(sum):
            sum_st.append(abc_map[c] + " * {}".format(10**i))
            if i > 0:
                non_zeros.add(c)
        st = []
        x = x + 1
        while x in board and board[x][y] and not board[x][y] == 'X':
            st.append(abc_map[(x, y)])
            tiles.append((x, y))
            x = x + 1
        count = count + 1
        #line.append("subto constraint_v{}:".format(v[k]))
        line.append("subto constraint{}:".format(count))
        line.append("    {} == {};".format(" + ".join(st), " + ".join(sum_st)))
        # unique constraint
        count = count + 1
        for digit in range(1, 10):
            line.append("subto constraint_unique_v_{}_{}:".format(count, digit))
            st = []
            for row, col in tiles:
                if board[row][col] == 'O':
                    st.append("row_{}_col_{}[{}]".format(row, col, digit))
                else:
                    st.append("X[{}, \"{}\"]".format(digit, board[row][col]))
            line.append("    {} <= 1;".format(" + ".join(st)))

    # horizontal sums
    for k in h.keys():
        tiles = []
        x, y = k
        sum = h[k]
        sum = reversed(list(sum))
        sum_st = []
        for i, c in enumerate(sum):
            sum_st.append(abc_map[c] + " * {}".format(10**i))
        st = []
        y = y + 1
        while y < len(board[x]) and board[x][y] and not board[x][y] == 'X':
            st.append(abc_map[(x, y)])
            tiles.append((x, y))
            y = y + 1
        count = count + 1
        #line.append("subto constraint_h{}:".format(h[k]))
        line.append("subto constraint{}:".format(count))
        line.append("    {} == {};".format(" + ".join(st), " + ".join(sum_st)))
        # unique constraint
        count = count + 1
        for digit in range(1, 10):
            line.append("subto constraint_unique_h_{}_{}:".format(count, digit))
            st = []
            for row, col in tiles:
                if board[row][col] == 'O':
                    st.append("row_{}_col_{}[{}]".format(row, col, digit))
                else:
                    st.append("X[{}, \"{}\"]".format(digit, board[row][col]))
            line.append("    {} <= 1;".format(" + ".join(st)))

    # non zero constraints
    for c in non_zeros:
        count = count + 1
        line.append("subto constraint_{}_non_zero:".format(c))
        line.append("    X[0, \"{}\"] == 0;".format(c))

    # unique empty tile constraints, empty tiles are non-zero as well
    for i, j in empty_tiles:
        line.append("subto constraint_row_{}_col_{}_one_digit:".format(i, j))
        line.append("    sum <digit> in digits:")
        line.append("        row_{}_col_{}[digit]".format(i, j))
        line.append("    == 1;")
        count = count + 1
        line.append("subto constraint_{}_non_zero:".format(count))
        line.append("    row_{}_col_{}[0] == 0;".format(i, j))


    line.append("")
    line.append("subto constraint_one_char_per_digit:")
    line.append("    forall <digit> in digits:")
    line.append("        sum <c> in abc:")
    line.append("            X[digit, c]")
    line.append("        == 1;")

    line.append("")
    line.append("subto constraint_one_digit_per_char:")
    line.append("    forall <c> in abc:")
    line.append("        sum <digit> in digits:")
    line.append("            X[digit, c]")
    line.append("        == 1;")

    with open(model_filename, "w") as f:
        f.write("\n".join(line))


def remove_input(model_filename):
    subprocess.run(["rm", model_filename])

def get_scip_result(model_filename):
    stdout = subprocess.run(["scip", "-c", "read {} optimize display solution quit".format(model_filename)],
                            stdout = subprocess.PIPE).stdout.decode('utf-8')
    ans = {}
    for line in stdout.split("\n"):
        if line.startswith("X#"):
            line = line.split(" ")
            line = line[0].split("#")
            digit, c = line[1].split('$')
            ans[c] = digit
    return ans

def solve(board, n, v, h, model_filename = "problem424.zpl"):
    generate_input(board, n, v, h, model_filename)
    ans = get_scip_result(model_filename)
    remove_input(model_filename)
    return ans

def create_empty_board(n):
    return {i: [None]*n for i in range(n)}

sum = 0
with open("inputs/p424_kakuro200.txt") as f:
    for num, line in enumerate(f):
        line = line.strip()
        line = list(line)
        inside_parenthesis = False
        for i in range(len(line)):
            if line[i] == '(':
                inside_parenthesis = True
            elif line[i] == ')':
                inside_parenthesis = False
            elif line[i] == ',' and inside_parenthesis:
                line[i] = ';'
        line = ''.join(line).split(',')
        n = int(line[0])
        board = create_empty_board(n)
        v, h = {}, {}
        for i, st in enumerate(line[1:]):
            col = i % n
            row = i // n
            if st.startswith('('):
                st = st[1:-1]
                st = st.split(';')
                for e in st:
                    if e[0] == 'h':
                        h[(row, col)] = e[1:]
                    elif e[0] == 'v':
                        v[(row, col)] = e[1:]
            else:
                board[row][col] = st
        ans = solve(board, n, v, h)
        result = int("".join([ans[c] for c in "ABCDEFGHIJ"]))
        sum = sum + result

print(sum)
        
        



