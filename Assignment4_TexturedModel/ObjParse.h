#ifndef OBJPARSE_H
#define OBJPARSE_H

#include <QtGui>
#include <QtOpenGL>
#include <QtWidgets>
#include <fstream>
#include <iostream>
#include <sstream>  // std::istringstream
#include <string>
#include <vector>

class ObjParse {
   public:
    void parse(std::string filename);
    QVector<QVector2D> getVTData2D();
    QVector<QVector3D> getVerts3D();
    std::vector<unsigned int> getIdx();
    static std::vector<unsigned int> getIdx(
        std::vector<std::vector<unsigned int>> faces);
    std::vector<float> verts;
    std::vector<float> vertNormals;
    std::vector<float> vertTextures;
    std::vector<std::vector<int>> faces;
    std::string matFileName;
};
#endif