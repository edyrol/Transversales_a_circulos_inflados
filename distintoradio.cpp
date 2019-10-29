#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include <iostream>

int pop_size = 60; // Population size
int family_size = 5; // Size of the family of circles
int k = 3; // The number k in Tk
int num_epochs = 900000; // Number of generations
Random R;

Fam_Circles random_family() {
    Fam_Circles F(family_size);
    std::generate(F.begin(), F.end(), []() {
        return Circle{{R.random_real(-10, 10), R.random_real(-10, 10)}, R.random_real(0, 5)}; // Distinto radio
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

void replace_by_bigger(double& a, double b)
{
    if (a < b)
        a = b;
}

int main()
{
    std::cout << "Different radii\nk, num_circles, pop_size, num_epochs\n";
    std::cin >> k >> family_size >> pop_size >> num_epochs;

    Chronometer C;
    std::vector<Fam_Circles> Population(pop_size);
    std::generate(Population.begin(), Population.end(), []() {
        return random_family();
    });

    DifferentialEvolver D(Population, [](const Fam_Circles& F) {
        return -min_inflate_sq(F);
    });

    double best = D.best_cost;

    for (int epoch : NN(num_epochs))
    {
        D.step(0.5, 0.5, [](Fam_Circles& F) {
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
                f = (5.0/max_number)*f;
            }
        });

        /*if (D.best_cost < best)
        {
            best = D.best_cost;
            std::cout << std::setprecision(10);
            std::cout << epoch << "  cost: " << std::sqrt(-best) << '\n';
            std::cout << D.best << "\n\n";
        }*/
        /*if (epoch%10000 == 10000 - 1)
        {
            std::cout << std::setprecision(10);
            std::cout << "Epoch " << epoch + 1 << "  cost: " <<
        std::sqrt(-D.best_cost) << '\n'; std::cout << D.best << "\n\n";
        }*/
    }
    std::cout << std::setprecision(15);
    std::cout << "Cost: " << std::sqrt(-D.best_cost) << '\n';
    std::cout << D.best << "\n";
    std::cout << "Total time taken: " << C.Peek() << "s\n";

    return 0;
}
