#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include <iostream>

int pop_size = 50;
int family_size = 5;
int k = 3;
double timeout = 600;
double print_timeout = 2;

Random R;

Fam_Circles random_family()
{
    Fam_Circles F(family_size);
    std::generate(F.begin(), F.end(), []() {
        return Circle{{R.random_real(-10, 10), R.random_real(-10, 10)}, 1}; // Mismo radio
    });
    rescale_TK(F, k); // Mismo radio
    return F;
}

Circle operator+(const Circle &A, const Circle &B)
{
    return {A.c + B.c, 1.0};
}

Circle operator-(const Circle &A, const Circle &B)
{
    return {Point(0, 0) + (A.c - B.c), 1.0};
}

Circle operator*(double a, const Circle &C) { return {a * C.c, 1.0}; }

int main()
{
    std::cout << "Mismo radio\nDifferential Evolution\nk\tnum_circles\ttimeout\n";
    std::cin >> k >> family_size >> timeout;
    std::cout << k << '\t' <<  family_size << '\t' << timeout << '\n';

    Chronometer C;
    std::vector<Fam_Circles> Population(pop_size);
    std::generate(Population.begin(), Population.end(), []() {
        return random_family();
    });

    DifferentialEvolver D(
        Population,
        [](const Fam_Circles &F) {
            return -min_inflate_sq(F);
        },
        [](Fam_Circles &F) {
            double max_number = 0;
            for (auto &f : F)
            {
                replace_by_bigger(max_number, std::abs(f.c.x()));
                replace_by_bigger(max_number, std::abs(f.c.y()));
            }
            for (auto &f : F)
            {
                f = (10.0 / max_number) * f;
                f.r = 1.0; // Mismo radio
            }
            rescale_TK(F, k); // Mismo radio
        });

    int steps = 0;
    int steps_until_best = 0;
    double time_until_best = 0.0;
    double best_cost = 0.0;
    bool improved = false;
    Chronometer miniC;
    std::cout << "Time\tSteps\tCost\n";
    while (C.Peek() < timeout)
    {
        D.step(0.5, 0.5);
        ++steps;
        if (D.best_cost < best_cost)
        {
            best_cost = D.best_cost;
            improved = true;
            steps_until_best = steps;
            time_until_best = C.Peek();
        }
        if (miniC.Peek() > print_timeout)
        {
            if (improved)
            {
                std::cout << C.Peek() << '\t' << steps << '\t' << std::sqrt(-D.best_cost) << "\n";
                improved = false;
            }
            miniC.Reset();
        }
    }
    std::cout << std::setprecision(15);
    std::cout << "\nSteps until best was found: " << steps_until_best << '\n';
    std::cout << "Time until best was found: " << time_until_best << "s\n";
    std::cout << "Cost: " << std::sqrt(-D.best_cost) << '\n';
    std::cout << D.best;
    return 0;
}
