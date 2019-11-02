#pragma once

#include "IntegerInterval.hpp"
#include "Projectors.hpp"
#include "Random.hpp"
#include <array>
#include <iostream>
#include "transversal.hpp"

using Vector = Kernel::Vector_2;

// This class MINIMIZES! Remember that!

template <class Cost, class Projector>
class GradDes_dr
{
public:
    GradDes_dr(Fam_Circles F, Cost f, Projector P = Identity<Fam_Circles>())
        : f(f), F(F), P(P) {
            cost = f(F);
    }
    void step(double force = 0.1, double h = 0.1) {
        Fam_Circles Fnew = gradient(force,h);
        P(Fnew);
        double costnew = f(Fnew);
        if (costnew < cost){
            F=Fnew;
            cost=costnew;
        }
    }

private:
    Fam_Circles gradient(double force, double h)
    {
        Fam_Circles Fnew=F;
        for(auto i : NN(F.size())){
            F[i].c += Vector(h,0);
            double xnew = Fnew[i].c.x() + force*(cost-f(F))/h;
            F[i].c -= Vector(h,0);
            F[i].c += Vector(0,h);
            double ynew = Fnew[i].c.y() + force*(cost-f(F))/h;
            F[i].c -= Vector(0,h);
            Fnew[i].c = Point(xnew,ynew);
            F[i].r += h;
            Fnew[i].r+= force*(cost-f(F))/h;
            F[i].r -= h;
        }
        return Fnew;
    }

private:
    Cost f;
    Projector P;
public:
    Fam_Circles F;
    double cost;
};

template <class Cost, class Projector>
class GradDes_mr
{
public:
    GradDes_mr(Fam_Circles F, Cost f, Projector P = Identity<Fam_Circles>())
        : f(f), F(F), P(P) {
            cost = f(F);
    }
    void step(double force = 0.1, double h = 0.1) {
        Fam_Circles Fnew = gradient(force,h);
        P(Fnew);
        double costnew = f(Fnew);
        if (costnew < cost){
            F=Fnew;
            cost=costnew;
        }
    }

private:
    Fam_Circles gradient(double force, double h)
    {
        Fam_Circles Fnew=F;
        for(auto i : NN(F.size())){
            F[i].c += Vector(h,0);
            double xnew = Fnew[i].c.x() + force*(cost-f(F))/h;
            F[i].c -= Vector(h,0);
            F[i].c += Vector(0,h);
            double ynew = Fnew[i].c.y() + force*(cost-f(F))/h;
            F[i].c -= Vector(0,h);
            Fnew[i].c = Point(xnew,ynew);
        }
        return Fnew;
    }

private:
    Cost f;
    Projector P;
public:
    Fam_Circles F;
    double cost;
};