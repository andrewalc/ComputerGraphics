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
#include "Vertexture.h"

class ObjParse {
   public:
    void parse(std::string filename);
    QVector<QVector2D> getVTData2D();
    QVector<QVector3D> getVerts3D();
    QVector<QVector3D> getNormals3D();
    std::vector<unsigned int> idx;
    std::vector<unsigned int> getIdx();
    std::vector<float> verts;
    std::vector<float> vertNormals;
    std::vector<float> vertTextures;
    std::vector<std::vector<int>> faces;
    std::string matFileName;
    QVector<Vertexture> vertextures;
};
#endif