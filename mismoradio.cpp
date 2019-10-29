#include "DifferentialEvolution.hpp"
#include "TimeHelpers.hpp"
#include "transversal.hpp"
#include <iostream>

int pop_size = 50;
int family_size = 7;
int k = 3;
int num_epochs = 900000;
Random R;

//Crea familia aleatoria
Fam_Circles random_family() {
    Fam_Circles F(family_size);
    std::generate(F.begin(), F.end(), []() {
        return Circle{{R.random_real(-10, 10), R.random_real(-10, 10)}, 1}; // Mismo radio
    });
    rescale_TK(F, k); // Mismo radio
    return F;
}

Circle operator+(const Circle& A, const Circle& B)
{ 
    return {A.c + B.c, 1.0}; 
}

Circle operator-(const Circle& A, const Circle& B)
{
    return {Point(0,0) + (A.c - B.c), 1.0};
}

Circle operator*(double a, const Circle& C) { return {a*C.c, 1.0}; }

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
            double max_number = 0;
            for (auto& f : F)
            {
                replace_by_bigger(max_number, std::abs(f.c.x()));
                replace_by_bigger(max_number, std::abs(f.c.y()));
            }

            for (auto& f : F)
            {
                f = (10.0/max_number)*f;
                f.r = 1.0; // Mismo radio
            }
            rescale_TK(F, k); // Mismo radio
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

    // int a;
    // std::cin >> a;
    return 0;
}
