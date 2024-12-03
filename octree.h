#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBack 4
#define TopRightBack 5
#define BottomRightBack 6
#define BottomLeftBack 7

template <typename T>
struct Point {
    T x;
    T y;
    T z;

    Point() : x(-1), y(-1), z(-1) {}

    Point(T a, T b, T c) : x(a), y(b), z(c) {}
};

template <typename T>
class Octree {
    Point<T>* point;

    Point<T>* topLeftFront, *bottomRightBack;
    vector<Octree*> children;
    int totalPoints = 0;

public:
    Octree() {
        point = new Point<T>();
        cout << "[LOG] Octree created at root level." << endl;
    }

    Octree(T x, T y, T z) {
        point = new Point<T>(x, y, z);
        cout << "[LOG] Octree created with point (" << x << ", " << y << ", " << z << ")." << endl;
    }

    int getOctant(T x, T y, T z, T midx, T midy, T midz) {
        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    return TopLeftFront;
                else
                    return TopLeftBack;
            } else {
                if (z <= midz)
                    return BottomLeftFront;
                else
                    return BottomLeftBack;
            }
        } else {
            if (y <= midy) {
                if (z <= midz)
                    return TopRightFront;
                else
                    return TopRightBack;
            } else {
                if (z <= midz)
                    return BottomRightFront;
                else
                    return BottomRightBack;
            }
        }
    }

    Octree(T x1, T y1, T z1, T x2, T y2, T z2) {
        if (x2 < x1 || y2 < y1 || z2 < z1) {
            throw invalid_argument("Invalid dimensions");
        }

        point = nullptr;

        topLeftFront = new Point<T>(x1, y1, z1);
        bottomRightBack = new Point<T>(x2, y2, z2);

        children.assign(8, nullptr);
        for (int i = TopLeftFront; i <= BottomLeftBack; ++i) {
            children[i] = new Octree<T>();
        }

        cout << "[LOG] Octree created with boundaries: ("
             << x1 << ", " << y1 << ", " << z1 << ") to ("
             << x2 << ", " << y2 << ", " << z2 << ")." << endl;
    }

    void insert(T x, T y, T z) {
        cout << "[LOG] Attempting to insert point (" << x << ", " << y << ", " << z << ")." << endl;

        if (find(x, y, z)) {
            throw invalid_argument("Point already exists");
        }

        // Verificación del límite
        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z) {
            cerr << "[ERROR] Point out of bounds: (" << x << ", " << y << ", " << z << ")" << endl;
            throw invalid_argument("Point is out of bounds");
        }

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);
        totalPoints++;

        cout << "[LOG] Inserting point in octant " << index << "." << endl;

        // Subdivisión recursiva en el octante correspondiente
        if (children[index]->point == nullptr) {
            children[index]->insert(x, y, z);
        }
        else if (children[index]->point->x == -1) {
            delete children[index];
            children[index] = new Octree<T>(x, y, z);
            cout << "[LOG] Replaced empty child node at index " << index << " with point (" << x << ", " << y << ", " << z << ")." << endl;
        } else {
            // Manejo del punto que ya existe en el octante
            T x_ = children[index]->point->x, y_ = children[index]->point->y, z_ = children[index]->point->z;
            delete children[index];
            children[index] = nullptr;

            cout << "[LOG] Found existing point at (" << x_ << ", " << y_ << ", " << z_ << ") in octant " << index << ", subdividing." << endl;

            // Crear nuevos octantes si es necesario
            if (index == TopLeftFront) {
                children[index] = new Octree<T>(topLeftFront->x, topLeftFront->y, topLeftFront->z, midx, midy, midz);
            } else if (index == TopRightFront) {
                children[index] = new Octree<T>(midx, topLeftFront->y, topLeftFront->z, bottomRightBack->x, midy, midz);
            } else if (index == BottomRightFront) {
                children[index] = new Octree<T>(midx , midy , topLeftFront->z, bottomRightBack->x, bottomRightBack->y, midz);
            } else if (index == BottomLeftFront) {
                children[index] = new Octree<T>(topLeftFront->x, midy , topLeftFront->z, midx, bottomRightBack->y, midz);
            } else if (index == TopLeftBack) {
                children[index] = new Octree<T>(topLeftFront->x, topLeftFront->y, midz , midx, midy, bottomRightBack->z);
            } else if (index == TopRightBack) {
                children[index] = new Octree<T>(midx , topLeftFront->y, midz , bottomRightBack->x, midy, bottomRightBack->z);
            } else if (index == BottomRightBack) {
                children[index] = new Octree<T>(midx , midy, midz  , bottomRightBack->x, bottomRightBack->y, bottomRightBack->z);
            } else if (index == BottomLeftBack) {
                children[index] = new Octree<T>(topLeftFront->x, midy , midz , midx, bottomRightBack->y, bottomRightBack->z);
            }
            children[index]->insert(x_, y_, z_);
            children[index]->insert(x, y, z);
        }

        cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") inserted successfully." << endl;
    }

    bool find(T x, T y, T z) {
        cout << "[LOG] Searching for point (" << x << ", " << y << ", " << z << ")." << endl;

        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z) {
            cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") is out of bounds." << endl;
            return false;
        }

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);

        if (children[index]->point == nullptr) {
            return children[index]->find(x, y, z);
        } else if (children[index]->point->x == -1) {
            return false;
        } else {
            if (x == children[index]->point->x &&
                y == children[index]->point->y &&
                z == children[index]->point->z) {
                cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") found." << endl;
                return true;
            }
        }
        cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") not found." << endl;
        return false;
    }

    Point<T>* getPoint(T x, T y, T z) {
        cout << "[LOG] Retrieving point (" << x << ", " << y << ", " << z << ")." << endl;

        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z) {
            cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") is out of bounds." << endl;
            return nullptr;
        }

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);

        if (children[index]->point != nullptr) {
            if (children[index]->point->x == x &&
                children[index]->point->y == y &&
                children[index]->point->z == z) {
                cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") retrieved." << endl;
                return children[index]->point;
            }
        }

        if (children[index]->children[0] != nullptr) {
            return children[index]->getPoint(x, y, z);
        }

        cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") not found." << endl;
        return nullptr;
    }

    int countPoints() {
        cout << "[LOG] Counting total points: " << totalPoints << endl;
        return totalPoints;
    }

    void remove(T x, T y, T z) {
        cout << "[LOG] Attempting to remove point (" << x << ", " << y << ", " << z << ")." << endl;

        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z) {
            cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") is out of bounds." << endl;
            return;
        }

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);
        totalPoints--;

        cout << "[LOG] Point (" << x << ", " << y << ", " << z << ") removed. Total points: " << totalPoints << endl;

        if (children[index] != nullptr) {
            if (children[index]->point != nullptr &&
                children[index]->point->x == x &&
                children[index]->point->y == y &&
                children[index]->point->z == z) {
                children[index]->point = nullptr;
                return;
            }

            children[index]->remove(x, y, z);

            if (children[index]->isLeaf()) {
                children[index] = nullptr;
                cout << "[LOG] Child at index " << index << " is now a leaf and has been removed." << endl;
            }
        }
    }

    bool isLeaf() {
        if (point != nullptr) {
            return true;
        }

        bool allChildrenEmpty = true;
        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr) {
                allChildrenEmpty = false;
                break;
            }
        }

        return allChildrenEmpty;
    }

    void printOctree(int level = 0) {
        // Imprimir la información del nodo actual
        if (point != nullptr) {
            // Si el nodo tiene un punto, imprimimos sus coordenadas
            cout << string(level * 2, ' ') << "Point at ("
                 << point->x << ", " << point->y << ", " << point->z << ")" << endl;
        } else {
            // Si el nodo no tiene un punto, imprimimos el rango (bounds)
            cout << string(level * 2, ' ') << "Node bounds: ("
                 << topLeftFront->x << ", " << topLeftFront->y << ", " << topLeftFront->z << ") to ("
                 << bottomRightBack->x << ", " << bottomRightBack->y << ", " << bottomRightBack->z << ")" << endl;

            // Recorremos los octantes (hijos)
            for (int i = 0; i < 8; ++i) {
                if (children[i] != nullptr) {
                    children[i]->printOctree(level + 1);  // Llamada recursiva para los hijos
                }
            }
        }
    }

};