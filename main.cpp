#include <iostream>
#include <vector>
#include "octree.h"
using namespace std;

int main() {
    // Crear un octree con límites (0, 0, 0) a (10, 10, 10)
    Octree<float> octree(0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 10.0f);

    // Insertar puntos
    octree.insert(1.0f, 1.0f, 1.0f);
    octree.insert(5.0f, 5.0f, 5.0f);
    octree.insert(8.0f, 8.0f, 8.0f);
    octree.insert(3.0f, 3.0f, 3.0f);
    octree.insert(10.0f, 10.0f, 10.0f);
    octree.insert(10.0f, 10.0f, 10.0f);

    // Buscar puntos
    cout << "Buscar (1.0f, 1.0f, 1.0f): " << octree.find(1.0f, 1.0f, 1.0f) << endl;
    cout << "Buscar (5.0f, 5.0f, 5.0f): " << octree.find(5.0f, 5.0f, 5.0f) << endl;
    cout << "Buscar (8.0f, 8.0f, 8.0f): " << octree.find(8.0f, 8.0f, 8.0f) << endl;
    cout << "Buscar (10.0f, 10.0f, 10.0f): " << octree.find(10.0f, 10.0f, 10.0f) << endl;

    // Mostrar el número total de puntos
    cout << "Total puntos después de inserciones: " << octree.countPoints() << endl;

    // Obtener coordenadas de un punto específico
    Point<float>* p = octree.getPoint(1.0f, 1.0f, 1.0f);
    if (p != nullptr) {
        cout << "Coordenadas de (1.0f, 1.0f, 1.0f): (" << p->x << ", " << p->y << ", " << p->z << ")" << endl;
    }

    // Eliminar un punto
    octree.remove(1.0f, 1.0f, 1.0f);
    cout << "Total puntos después de eliminación de (1.0f, 1.0f, 1.0f): " << octree.countPoints() << endl;

    // Buscar el punto eliminado
    cout << "Buscar (1.0f, 1.0f, 1.0f) después de eliminación: " << octree.find(1.0f, 1.0f, 1.0f) << endl;

    // Intentar insertar un punto fuera de los límites
    try {
        octree.insert(11.0f, 11.0f, 11.0f);  // Esto debe lanzar una excepción
    } catch (const exception& e) {
        cout << "Error al insertar fuera de los límites: " << e.what() << endl;
    }

    // Probar la eliminación de un punto fuera de los límites
    octree.remove(11.0f, 11.0f, 11.0f);  // No debe hacer nada, ya que está fuera de los límites

    // Insertar un punto después de la eliminación
    octree.insert(2.0f, 2.0f, 2.0f);
    cout << "Total puntos después de insertar (2.0f, 2.0f, 2.0f): " << octree.countPoints() << endl;

    // Test de un octree vacío
    Octree<float> emptyOctree(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    cout << "Buscar en un octree vacío (0.5f, 0.5f, 0.5f): " << emptyOctree.find(0.5f, 0.5f, 0.5f) << endl;
    cout << "Total puntos en un octree vacío: " << emptyOctree.countPoints() << endl;

    // Insertar puntos en un octree pequeño
    emptyOctree.insert(0.5f, 0.5f, 0.5f);
    emptyOctree.insert(0.2f, 0.2f, 0.2f);
    cout << "Total puntos después de insertar en el octree pequeño: " << emptyOctree.countPoints() << endl;

    // Eliminar un punto en el octree pequeño
    emptyOctree.remove(0.5f, 0.5f, 0.5f);
    cout << "Total puntos después de eliminar (0.5f, 0.5f, 0.5f): " << emptyOctree.countPoints() << endl;

    // Verificar la estructura después de eliminar un punto
    cout << "Buscar (0.5f, 0.5f, 0.5f) en octree pequeño después de eliminar: " << emptyOctree.find(0.5f, 0.5f, 0.5f) << endl;

    return 0;
}
