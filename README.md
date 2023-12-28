### Notes:

`problem369`: found a compact representation for states, which can be used to calculate the number of ways corresponding to each state hash via the multinomial theorem. Used the linear_sum_assignment() to check whether a state affect the result.

`problem399`: not something that is particularly hard, but I wrote a function converting 10^(a.aaaaa) to b.b\*10^(integer).

`problem588`: used np.convolve() for polynomial multiplication.

`problem701`: graph coloring. There are two incorrect implementation but failed differently. So, having two incorrect implementations might result in a correct implementation :D

`problem810`: used x86's clmul / arm's vmull for carry-less multiplication results in ~5x speedup.

`problem813`: used SageMath Polynomial under GF2 to do carry-less exponentiation.
