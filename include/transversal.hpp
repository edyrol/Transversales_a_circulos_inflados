#pragma once

#include <CGAL/Simple_cartesian.h>
#include <algorithm>
#include <discreture.hpp>
#include <iostream>
#include <vector>

using Kernel = CGAL::Simple_cartesian<double>;
using Point = Kernel::Point_2;
using Line = Kernel::Line_2;

//Círculo con centro c y radio r
struct Circle {
    Point c;
    double r;
};

//Familia de círculos
using Fam_Circles = std::vector<Circle>;

//Operaciones con puntos
Point operator+(const Point& A, const Point& B) {
    return Point(A.x() + B.x(), A.y() + B.y());
}

Point operator*(double a, const Point& P) {
    return Point(a*P.x(), a*P.y());
}

//Imprimir círculos
std::ostream& operator<<(std::ostream& os, const Circle& C)
{
    os << "Circle((" << C.c.x() << "," << C.c.y() << ")," << C.r << ")";
    return os;
}

//Imprimir vectores
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& C)
{
    os << '\n';
    for (auto& c : C)
        os << c << '\n';
    return os;
}

//Transversal candidata a tres círculos A,B,C  que separa a A de B,C
Line transversal_candidate_bc(const Circle& A, const Circle& B, const Circle& C) {
    return {A.c + A.r*(B.c - A.c)/(A.r + B.r),
            A.c + A.r*(C.c - A.c)/(A.r + C.r)}; // Los puntos de los
                                                    // segmentos
}

//Vector con las tres lineas candidatas
std::array<Line, 3> transversal_candidates(const Circle& A, const Circle& B, const Circle& C) {
    return {transversal_candidate_bc(A, B, C),
            transversal_candidate_bc(B, A, C),
            transversal_candidate_bc(C, A, B)};
}

//Lo mínimo que hay que inflar a F para que L sea transversal (al cuadrado)
double min_inflate_sq(const Fam_Circles& F, Line L) {
    double worst = 0;
    for (const Circle& circle : F)
    {
        auto d2 = squared_distance(circle.c, L);
        auto ratio = d2/(circle.r*circle.r);
        if (worst < ratio)
            worst = ratio;
    }
    return worst;
}

//Lo mínimo que hay que inflar a F para que tenga transversal (al cuadrado)
double min_inflate_sq(const Fam_Circles& F) {
    double best = 99999999999.0;
    for (auto&& vc : discreture::combinations(F, 3))
    {
        auto L = transversal_candidates(vc[0], vc[1], vc[2]);
        for (auto&& l : L)
        {
            double candidate = min_inflate_sq(F, l);
            if (candidate < best)
                best = candidate;
        }
    }
    return best;
}

//Lo mínimo que hay que inflar F para que tenga la propiedad T(k) (al cuadrado)
double min_inflate_TK_sq(const Fam_Circles& F, int k) {
    double worst = 0.0;
    for (auto&& kada : discreture::combinations(F, k))
    {
        Fam_Circles T(kada.begin(), kada.end());
        double lll = min_inflate_sq(T);
        if (worst < lll)
            worst = lll;
    }
    return worst;
}

//Mismo radio
//Lo que hay que reescalar para que tenga la propiedad T(k)
//Reescala igual a todos los círculos
void rescale_TK(Fam_Circles& F, int k) {
    double l = sqrt(min_inflate_TK_sq(F, k));
    for (auto& circle : F)
        circle.r *= l;
}

//Distinto radio
//Lo que hay que reescalar para que tenga la propiedad T(k)
//Reescala a cada círculo independientemente
void greedy_rescale_TK(Fam_Circles& F, int k)
{
    int N = F.size();
    std::vector<double> worst(F.size(), 0.0);
    for (auto&& kada : discreture::combinations(N, k))
    {
        Fam_Circles T = discreture::compose(F, kada);
        double lll = min_inflate_sq(T);
        for (auto i : kada)
        {
            if (worst[i] < lll)
                worst[i] = lll;
        }
    }
    for (int i = 0; i < N; ++i)
    {
        F[i].r *= sqrt(worst[i]);
    }
}

void replace_by_bigger(double& a, double b)
{
    if (a < b)
        a = b;
}
