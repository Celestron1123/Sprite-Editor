#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @class MainWindow
 * @brief A view class that handles everything displayed in the main window of the UI.
 *
 * @authors Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 *
 * Checked by Julia Maia
 */

#include <QMainWindow>
#include "sprite.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * MainWindow constructor.
     * @param sprite
     * @param parent
     */
    MainWindow(Sprite *sprite, QWidget *parent = nullptr);

    /**
     * MainWindow destructor
     */
    ~MainWindow();

public slots:

    /**
     * Adds a new frame to the window.
     * @param s
     */
    void addFrame(Sprite *s);

    /**
     * Removes the specified frame from the window.
     * @param frameIndex
     * @param s
     */
    void removeFrameFromLayout(int frameIndex, Sprite *s);

    /**
     * Clears all drawings from the frame.
     */
    void clearFrames();

    /**
     * Displays the size options in the window when making a new sprite.
     * @param s
     */
    void showSizeSelector(Sprite *s);

    /**
     * Sets a new color for the paintbrush when one is chosen.
     */
    void on_color_clicked();

signals:
    /**
     * Emitted when the color is changed to update the window.
     * @param color
     */
    void colorChanged(QColor color);

private slots:
    /**
     * Sets the zoomFactor when zoomOut is clicked.
     */
    void on_zoomOut_clicked();

    /**
     * Sets the zoomFactor when zoomIn is clicked.
     */
    void on_zoomIn_clicked();

    /**
     * Resets the zoomFactor when reset is clicked.
     */
    void on_reset_clicked();

    /**
     * Displays the help window when help is clicked.
     */
    void on_help_clicked();

private:
    Ui::MainWindow *ui;
    int theZoomFactor;

    /**
     * Saves the current sprite
     * @param sprite
     * @param fileName
     */
    void saveSprite(Sprite *sprite, const QString &fileName);

    /**
     * Loads the selected sprite into the editor.
     * @param sprite
     * @param fileName
     */
    void loadSprite(Sprite *sprite, const QString &fileName);

    /**
     * Updates the undo and redo buttons to be enabled or disabled.
     * @param frame
     */
    void updateUndoRedoButtons(Frame* frame);

    /**
     * Enables frame, file, and menu buttons.
     */
    void enableButtons();

};
#endif // MAINWINDOW_H
