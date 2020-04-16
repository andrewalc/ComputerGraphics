#ifndef VERTEXTURE_H
#define VERTEXTURE_H
struct Vertexture {
    float x, y, z;
    float nx, ny, nz;
    float a, b;
    Vertexture(float x, float y, float z, float nx, float ny, float nz, float a, float b) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->nx = nx;
        this->ny = ny;
        this->nz = nz;
        this->a = a;
        this->b = b;
    }

    // Tests if two VertexData are equal
    bool operator==(const Vertexture &other) {
        return (x == other.x) && (y == other.y) && (z == other.z) &&
               (nx == other.nx) && (ny == other.ny) && (nz == other.nz) &&
               (a == other.a) && (b == other.b);
    }
};
inline bool operator==(const Vertexture &a, const Vertexture &b) {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.nx == b.nx) && (a.ny == b.ny) && (a.nz == b.nz) &&
            (a.a == b.a) && (a.b == b.b);
}
#endif