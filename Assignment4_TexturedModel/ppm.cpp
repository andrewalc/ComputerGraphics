#include "PPM.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    // TODO:    Load and parse a ppm to get its pixel
    //          data stored properly.
    std::ifstream inFile;
    inFile.open(fileName);
    if (inFile.is_open()) {
        std::string line;
        int lineCount = 0;

        while (getline(inFile, line)) {
            lineCount++;
        }
        m_PixelData = new unsigned int[lineCount - 4];
        pixelDataSize = lineCount - 4;
        inFile.clear();
        inFile.seekg(0, std::ios::beg);

        lineCount = 1;
        while (getline(inFile, line)) {
            if (lineCount != 2) {
                int commentIndex = line.find('#') > 0 ? line.find('#') : 0;
                line = line.substr(0, commentIndex);
            }

            if (lineCount == 1) {
                header = line;
            } else if (lineCount == 2) {
                comment = line;
            } else if (lineCount == 3) {
                unsigned int i = 0;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        m_width = std::stoi(line.substr(0, i));
                        m_height = std::stoi(line.substr(i + 1));
                        break;
                    }
                    ++i;
                }
            } else if (lineCount == 4) {
                max_value = std::stoul(line);
            } else {
                int actualValue = std::stoul(line);
                // apply ratio
                m_PixelData[lineCount - 5] = actualValue * 255 / max_value;
                // std::stringstream ss(line);
                // do {
                //     int value;
                //     ss >> value;
                //     m_PixelData[lineCount - 5] = value;
                //     lineCount++;
                // } while (ss);
            }
            lineCount++;
        }
        inFile.close();
    }
}

// Destructor clears any memory that has been allocated
PPM::~PPM() { delete[] m_PixelData; }

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
    std::ofstream outFile;
    outFile.open(outputFileName);
    outFile << header << std::endl;
    outFile << comment << std::endl;
    outFile << m_width << " " << m_height << std::endl;
    outFile << max_value << std::endl;
    for (int i = 0; i < pixelDataSize; i++) {
        int thing = *(m_PixelData + i);
        outFile << *(m_PixelData + i) << std::endl;
    }
    // Closes the file we are streaming data to
    outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
    for (int i = 0; i < pixelDataSize; i++) {
        int thing = *(m_PixelData + i);
        *(m_PixelData + i) = (thing - 50) >= 0 ? (thing - 50) : 0;
    }
}

// Sets a pixel to a specific R,G,B value
void PPM::setPixel(int x, int y, int R, int G, int B) {
    // TODO: Implement
    int i = (y * m_width * 3) + (x * 3);
    *(m_PixelData + i) = R;
    *(m_PixelData + i + 1) = G;
    *(m_PixelData + i + 2) = B;
}

QVector<QVector2D> PPM::getPixelData2D(){
    QVector<QVector2D> pixelData;
    QVector2D vector;
    for (int i = 0; i < pixelDataSize; i++) {
        int value = *(m_PixelData + i);
        if (i % 2 == 0) {
            vector.setX(value);
        } else if (i % 2 == 1) {
            vector.setY(value);
            pixelData.push_back(vector);
        }
    }
    return pixelData;
}
