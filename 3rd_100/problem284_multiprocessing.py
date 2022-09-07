# Question: https://projecteuler.net/problem=284

# Very slow indeed. This implementation uses Sage's solve_mod and multiprocessing.

import multiprocessing as mp
from pathlib import Path

N = 10000

base_alphabet = "0123456789" + "abcdefghijklmnopqrstuvwxyz"

def int_to_string(n, base):
    digits = []
    while n > 0:
        digits.append(base_alphabet[n % base])
        n //= base
    digits = list(reversed(digits))
    return "".join(digits)

def digit_sum(n, base):
    ans = 0
    while n > 0:
        ans += n % base
        n //= base
    return ans

def solve_per_process(solve_range):
    start, end = solve_range

    # check if the range is solved
    success_indicator_path = Path(f"results/{start}_{end}.success")
    if success_indicator_path.is_file():
        return

    # if the range is not solved then blank out the result file
    result_path = Path(f"results/{start}_{end}.txt")
    with open(result_path, "w") as f:
        pass

    print(f"Working on range {start} {end}")

    from sage.all import var, solve_mod, lift
    x = var('x')

    for n_digits in range(start, end+1):
        lower_bound = 14**(n_digits-1)
        upper_bound = 14**(n_digits)-1
        roots = solve_mod(x**2-x==0, 14**n_digits)
        result = []
        for root in roots:
            root = root[0]
            if root >= lower_bound and root <= upper_bound:
                result.append(digit_sum(lift(root), 14))
        with open(result_path, "a") as f:
            for r in result:
                f.write(f"{n_digits} {r}\n")
        with open(success_indicator_path, "w") as f:
            f.write("success")

def solve(original_start, original_end, n_processes):
    Path("./results").mkdir(exist_ok=True)
    jobs = []
    step_size = 1
    for start in range(original_start, original_end+1, step_size):
        end = min(start + step_size - 1, original_end+1)
        jobs.append((start, end))
    with mp.Pool(n_processes) as process_pool:
        process_pool.map(solve_per_process, jobs)

def calculate_result():
    ans = 0
    results_dir = Path("results")
    for filepath in results_dir.glob("*.success"):
        result_filepath = filepath.with_suffix(".txt")
        with open(result_filepath) as f:
            for line in f.readlines():
                line = line.strip().split()
                ans += int(line[1], 10)
    return ans

if __name__ == "__main__":
    solve(1, N, mp.cpu_count())
    ans = calculate_result()
    print(int_to_string(ans, 14))
