# Question: https://projecteuler.net/problem=12

# triangle number can be written as: T = N(N+1)/2
# N and N+1 have disjoint divisior sets except 1
# so if we have a function that count the number of divisors, we can count the number of divisors of N and (N+1) easily
# then we can determine the number of divisors of T easily (F(N) + F(N+1) - 1 if (either N or N+1 is divisible by 4), F(N) + F(N+1) - 2 otherwise)

# but if we already have the magic function counting the number of divisors, we can apply that function to T directly


sum = 0
i = 0
while True:
    i = i + 1
    sum = sum + i
    if len(divisors(sum)) >= 500:
         break
print(sum)
