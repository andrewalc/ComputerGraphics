#include "ObjParse.h"

// Parse data from .obj
void ObjParse::parse(std::string fileName) {
    std::ifstream inFile;
    inFile.open(fileName);
    if (inFile.is_open()) {
        std::string line;
        inFile.clear();
        inFile.seekg(0, std::ios::beg);
        while (getline(inFile, line)) {
            std::vector<int> face;
            std::string tag = line.substr(0, 2);
            if (line.substr(0, 7) == "mtllib") {
                matFileName = line.substr(8);
            }
            if (tag == "v ") {
                int lastSpace = 2;
                int i = 2;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        verts.push_back(
                            std::stof(line.substr(lastSpace, i - lastSpace)));
                        lastSpace = i;
                    }
                    i++;
                }
                verts.push_back(std::stof(line.substr(lastSpace)));
            }
            if (tag == "vt ") {
                int lastSpace = 3;
                int i = 3;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        vertTextures.push_back(
                            std::stof(line.substr(lastSpace, i - lastSpace)));
                        lastSpace = i;
                    }
                    i++;
                }
                vertTextures.push_back(std::stof(line.substr(lastSpace)));
            }
            if (tag == "vn ") {
                int lastSpace = 3;
                int i = 3;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        vertNormals.push_back(
                            std::stof(line.substr(lastSpace, i - lastSpace)));
                        lastSpace = i;
                    }
                    i++;
                }
                vertNormals.push_back(std::stof(line.substr(lastSpace)));
            }
            if (tag == "f ") {
                int lastSpace = 2;
                int i = 2;
                while (i < line.length()) {
                    if (line.at(i) == '/') {
                        line[i] = ' ';
                    }
                    i++;
                }
                i = 2;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        std::string numberStr =
                            line.substr(lastSpace, i - lastSpace);
                        if (numberStr != " ") {
                            face.push_back(std::stoi(numberStr) - 1);
                        }
                        lastSpace = i;
                    }
                    i++;
                }
                face.push_back(std::stoi(line.substr(lastSpace)) - 1);
            }
            faces.push_back(face);
        }
        inFile.close();
    }
}

QVector<QVector2D> ObjParse::getVTData2D() {
    QVector<QVector2D> vertTextureData;
    QVector2D vector;
    for (int i = 0; i < vertTextures.size(); i++) {
        int value = vertTextures[i];
        if (i % 2 == 0) {
            std::cout << "VT A : " << vertTextures[i] << "\n";
            vector.setX(value);
        } else if (i % 2 == 1) {
            std::cout << "VT B : " << vertTextures[i] << "\n";
            vector.setY(value);
            vertTextureData.push_back(vector);
        }
    }
    return vertTextureData;
}

QVector<QVector3D> ObjParse::getVerts3D() {
    QVector<QVector3D> vertsVector;
    QVector3D vector;
    for (int i = 0; i < verts.size(); i++) {
        if (i % 3 == 0) {
            vector.setX(verts[i]);
            // std::cout << "VEC X : " << verts[i] << "\n";
        } else if (i % 3 == 1) {
            // std::cout << "VEC Y : " << verts[i] << "\n";
            vector.setY(verts[i]);
        } else if (i % 3 == 2) {
            // std::cout << "VEC Z : " << verts[i] << "\n";
            vector.setZ(verts[i]);
            vertsVector.push_back(vector);
        }
    }
    return vertsVector;
}

std::vector<unsigned int> ObjParse::getIdx() {
    std::vector<unsigned int> idx;
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].size() > 0) {
            for (int j = 0; j < faces[i].size(); j++) {
                if (j % 2 == 0) {
                    idx.push_back(faces[i][j]);
                }
            }
        }
    }
    return idx;
}

std::vector<unsigned int> ObjParse::getIdx(
    std::vector<std::vector<unsigned int>> faces) {
    std::vector<unsigned int> idx;
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].size() > 0) {
            for (int j = 0; j < faces[i].size(); j++) {
                if (j % 2 == 0) {
                    idx.push_back(faces[i][j]);
                }
            }
        }
    }
    return idx;
}