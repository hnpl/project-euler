// Problem: https://projecteuler.net/problem=307
// g++ problem307.cpp -lgmp -I$HOME/gmp/include -L$HOME/gmp/lib -pthread

/*
    . Note that the defects are not the same,
        This means, a chip with (defect 1 and defect 2) is different from that chip with (defect 2 and defect 3),
    
    . Let N be the number of chips, M be the number of defects.
    . Let x be the number of chips having 2 defects.
      This means there are M - 2x chips having 1 defect,
             and there are N - M + x chips with 0 defects.
    
    . The number of ways of choosing x chips from N chips:
        C(N, x)
    . The number of ways of choosing 2x defects from M defects:
        C(M, 2x)
    . The number of ways of assigning the above 2x defects to the above x chips where each chip has exactly 2 defects:
        (2x)! / (2^x)
        Why?
            Suppose there are 2x numbers and a grid of size 2 rows and x columns.
            There are (2x)! ways to assign 2x numbers to 2x squares.
            Let each column represent a chip, then a chip with (defect 1 and defect 2) is the same as that chip with (defect 2 and defect 1).
            So, for each chip, we are double counting the number of assignments.
            So, for each chip, we divide the number of assignments by 2.
            As a result, the number of ways of assigning 2x defects to x chips where each chip has exactly 2 defects is (2x)! / 2^x.
    . So, now we have (N-x) chips to assign (M-2x) defects.
      Note that, now, each chip has at most 1 defects.
      So, it is the same as choosing (M-2x) chips and putting 1 defect on each.
    . The number of ways of choosing (M-2x) chips from (N-x) chips:
        C(N-x, M-2x)
    . The number of ways of assigning (M-2x) defects to (M-2x) chips:
        (M-2x)!

    . So, the number of ways of having x chips with 2 defects each is:
        C(N, x) * C(M, 2x) * (2x)! / (2^x) * C(N-x, M-2x) / (M-2x)!

    . The number of ways of assigning M defects to N chips:
        N**M (there are N options to assign to each defect, so for M defects, there are N**M possible assignments of M defects to N chips)

    . The probability of having a chip with 3 defects equals to ONE minus the probability of having no chips with more than 2 defects.
      So,
        the_answer = 1.0 - sum(x | 0 <= x <= M/2)[C(N, x) * C(M, 2x) * (2x)! / (2^x) * C(N-x, M-2x) / (M-2x)! / N**M]

*/

#include<gmp.h>

#include<iostream>
#include<vector>
#include<thread>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 1000000;
const ui M = 20000;

const ui PRECISION = 15;
const ui N_THREADS = 8;


struct ThreadResult
{
    mpf_t MPF_ans;
};

void worker(ui range_begin, ui range_end, ThreadResult& result)
{
    mpz_t MPZ_N;
    mpz_init(MPZ_N);
    mpz_set_ui(MPZ_N, N);

    mpz_t MPZ_M;
    mpz_init(MPZ_M);
    mpz_set_ui(MPZ_M, M);

    mpf_t MPF_ans;
    mpf_init(MPF_ans); // set ans to zero

    mpz_t MPZ_n_chips_2_defects;
    mpz_init(MPZ_n_chips_2_defects);

    mpz_t MPZ_N_power_M;
    mpz_init(MPZ_N_power_M);
    mpz_set(MPZ_N_power_M, MPZ_N); // set N_power_M to N
    mpz_pow_ui(MPZ_N_power_M, MPZ_N_power_M, M); // N_power_M = N ** M

    mpf_t MPF_N_power_M;
    mpf_init(MPF_N_power_M);
    mpf_set_z(MPF_N_power_M, MPZ_N_power_M);

    mpf_t MPF_Probability;
    mpf_init(MPF_Probability);

    mpz_t MPZ_n_ways; // number of ways of having n_chips_2_defects chips with 2 defects
                      // also means we have M - n_chips_2_defects chips with 1 defect
                      // and N - M - n_chips_2_defects chips with 0 defects
    mpz_init(MPZ_n_ways);

    mpf_t MPF_2_power;
    mpf_init(MPF_2_power);
    mpf_set_ui(MPF_2_power, 1);

    mpz_t MPZ_Temp;
    mpz_init(MPZ_Temp);


    for (ui n_chips_2_defects = range_begin; n_chips_2_defects <= range_end; n_chips_2_defects++)
    {
        mpz_set_ui(MPZ_n_chips_2_defects, n_chips_2_defects);

        mpz_set_ui(MPZ_n_ways, 1);

        mpz_bin_uiui(MPZ_Temp, N, n_chips_2_defects);
        mpz_mul(MPZ_n_ways, MPZ_n_ways, MPZ_Temp);

        mpz_bin_uiui(MPZ_Temp, M, 2 * n_chips_2_defects);
        mpz_mul(MPZ_n_ways, MPZ_n_ways, MPZ_Temp);

        mpz_bin_uiui(MPZ_Temp, N-n_chips_2_defects, M-2*n_chips_2_defects);
        mpz_mul(MPZ_n_ways, MPZ_n_ways, MPZ_Temp);        
        
        mpz_fac_ui(MPZ_Temp, 2*n_chips_2_defects);
        mpz_mul(MPZ_n_ways, MPZ_n_ways, MPZ_Temp);

        mpz_fac_ui(MPZ_Temp, M - 2*n_chips_2_defects);
        mpz_mul(MPZ_n_ways, MPZ_n_ways, MPZ_Temp);

        mpf_set_z(MPF_Probability, MPZ_n_ways);
        mpf_div(MPF_Probability, MPF_Probability, MPF_2_power);
        mpf_div(MPF_Probability, MPF_Probability, MPF_N_power_M);

        mpf_add(MPF_ans, MPF_ans, MPF_Probability);
        mpf_mul_ui(MPF_2_power, MPF_2_power, 2);
    }

    mpf_set(result.MPF_ans, MPF_ans);

    // clean up
    mpz_clear(MPZ_Temp);

    mpf_clear(MPF_2_power);
    mpz_clear(MPZ_n_ways);
    mpf_clear(MPF_Probability);

    mpz_clear(MPZ_n_chips_2_defects);

    mpf_clear(MPF_N_power_M);
    mpz_clear(MPZ_N_power_M);

    mpf_clear(MPF_ans);
    mpz_clear(MPZ_M);
    mpz_clear(MPZ_N);
}


int main()
{
    mpf_set_default_prec(PRECISION); // set the precision of floating point numbers

    mpf_t MPF_ans;
    mpf_init(MPF_ans);
    mpf_set_ui(MPF_ans, 0);

    vector<thread> threads;
    vector<ThreadResult> thread_results = vector<ThreadResult>(N_THREADS);
    for (auto &thread_result: thread_results)
        mpf_init(thread_result.MPF_ans);

    ui interval_length = (M / 2) / N_THREADS;

    for (ui i = 0; i < N_THREADS; i++)
    {
        ui range_begin = i * interval_length;
        ui range_end = min((i+1)*interval_length - 1, M/2);
        thread new_thread(worker, range_begin, range_end, ref(thread_results[i]));
        threads.push_back(move(new_thread));
    }

    for (ui i = 0; i < N_THREADS; i++)
        threads[i].join();

    mpf_set_ui(MPF_ans, 1);
    for (ui i = 0; i < N_THREADS; i++)
        mpf_sub(MPF_ans, MPF_ans, thread_results[i].MPF_ans);

    for (auto &thread_result: thread_results)
        mpf_clear(thread_result.MPF_ans);

    char output[PRECISION+2];
    mp_exp_t expptr;

    for (ui i = 0; i < PRECISION + 2; i++)
        output[i] = 0;

    mpf_get_str(&output[0], &expptr, 10, PRECISION, MPF_ans);
    cout << "0." << output << "e"<< expptr << endl;

    mpf_clear(MPF_ans);

    return 0;
}

