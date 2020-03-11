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

std::vector<int> ObjParse::getIdx() {
    std::vector<int> idx;
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

std::vector<int> ObjParse::getIdx(std::vector<std::vector<int>> faces) {
    std::vector<int> idx;
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