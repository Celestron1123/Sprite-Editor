/**
 * @file main.cpp
 * @brief Main entry point for the application.
 *
 * This file initializes the application, creates a Sprite object, and sets up
 * the MainWindow with the Sprite. It then starts the application event loop.
 * Checked by Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, and Elijah Potter.
 *
 * @author Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 */

#include "mainwindow.h"
#include "sprite.h"

#include <QApplication>

/**
 * @brief Main entry point for the application.
 *
 * Initializes the QApplication, creates a Sprite with a specified pixel size,
 * and sets up the MainWindow for displaying the sprite. It enters the event loop
 * to start the application.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return int The exit status of the application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sprite s(16);
    MainWindow w(&s);
    w.show();
    return a.exec();
}
