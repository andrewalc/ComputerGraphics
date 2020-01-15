#include "PPM.h"

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
    // TODO:    Load and parse a ppm to get its pixel
    //          data stored properly.
    std::ifstream inFile;
    inFile.open(fileName);
    if (inFile.is_open()) {
        std::string line;
        int count = 0;
        unsigned int r, g, b;
        std::vector<std::string> pixelData;

        while (getline(inFile, line)) {
            if (count == 2) {
                unsigned int i = 0;
                while (i < line.length()) {
                    if (line.at(i) == ' ') {
                        m_width = std::stoi(line.substr(0, i));
                        m_height = std::stoi(line.substr(i + 1));
                        break;
                    }
                    ++i;
                }
            } else if (count > 3) {
                pixelData.push_back(line);

                // int colorIndex = (count - 4) % 3;
                // int colorData = std::stoi(line);
                // if (colorIndex == 0) {
                //     r = colorData;
                // } else if (colorIndex == 1) {
                //     g = colorData;
                // } else if (colorIndex == 2) {
                //     b = colorData;
                //     // Store the rgb data and clear the vars
                // }
            }
        }
        // When we've recorded everything
        int datalen = pixelData.size();
        char* newArray = new char[datalen];
    }
    inFile.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
    // TODO: Save a PPM image to disk
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
    // TODO: Output a 'filtered' PPM image.
}

// Sets a pixel to a specific R,G,B value
void PPM::setPixel(int x, int y, int R, int G, int B) {
    // TODO: Implement
}
