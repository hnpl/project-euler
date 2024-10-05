# Problem: https://projecteuler.net/problem=151

"""
    State -> number of sheets of each of {A1, A2, A3, A4, A5}
    The number of distinct ways to reach the final state is small
        -> we can iterate through each way
            -> calculate the probability of that state and count the number of times there is only one sheet
        -> using the linearity of expectation, the expectation of number of times one sheet is encountered is: 
            sum_{w: ways} (probability of w * number_of_one_sheet_encounters)
"""

from decimal import *

getcontext().prec = 8

class Sheet:
    A1 = 0
    A2 = 1
    A3 = 2
    A4 = 3
    A5 = 4
    n_types = 5

def DFS(batch_idx, envelope, curr_probability, n_one_sheet_encounters):
    if (batch_idx == 16):
        return curr_probability * n_one_sheet_encounters

    total_expectation = Decimal(0)
    next_batch_idx = batch_idx + 1
    n_sheets_in_envelope = sum([v for v in envelope.values()])
    if n_sheets_in_envelope == 1:
        n_one_sheet_encounters = n_one_sheet_encounters + 1
    # choose the next sheet to pick
    for curr_sheet_type, n_sheets in envelope.items():
        if n_sheets == 0:
            continue
        new_envelope = dict(envelope)
        new_envelope[curr_sheet_type] = new_envelope[curr_sheet_type] - 1
        for new_sheet_type in range(curr_sheet_type + 1, Sheet.n_types):
            new_envelope[new_sheet_type] += 1
        next_probability = curr_probability * n_sheets / n_sheets_in_envelope
        total_expectation = total_expectation + DFS(batch_idx = next_batch_idx,
                                                    envelope = new_envelope,
                                                    curr_probability = next_probability,
                                                    n_one_sheet_encounters = n_one_sheet_encounters)

    return total_expectation

if __name__ == "__main__":
    first_envelope = { Sheet.A2: 1, Sheet.A3: 1, Sheet.A4: 1, Sheet.A5: 1 }

    ans = DFS(batch_idx = 2, 
              envelope = first_envelope,
              curr_probability = Decimal(1.0),
              n_one_sheet_encounters = 0)

    print("{:.6f}".format(ans))
