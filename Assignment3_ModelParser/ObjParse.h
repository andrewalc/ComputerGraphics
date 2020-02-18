#ifndef OBJPARSE_H
#define OBJPARSE_H

#include <fstream>
#include <iostream>
#include <sstream>  // std::istringstream
#include <string>
#include <vector>

class ObjParse {
    public:
        void parse(std::string filename);
        std::vector<float> verts;
        std::vector<float> vertNormals;
        std::vector<std::vector<int>> faces;
};
#endif