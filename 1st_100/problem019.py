# Question: https://projecteuler.net/problem=19

import numpy as np

day = 1 + 366 # Jan 1, 1901
# day mod 7 == 1 -> Sunday

leap_year = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
norm_year = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
assert(sum(leap_year) == 366)
assert(sum(norm_year) == 365)

num_sundays_first_day = np.zeros((2, 7), dtype = np.uint16)

# i = 0 -> leap year, i = 1 -> norm year
# j = 1 -> Sunday, ..., j = 6 -> Friday, j = 0 -> Saturday

for Jan1day in range(7):
    for i, year in enumerate([leap_year, norm_year]):
        day = Jan1day
        for month in year:
            if day % 7 == 1:
                num_sundays_first_day[i][Jan1day] = num_sundays_first_day[i][Jan1day] + 1
            day = day + month

day = 1 + 366 # Jan 1, 1901

result = 0

for year in range(1901, 2000 + 1):
    is_leap_year = (year % 4 == 0) and not (year % 400 == 0)
    if is_leap_year:
        i = 0
    else:
        i = 1
    result = result + num_sundays_first_day[i][day % 7]
    if is_leap_year:
        day = day + 366
    else:
        day = day + 365

print(result)
