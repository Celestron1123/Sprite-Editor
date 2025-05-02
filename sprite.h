#ifndef SPRITE_H
#define SPRITE_H

/**
 * @class Sprite
 * @brief Represents a sprite composed of multiple frames with tools for editing.
 *
 * The Sprite class manages a collection of frames, provides tools for editing,
 * and supports animation previews using a timer.
 *
 * @authors Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @date 3/31/2025
 * Checked by Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, and Elijah Potter.
 */

#include "frame.h"
#include <QLabel>
#include <QTimer>
#include <QObject>

class Sprite : public QObject {
    Q_OBJECT

private:
    /**
     * Size of each pixel in the sprite.
     */
    int pixelSize;

    /**
     * List of frames in the sprite.
     */
    QVector<Frame*> frames;

    /**
     * Index of the currently selected frame.
     */
    int selectedFrame = 0;

    /**
     * Timer for previewing the animation.
     */
    QTimer* previewTimer;

    /**
     * QLabel for displaying animation preview.
     */
    QLabel* previewLabel;

    /**
     * Current frame index in the animation preview.
     */
    int previewFrameIndex = 0;

public:
    /**
     * @brief Constructs a Sprite object with a specified pixel size.
     * @param pixelSize The size of each pixel in the sprite.
     * @param parent The parent QObject, defaults to nullptr.
     */
    explicit Sprite(int pixelSize, QObject* parent = nullptr);

    /**
     * @brief Adds a frame to the list of frames in the sprite.
     * @param frame Pointer to the Frame object to be added.
     */
    void putFrameInList(Frame* frame);

    /**
     * @brief Gets the index of the currently selected frame.
     * @return Index of the selected frame.
     */
    int getSelectedFrameIndex();

    /**
     * @brief Gets the total number of frames in the sprite.
     * @return The number of frames.
     */
    int getFrameCount();

    /**
     * @brief Gets the pixel size of the sprite.
     * @return The pixel size.
     */
    int getPixelSize();

    /**
     * @brief Retrieves a frame at a given index.
     * @param frame Index of the frame to retrieve.
     * @return Pointer to the Frame object.
     */
    Frame* getSelectedFrame(int frame);

    /**
     * @brief Sets the drawing color for the current frame of the Sprite.
     * @param color The QColor to set.
     */
    void setColor(QColor color);

    /**
     * @brief Starts the animation preview at a specified frame rate.
     * @param frameRate The frame rate in milliseconds per frame.
     */
    void startPreview(int frameRate);

    /**
     * @brief Stops the animation preview.
     */
    void stopPreview();

    /**
     * @brief Hides all frames in the sprite.
     */
    void hideAllFrames();

    /**
     * @brief Activates the square drawing tool.
     */
    void setSquareTool();

    /**
     * @brief Activates the circle drawing tool.
     */
    void setCircleTool();

    /**
     * @brief Activates the freehand drawing tool.
     */
    void setFreehandTool();

    /**
     * @brief Activates the eraser tool.
     */
    void setEraserTool();

    /**
     * @brief Activates the line drawing tool.
     */
    void setLineTool();

private:
    /**
     * @brief Sets the pixel size of the sprite.
     * @param size The new pixel size.
     */
    void setPixelSize(int size);

signals:
    /**
     * @brief Signal emitted when an initial frame is added to the sprite.
     * @param s Pointer to the Sprite object.
     */
    void addInitialFrameSignal(Sprite* s);

    /**
     * @brief Signal emitted when a frame is removed from the layout.
     * @param frameIndex The index of the frame to remove.
     * @param s Pointer to the Sprite object.
     */
    void removeFrameFromLayout(int frameIndex, Sprite* s);

public slots:
    /**
     * @brief Creates a new sprite with a given size.
     * @param size The size of the sprite.
     */
    void createSprite(int size);

    /**
     * @brief Clears the currently selected frame.
     */
    void clearFrame();

    /**
     * @brief Deletes the currently selected frame.
     */
    void deleteFrame();

    /**
     * @brief Updates the selected frame with a new frame object.
     * @param frame Pointer to the new Frame object.
     */
    void updateSelectedFrame(Frame* frame);

    /**
     * @brief Updates the selected frame using an index.
     * @param frame The index of the new selected frame.
     */
    void updateSelectedFrame(int frame);

private slots:
    /**
     * @brief Updates the animation preview frame.
     */
    void updatePreviewFrame();
};

#endif // SPRITE_H
