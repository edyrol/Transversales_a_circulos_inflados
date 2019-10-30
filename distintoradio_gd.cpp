#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include "transversalGrad.hpp"
#include <iostream>

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
    double best_cost = 0.0;
    Fam_Circles best;
    int tries = 0;
    Chronometer miniC;
    while(C.Peek() < timeout)
    {
        Fam_Circles F = random_family();
        GradDes_dr GD(
            F,
            [](const Fam_Circles& F) {
                return -min_inflate_sq(F);
            },
            [](Fam_Circles& F) {
                for (auto& f : F)
                {
                    if(f.r<0){
                        f.r = 0;
                    }
                }
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
        
        double best_local = GD.cost;
        while(true) {
            GD.step(0.2, 0.01);
            if(GD.cost >= best_local)
                break;
            else
                best_local = GD.cost;
        }
        ++tries;
        if(GD.cost < best_cost){
            best_cost = GD.cost;
            best = GD.F;
        }
        if(miniC.Peek() > print_timeout){
            std::cout <<"Tries: "<< tries << '\n' << "Cost:  " << std::sqrt(-best_cost) << "\n";
            miniC.Reset();
        }
    }
    std::cout << std::setprecision(15);
    std::cout << "\nCost: " << std::sqrt(-best_cost) << '\n';
    std::cout << best;
}
