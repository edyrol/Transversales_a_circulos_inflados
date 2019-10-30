#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include <iostream>

int pop_size = 60; // Population size
int family_size = 5; // Size of the family of circles
int k = 3; // The number k in Tk
double timeout = 600;
double print_timeout = 10;

Random R;

Fam_Circles random_family() {
    Fam_Circles F(family_size);
    std::generate(F.begin(), F.end(), []() {
        return Circle{{R.random_real(-10, 10), R.random_real(-10, 10)}, R.random_real(0, 10)}; // Distinto radio
    });
    greedy_rescale_TK(F, k); // Distinto radio
    return F;
}

Circle operator+(const Circle& A, const Circle& B)
{
    return {A.c + B.c, A.r + B.r};
}

Circle operator-(const Circle& A, const Circle& B)
{
    return {Point(0,0) + (A.c - B.c), std::abs(A.r-B.r)};
}

Circle operator*(double a, const Circle& C) { return {a*C.c, a*C.r}; }

int main()
{
    std::cout << "Distinto radio\nk, num_circles, timeout\n";
    std::cin >> k >> family_size >> timeout;

    Chronometer C;
    std::vector<Fam_Circles> Population(pop_size);
    std::generate(Population.begin(), Population.end(), []() {
        return random_family();
    });

    DifferentialEvolver D(
        Population,
        [](const Fam_Circles& F) {
            return -min_inflate_sq(F);
        },
        [](Fam_Circles& F) {
            greedy_rescale_TK(F, k); // Distinto radio
            double max_number = 0;
            for (auto& f : F)
            {
                replace_by_bigger(max_number, std::abs(f.c.x()));
                replace_by_bigger(max_number, std::abs(f.c.y()));
                replace_by_bigger(max_number, std::abs(f.r)); // Distinto radio
            }
            for (auto& f : F)
            {
                f = (10.0/max_number)*f;
            }
        }
    );

    int steps = 0;
    Chronometer miniC;
    while(C.Peek() < timeout)
    {
        D.step(0.5, 0.5);
        ++steps;
        if(miniC.Peek() > print_timeout){
            std::cout <<"Steps: "<< steps << '\n' << "Cost:  " << std::sqrt(-D.best_cost) << "\n";
            miniC.Reset();
        }
    }
    std::cout << std::setprecision(15);
    std::cout << "\nCost: " << std::sqrt(-D.best_cost) << '\n';
    std::cout << D.best;
    return 0;
}
