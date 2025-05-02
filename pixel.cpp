/**
 * Pixel class that represents the components of a pixel.
 *
 * @author Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @version March 31, 2025
 *
 * Checked by Julia Maia
 */

#include "pixel.h"
#include <QColor>

Pixel::Pixel(int r, int g, int b, int a, int x, int y) : red(r), green(g), blue(b), alpha(a), x(x), y(y) {}

void Pixel::setColor(int r, int g, int b, int a = 255) {
    // Set pixel color
    red = r; green = g; blue = b; alpha = a;
}

QColor const Pixel::getColor() {
    return QColor(red, green, blue, alpha);
}
