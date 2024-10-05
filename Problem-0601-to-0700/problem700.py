# Question: https://projecteuler.net/problem=700

#                coin            n
#  1 1504170715041707            1 
#  2    8912517754604            3
#  3    2044785486369          506
#  4    1311409677241         2527
#  5     578033868113         4548
#  6     422691927098        11117
#  7     267349986083        17686
#  8     112008045068        24255
#  9      68674149121        55079
# 10      25340253174        85903
# 11       7346610401       202630
# 12       4046188430       724617
# 13        745766459      1246604
# 14        428410324      6755007
# 15        111054189     12263410
# 16         15806432     42298633
#             ^^^^^^^
#          intermediate values while finding GCD
#          why? idk...

factor = 1504170715041707
MOD = 4503599627370517
def gcd(a, b):
    ans = a # first coin
    while not a == b:
        if a > b:
            a = a - b
            ans = ans + a
            #print(a)
        else:
            b = b - a
        #print(a,b)
    return ans
print(gcd(factor, MOD))