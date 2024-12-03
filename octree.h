#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBottom 4
#define TopRightBottom 5
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
    }

    Octree(T x, T y, T z) {
        point = new Point<T>(x, y, z);
    }

    int getOctant(T x, T y, T z, T midx, T midy, T midz) {
      
        if (x <= midx) {
            if (y <= midy) {
                if (z <= midz)
                    return TopLeftFront;
                else
                    return TopLeftBottom;
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
                    return TopRightBottom;
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
    }

    void insert(T x, T y, T z) {
        if (find(x, y, z)) {
            throw invalid_argument("Point already exists");
        }

        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z) {
            throw invalid_argument("Point is out of bounds");
        }

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);
        totalPoints = totalPoints + 1;

        if (children[index]->point == nullptr) {
            children[index]->insert(x, y, z);
            return;
        }

        else if (children[index]->point->x == -1) {
            delete children[index];
            children[index] = new Octree<T>(x, y, z);
            return;
        } else {
            T x_ = children[index]->point->x,
                y_ = children[index]->point->y,
                z_ = children[index]->point->z;
            delete children[index];
            children[index] = nullptr;

            if (index == TopLeftFront) {
                children[index] = new Octree<T>(topLeftFront->x, topLeftFront->y, topLeftFront->z, midx, midy, midz);
            } else if (index == TopRightFront) {
                children[index] = new Octree<T>(midx + 1, topLeftFront->y, topLeftFront->z, bottomRightBack->x, midy, midz);
            } else if (index == BottomRightFront) {
                children[index] = new Octree<T>(midx + 1, midy + 1, topLeftFront->z, bottomRightBack->x, bottomRightBack->y, midz);
            } else if (index == BottomLeftFront) {
                children[index] = new Octree<T>(topLeftFront->x, midy + 1, topLeftFront->z, midx, bottomRightBack->y, midz);
            } else if (index == TopLeftBottom) {
                children[index] = new Octree<T>(topLeftFront->x, topLeftFront->y, midz + 1, midx, midy, bottomRightBack->z);
            } else if (index == TopRightBottom) {
                children[index] = new Octree<T>(midx + 1, topLeftFront->y, midz + 1, bottomRightBack->x, midy, bottomRightBack->z);
            } else if (index == BottomRightBack) {
                children[index] = new Octree<T>(midx + 1, midy + 1, midz + 1, bottomRightBack->x, bottomRightBack->y, bottomRightBack->z);
            } else if (index == BottomLeftBack) {
                children[index] = new Octree<T>(topLeftFront->x, midy + 1, midz + 1, midx, bottomRightBack->y, bottomRightBack->z);
            }
            children[index]->insert(x_, y_, z_);
            children[index]->insert(x, y, z);
        }
    }

    bool find(T x, T y, T z) {
        if (x < topLeftFront->x || x > bottomRightBack->x ||
            y < topLeftFront->y || y > bottomRightBack->y ||
            z < topLeftFront->z || z > bottomRightBack->z)
            return false;

        T midx = (topLeftFront->x + bottomRightBack->x) / 2;
        T midy = (topLeftFront->y + bottomRightBack->y) / 2;
        T midz = (topLeftFront->z + bottomRightBack->z) / 2;

        int index = getOctant(x, y, z, midx, midy, midz);

        if (children[index]->point == nullptr) {
            return children[index]->find(x, y, z);
        }

        else if (children[index]->point->x == -1) {
            return false;
        } else {
            if (x == children[index]->point->x &&
                y == children[index]->point->y &&
                z == children[index]->point->z)
                return true;
        }
        return false;
    }
      
    Point<T>* getPoint(T x, T y, T z) {
    if (x < topLeftFront->x || x > bottomRightBack->x ||
        y < topLeftFront->y || y > bottomRightBack->y ||
        z < topLeftFront->z || z > bottomRightBack->z) {
        return nullptr; 
    }

    T midx = (topLeftFront->x + bottomRightBack->x) / 2;
    T midy = (topLeftFront->y + bottomRightBack->y) / 2;
    T midz = (topLeftFront->z + bottomRightBack->z) / 2;

    int index = getOctant(x, y, z, midx, midy, midz);

    if (children[index]->point != nullptr) {
        if (children[index]->point->x == x && children[index]->point->y == y && children[index]->point->z == z) {
            return children[index]->point;  
        }
    }

    if (children[index]->children[0] != nullptr) {
        return children[index]->getPoint(x, y, z);  
    }

    return nullptr; 
}


    int countPoints() {
        return totalPoints;
    }

   void remove(T x, T y, T z) {
    if (x < topLeftFront->x || x > bottomRightBack->x ||
        y < topLeftFront->y || y > bottomRightBack->y ||
        z < topLeftFront->z || z > bottomRightBack->z) {
        return; 
    }

    T midx = (topLeftFront->x + bottomRightBack->x) / 2;
    T midy = (topLeftFront->y + bottomRightBack->y) / 2;
    T midz = (topLeftFront->z + bottomRightBack->z) / 2;

    int index = getOctant(x, y, z, midx, midy, midz);
                totalPoints--;  

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



};