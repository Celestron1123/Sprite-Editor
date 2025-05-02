#ifndef PIXEL_H
#define PIXEL_H

/**
 * @class Pixel
 * @brief Pixel class that represents the components of a pixel.
 *
 * @author Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 *
 * Checked by Julia Maia
 */

#include <QColor>

class Pixel
{
private:

    // RGBA components
    int red, green, blue, alpha;

    // mouse coordinates
    int x, y;
public:

    /**
     * Pixel constructor
     * @param r - red value
     * @param g - green value
     * @param b - blue value
     * @param a - alpha value
     * @param x - x coordinate of the mouse
     * @param y - y coordinate of the mouse
     */
    Pixel(int r = 0, int g = 0, int b = 0, int a = 255, int x = 0, int y = 0);

    /**
     * Sets the color of the pixel
     * @param r - red value
     * @param g - green value
     * @param b - blue value
     * @param a - alpha value
     */
    void setColor(int r, int g, int b, int a);

    /**
     * Gets the color of a given pixel
     * @return the color of the pixel
     */
    QColor const getColor();
};

#endif // PIXEL_H
