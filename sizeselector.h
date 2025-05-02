#ifndef SIZESELECTOR_H
#define SIZESELECTOR_H

/**
 * @class SizeSelector
 * @brief Dialog for selecting the pixel size for the sprite.
 *
 * The SizeSelector class provides a UI dialog that allows the user to select the pixel size
 * for a sprite. It emits a signal with the selected pixel size when a choice is made.
 *
 * @authors Steven Luo, Ryan Hamerman, and Elijah Potter.
 * @date 3/31/2025
 * Checked by Steven Luo, Ryan Hamerman, and Elijah Potter.
 */

#include <QDialog>

namespace Ui {
class SizeSelector;
}

/**
 * @class SizeSelector
 * @brief Dialog for selecting the pixel size for the sprite.
 */
class SizeSelector : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SizeSelector object.
     * @param parent The parent QWidget, defaults to nullptr.
     */
    explicit SizeSelector(QWidget *parent = nullptr);

    /**
     * @brief Destroys the SizeSelector object.
     */
    ~SizeSelector();

private slots:
    /**
     * @brief Handles the size selection from the user.
     */
    void handleSizeSelection();

private:
    /**
     * Pointer to the UI for the SizeSelector dialog.
     */
    Ui::SizeSelector *ui;

signals:
    /**
     * @brief Emitted when the user selects a size.
     * @param pixelSize The selected pixel size.
     */
    void sizeSelected(int pixelSize);
};

#endif // SIZESELECTOR_H
