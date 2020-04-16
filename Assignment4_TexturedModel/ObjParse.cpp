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
            if (tag == "vt") {
                int i = 3;
                std::string numBuffer = "";
                while (i < line.length()) {
                    if (line.at(i) == '/' || line.at(i) == ' ' ||
                        line.at(i) == '\n') {
                        vertTextures.push_back(std::stof(numBuffer));
                        numBuffer = "";
                    } else {
                        // add the char to the buffer
                        numBuffer += line.at(i);
                    }
                    i++;
                }
                vertTextures.push_back(std::stof(numBuffer));
                numBuffer = "";
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
                int i = 2;
                std::string numBuffer = "";
                while (i < line.length()) {
                    if (line.at(i) == '/' || line.at(i) == ' ' ||
                        line.at(i) == '\n') {
                        face.push_back(std::stoi(
                            numBuffer));  // indices are 1 based, subtract 1
                        numBuffer = "";
                    } else {
                        // add the char to the buffer
                        numBuffer += line.at(i);
                    }
                    i++;
                }
                face.push_back(std::stoi(numBuffer));
                numBuffer = "";
                faces.push_back(face);
            }
            faces.push_back(face);
        }
        inFile.close();
    }

    QVector<QVector3D> verts = getVerts3D();
    QVector<QVector2D> VTData = getVTData2D();

    for (int i = 0; i < faces.size(); i++) {         // each line
        for (int j = 0; j < faces[i].size(); j++) {  // each num
            if (j % 3 == 0) {
                // std::cout << faces[i][j] << " / " <<  faces[i][j+1] << "\n";
                QVector3D position = verts.at(faces[i][j] - 1);
                QVector2D texture = VTData.at(faces[i][j + 1] - 1);
                Vertexture vt =
                    Vertexture(position.x(), position.y(), position.z(),
                               texture.x(), texture.y());
                if (!vertextures.contains(vt)) {
                    vertextures.push_back(vt);
                }
                idx.push_back(vertextures.indexOf(vt));
            }
        }
    }
    // std::cout << "verts len " <<  verts.size() << "\n";
    // std::cout << "VTDATA len " <<  VTData.size() << "\n";
    // std::cout << "vertextures len " <<  vertextures.size() << "\n";
}

QVector<QVector2D> ObjParse::getVTData2D() {
    QVector<QVector2D> vertTextureData;
    QVector2D vector;
    int xCount = 0;
    int yCount = 0;
    for (int i = 0; i < vertTextures.size(); i++) {
        float value = vertTextures[i];
        if (i % 2 == 0) {
            vector.setX(value * -1);
            xCount++;
        } else if (i % 2 == 1) {
            yCount++;
            vector.setY(value * -1);
            if (!vertTextureData.contains(vector)) {
                vertTextureData.push_back(vector);
            }
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
        } else if (i % 3 == 1) {
            vector.setY(verts[i]);
        } else if (i % 3 == 2) {
            vector.setZ(verts[i]);
            vertsVector.push_back(vector);
        }
    }
    //std::cout << "Verts length " << vertsVector.length() << "\n";
    return vertsVector;
}

std::vector<unsigned int> ObjParse::getIdx() { return idx; }
