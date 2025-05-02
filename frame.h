#ifndef FRAME_H
#define FRAME_H

/**
 * @class Frame
 * @brief A QWidget based class for drawing and editing a pixel-based image
 * based on events from user interaction.
 *
 * @authors Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 *
 * Checked by Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, and Elijah Potter.
 */

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QStack>
#include <vector>
#include "pixel.h"
class Frame : public QWidget
{
    Q_OBJECT

private:
    /**
     * @brief QImage to be drawn on
     */
    QImage canvas;
    QVector<Pixel> pixels;
    std::vector<int> dimensions;
    QColor color;
    int pixelSize;
    bool showGrid = true;
    bool blockDrawingOnGrid = true;
    bool drawingShape = false;
    QPoint start;
    QPoint end;
    QRect rect;
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

public:
    /**
     * @brief Creates a Frame object.
     * @param pixelSize The size of each pixel on the canvas.
     */
    explicit Frame(int pixelSize, QWidget *parent = nullptr);

    /**
     * @brief Paint event handler.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Handles mouse press events (when user begins drawing).
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse movement events (when user drags
     * after having clicked).
     * @param event The mouse event.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Handles mouse release events.
     * @param event The mouse event.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Clears frame. Set all pixels back to transparent.
     */
    void clear();

    /**
     * @brief Sets the current drawing color.
     * @param newColor The new color to be set.
     */
    void setColor(QColor newColor);

    /**
     * @brief Returns the current canvas.
     */
    QImage getImage();

    /**
     * @brief Sets the canvas image.
     * @param img The image to set.
     */
    void setImage(const QImage &img);

    enum Tool
    {
        Freehand,
        Square,
        Circle,
        Eraser,
        Line
    };

    /**
     * @brief Sets the current drawing tool.
     * @param newTool The tool to be set.
     */
    void setTool(Tool newTool);

    /**
     * @brief Undoes the last drawing action.
     */
    void undo();

    /**
     * @brief Redoes the last undone drawing action.
     */
    void redo();

    /**
     * @brief Checks if an undo action is possible.
     * @return True if undo is possible, false otherwise.
     */
    bool canUndo();

    /**
     * @brief Checks if a redo action is possible.
     * @return True if redo is possible, false otherwise.
     */
    bool canRedo();

    /**
     * @brief Draws a line on the canvas.
     * @param painter The painter used for drawing.
     */
    void drawLine(QPainter &painter);

private:
    /**
     * @brief Sets pixel size.
     * @param size The new size to set pixels to.
     */
    void setPixelSize(int size);

    /**
     * @brief Draws the canvas grid.
     * @param painter The QPainter used for drawing.
     */
    void drawGrid(QPainter &painter);

    /**
     * @brief Checks if user clicked on a coordinate of the grid.
     * @return True if user clicked on a gridline, false otherwise.
     */
    bool isOnGridLine(int x, int y) const;

    /**
     * @brief Draws a circle
     * @param painter The QPainter used for drawing
     */
    void drawCircle(QPainter &painter);

    /**
     * @brief Pushes the current state of canvas to undo stack.
     */
    void pushUndo();

    Tool currentTool;

signals:
    /**
     * @brief Signal emitted when the frame changes.
     */
    void frameChanged();
};

#endif // FRAME_H
