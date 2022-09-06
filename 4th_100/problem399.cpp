// Question: https://projecteuler.net/problem=399
// Wall's conjecture: if fib[x] is the first Fibonacci number that is divisible by p, then x * p is the divisibility  period of p^2.

#include<iostream>
#include<iomanip>
#include<array>
#include<vector>
#include<cmath>
#include<utility>
#include<cassert>

typedef __uint128_t ui;
#define endl "\n"

using namespace std;

constexpr ui ui_pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * ui_pow(base, exp-1);
};

const ui N = pow(10, 8);
const ui N_MAX = 2*N;

vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(n));
    for (ui p = 3; p <= sqrt_n; p+=2)
    {
        if (!is_prime[(p-3)/2])
            continue;
        for (ui np = 3*p; np <= n; np += 2*p)
            is_prime[(np-3)/2] = false;
    }
    primes.push_back(2);
    ui size = is_prime.size();
    for (ui p = 0; 2*p+3 <= n; p++)
        if (is_prime[p])
            primes.push_back(2*p+3);
    return primes;
}

ui find_mod_period(ui mod, ui upperbound)
{
    ui a = 0;
    ui b = 1;
    ui idx = 0;

    do 
    {
        ui old_b = b;
        b = (a + b) % mod;
        a = old_b;
        idx++;
    } while (a != 0 && idx < upperbound);

    return idx;
}

ui find_squarefree_fib_position(ui pos)
{
    auto primes = get_primes(pos);
    vector<bool> is_squarefree = vector<bool>(N_MAX+1, true);
    ui c = 0;
    for (auto prime: primes)
    {
        c++;
        if (c % 100000 == 0)
            cout << (uint64_t)c << endl;
        ui period = find_mod_period(prime, 2 * pos / prime) * prime;
        if ((!is_squarefree[period]) && (!is_squarefree[period*2]))
            continue;
        for (ui i = 0; i <= N_MAX; i += period)
            is_squarefree[i] = false;
//        cout << prime << " " << period << endl;
    }
    ui count = 0;
    ui idx = 0;
    while (count < pos) // the problem statement considers 1 as the first Fibonacci number
    {
        idx += 1;
        if (is_squarefree[idx])
            count += 1;
    }
    return idx;
}

class Matrix_2_2 // 2x2 matrix
{
    public:
        vector<vector<ui>> data;
        Matrix_2_2()
        {
            this->data = vector<vector<ui>>(2, vector<ui>(2, 0));
        }
        Matrix_2_2(const Matrix_2_2& other)
        {
            this->data = other.data;
        }
        void matrix_square()
        {
            Matrix_2_2 ans = Matrix_2_2();
            ans.data[0][0] = this->data[0][0] * this->data[0][0] + this->data[0][1] * this->data[1][0];
            ans.data[0][1] = this->data[0][0] * this->data[0][1] + this->data[0][1] * this->data[1][1];
            ans.data[1][0] = this->data[1][0] * this->data[0][0] + this->data[1][1] * this->data[1][0];
            ans.data[1][1] = this->data[1][0] * this->data[0][1] + this->data[1][1] * this->data[1][1];
            this->data = ans.data;
        }
        void mod(const ui& m)
        {
            this->data[0][0] %= m;
            this->data[0][1] %= m;
            this->data[1][0] %= m;
            this->data[1][1] %= m;
        }
        void multiply(Matrix_2_2 rhs)
        {
            Matrix_2_2 ans = Matrix_2_2();
            ans.data[0][0] = this->data[0][0] * rhs.data[0][0] + this->data[0][1] * rhs.data[1][0];
            ans.data[0][1] = this->data[0][0] * rhs.data[0][1] + this->data[0][1] * rhs.data[1][1];
            ans.data[1][0] = this->data[1][0] * rhs.data[0][0] + this->data[1][1] * rhs.data[1][0];
            ans.data[1][1] = this->data[1][0] * rhs.data[0][1] + this->data[1][1] * rhs.data[1][1];
            this->data = ans.data;
        }
        void pow_mod(ui exp, const ui& m)
        {
            Matrix_2_2 base = Matrix_2_2();
            base.data = this->data;
            Matrix_2_2 ans = Matrix_2_2();
            ans.data[0][0] = 1;
            ans.data[1][1] = 1;

            while (exp > 0)
            {
                if (exp % 2 == 1)
                {
                    ans.multiply(base);
                    ans.mod(m);
                }
                base.multiply(base);
                base.mod(m);
                exp /= 2;
            }

            this->data = ans.data;
        }
};

ui fib_mod(ui pos, ui mod)
{
    Matrix_2_2 T = Matrix_2_2();
    T.data[0][0] = 1;
    T.data[0][1] = 1;
    T.data[1][0] = 1;

    T.pow_mod(pos-1, mod);
    return (T.data[0][0]) % mod;
}

pair<double, uint64_t> fib_exp(ui n)
{
    // Calculating fib(n) = 1/5 * (((1+sqrt(5))/2)^n - ((1-sqrt(5))/2)^n)
    // if n is even, then fib(n) = 1/5 * (((1+sqrt(5))/2)^n - ((sqrt(5)-1)/2)^n)
    // if n is odd, then fib(n) = 1/5 * (((1+sqrt(5))/2)^n + ((sqrt(5)-1)/2)^n)
    // This function only solves the case when n is even
    assert(n % 2 == 0);

    // Let phi_a = n * log((1+sqrt(5))/2) / log(10)
    // Let phi_b = n * log((sqrt(5)-1)/2) / log(10)
    // Then fib(n) = 1/5 * (10**phi_a - 10**phi_b)
    // Note that, for a large n, we have that (phi_a) ~ -(phi_b)
    // So, we can manipulate (10**phi_a - 10**phi_b)
    //                     = (10**(-phi_a)) (10**(2*phi_a) - 10**(phi_a+phi_b))
    //                     ~ (10**(-phi_a)) (10**(2*phi_a) -            1       ) (since phi_a + phi_b ~ 0)
    //                     ~ (10**(-phi_a)) (10**(2*phi_a)) - (10**(-phi_a))
    //                     ~ 10**phi_a                      - 0
    //                     ~ 10**phi_a
    // So, fib(n) ~ 10 ** (log(1/5)/log(10)) * (10**phi_a)
    //            ~ 10 ** (log(1/5)/log(10) + phi_a)
    // Let alpha = (log(1/5)/log(10) + phi_a)
    // Then fib(n) ~ 10**alpha
    double phi_a = n * log((sqrt(5)+1.0)/2.0) / log(10.0);
    double alpha = log(1.0/sqrt(5)) / log(10.0) + phi_a;

    // Note that, we currently have fib(n) as 10**alpha.
    // However, we want it to be in the form of
    //      significand * (10 ** exponent)
    // where exponent is an integer and significand is a floating point number.
    // We can achieve this by, we have,
    //   fib(n) ~ 10**alpha
    //          ~ 10**floor(alpha) * 10**(alpha - floor(alpha))
    // and we can let significand = 10**(alpha - floor(alpha)) // note that, even though alpha is large, we have 0 < alpha - floor(alpha) < 1
    //            and exponent = floor(alpha)
    //cout << alpha - int(alpha) << endl;
    double significand = pow(10, alpha - int(alpha));
    uint64_t exponent = int(alpha);
    return make_pair(significand, exponent);
}

int main()
{
    ui fib_pos = find_squarefree_fib_position(N);
    ui ans_mod_16 = fib_mod(fib_pos, ui_pow(10, 16));
    auto ans_exp = fib_exp(fib_pos);
    double ans_exp_significand = ans_exp.first;
    uint64_t ans_exp_exponent = ans_exp.second;
    cout << (uint64_t)ans_mod_16 << "," <<fixed<<setprecision(1)<< ans_exp_significand << "e" << ans_exp_exponent << endl;
    return 0;
}

