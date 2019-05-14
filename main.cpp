#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/intersections.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Intersect_2 Intersect_2;

int main() {
    
    Point_2 A,B,C,D; //Define 4 puntos
    std::cout << "Dame 8 números:";
    std::cin >> A >> B >> C >> D;  //Lee los 4 puntos
    
    Segment_2 S1(A,B), S2(C,D); //Crea los segmentos
    if(do_intersect(S1,S2)){
        auto res = intersection(S1,S2); //define la intersección, hay que hacer más cosas porque a priori no sabemos si res es vacío o un punto
        const Point_2* P = boost::get<Point_2>(&*res);
        std::cout << "El segmento definido por " << A << " y " << B << " intersecta al segmento definido por " << C << " y " << D << " en " << P << "." << std::endl;
    }
    else{
        std::cout << "El segmento definido por " << A << " y " << B << " no intersecta al segmento definido por " << C << " y " << D << "." << std::endl;
    }

    return 0;
}
