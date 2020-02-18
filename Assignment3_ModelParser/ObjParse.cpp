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
            std::string tag = line.substr(0,2);
            // std::cout << "the line: " + line << std::endl;
            if (tag == "v ") {
                int lastSpace = 2;
                int i = 2;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        // std::cout << line.substr(lastSpace, i-lastSpace) << "\n";
                        verts.push_back(std::stof(line.substr(lastSpace, i-lastSpace)));
                        lastSpace = i;
                    }
                    i++;
                }
                // std::cout << line.substr(lastSpace) << "\n";
                verts.push_back(std::stof(line.substr(lastSpace)));
            }
            if (tag == "vn ") {
                int lastSpace = 3;
                int i = 3;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        // std::cout << line.substr(lastSpace, i-lastSpace) << "\n";
                        vertNormals.push_back(std::stof(line.substr(lastSpace, i-lastSpace)));
                        lastSpace = i;
                    }
                    i++;
                }
                // std::cout << line.substr(lastSpace) << "\n";
                vertNormals.push_back(std::stof(line.substr(lastSpace)));
            }
            if (tag == "f "){
                int lastSpace = 2;
                int i = 2;
                while(i < line.length()) {
                    if (line.at(i) == '/') {
                        line[i] = ' ';
                    }
                    i++;
                }
                i = 2;
                while (i < line.length()) {
                    //std::cout << i << " @:" << line.at(i) << "\n";
                    if (line.at(i) == ' ') {
                        std::string numberStr = line.substr(lastSpace, i-lastSpace);
                        if (numberStr != " ") {
                            // std::cout << i << " push:" << numberStr << "\n";
                            face.push_back(std::stoi(numberStr) - 1);
                        }
                        lastSpace = i;
                    }
                    i++;
                }
                //std::cout << " push:" << line.substr(lastSpace) << "\n";
                face.push_back(std::stoi(line.substr(lastSpace)) - 1);
            }
            faces.push_back(face);
        }
        inFile.close();
    }
}