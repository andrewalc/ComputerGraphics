/** @file PPM.h
 *  @brief Class for working with PPM images
 *
 *  Class for working with P3 PPM images specifically.
 *
 *  @author your_name_here
 *  @bug No known bugs.
 */
#ifndef PPM_H
#define PPM_H

#include <fstream>
#include <iostream>
#include <sstream>  // std::istringstream
#include <string>
#include <vector>

class PPM {
   public:
    // Constructor loads a filename with the .ppm extension
    PPM(std::string fileName);
    // Destructor clears any memory that has been allocated
    ~PPM();
    // Saves a PPM Image to a new file.
    void savePPM(std::string outputFileName);
    // Darken subtracts 50 from each of the red, green
    // and blue color components of all of the pixels
    // in the PPM. Note that no values may be less than
    // 0 in a ppm.
    void darken();
    // Sets a pixel to a specific R,G,B value
    void setPixel(int x, int y, int R, int G, int B);
    // Returns the raw pixel data in an array.
    // You may research what 'inline' does.
    inline unsigned int* pixelData() { return m_PixelData; }
    // Returns image width
    inline int getWidth() { return m_width; }
    // Returns image height
    inline int getHeight() { return m_height; }
    // NOTE:    You may add any helper functions you like in the
    //          private section.
   private:
    // Store the raw pixel data here
    // Data is R,G,B format
    unsigned int* m_PixelData;
    unsigned int pixelDataSize;
    // Store width and height of image.
    std::string header;
    std::string comment;

    int m_width{0};
    int m_height{0};
    int max_value{0};
};

#endif
