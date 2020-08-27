# Transversales a Círculos Inflados

Sean *k>=3* y *F={D1, D2, ..., Dn}* una familia de discos en el plano. Supongamos que para cualquier subfamilia de *F* de tamaño *k* existe una línea que intersecta a todos sus elementos. Un problema de geometría discreta busca el menor número *λ* tal que existe una línea transversal a la familia *λF={λvD1, λD2, ..., λDn}*.

El código de este repositorio busca encontrar, para cada *k*, familias *F* para las cuáles *λ* es lo más grande posible. Para hacerlo se utiliza el algoritmo de optimización Differential Evolution.

Para ejecutar el código se requiere C++17, Boost y CGAL.
