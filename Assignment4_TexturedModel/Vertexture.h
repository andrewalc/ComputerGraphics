#ifndef VERTEXTURE_H
#define VERTEXTURE_H
struct Vertexture {
    float x, y, z;
    float a, b;
    Vertexture(float x, float y, float z, float a, float b) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->a = a;
        this->b = b;
    }

    // Tests if two VertexData are equal
    bool operator==(const Vertexture &other) {
        return (x == other.x) && (y == other.y) && (z == other.z) &&
               (a == other.a) && (b == other.b);
    }
};
inline bool operator==(const Vertexture &a, const Vertexture &b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.a == b.a) &&
           (a.b == b.b);
}
#endif