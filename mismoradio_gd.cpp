#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include "transversalGrad.hpp"
#include <iostream>

int family_size = 5; // Size of the family of circles
int k = 3;           // The number k in Tk
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
    return {A.c + B.c, A.r + B.r};
}

Circle operator-(const Circle &A, const Circle &B)
{
    return {Point(0, 0) + (A.c - B.c), std::abs(A.r - B.r)};
}

Circle operator*(double a, const Circle &C) { return {a * C.c, a * C.r}; }

int main()
{
    std::cout << "Mismo radio\nGradient Descent\nk\tnum_circles\ttimeout\n";
    std::cin >> k >> family_size >> timeout;
    std::cout << k << '\t' <<  family_size << '\t' << timeout << '\n';

    Chronometer C;
    double best_cost = 0.0;
    Fam_Circles best;
    int tries = 0;
    int tries_until_best = 0;
    double time_until_best = 0.0;
    bool improved = false;
    Chronometer miniC;
    std::cout << "Time\tTries\tCost\n";
    while (C.Peek() < timeout)
    {
        Fam_Circles F = random_family();
        GradDes_mr GD(
            F,
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

        double best_local = GD.cost;
        double force = 0.2;
        while (true)
        {
            GD.step(force, 0.01);
            if(GD.cost < best_local)
                best_local = GD.cost;
            else if (force > 0.0001)
                force *=0.1;
            else
                break;
        }
        ++tries;
        if (GD.cost < best_cost)
        {
            tries_until_best = tries;
            time_until_best = C.Peek();
            best_cost = GD.cost;
            improved = true;
            best = GD.F;
        }
        if (miniC.Peek() > print_timeout)
        {
            if (improved)
            {
                std::cout << C.Peek() << '\t' << tries << '\t' << std::sqrt(-best_cost) << '\n';
                improved = false;
            }
            miniC.Reset();
        }
    }
    std::cout << std::setprecision(15);
    std::cout << "\nTries until best was found: " << tries_until_best << '\n';
    std::cout << "Time until best was found: " << time_until_best << "s\n";
    std::cout << "Cost: " << std::sqrt(-best_cost) << '\n';
    std::cout << best;
}