// Checked by Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, and Elijah Potter

#include "sprite.h"

Sprite::Sprite(int pixelSize, QObject* parent) : QObject{parent}, pixelSize(pixelSize) {
    previewTimer = new QTimer(this);
    connect(previewTimer, &QTimer::timeout, this, &Sprite::updatePreviewFrame);
}

int Sprite::getSelectedFrameIndex() {
    return selectedFrame;
}

Frame* Sprite::getSelectedFrame(int frame) {
    return frames.at(frame);
}

int Sprite::getFrameCount() {
    return frames.count();
}

void Sprite::setPixelSize(int size) {
    pixelSize = size;
}

int Sprite::getPixelSize() {
    return pixelSize;
}

void Sprite::createSprite(int size) {
    setPixelSize(size);

    for(Frame* frame : frames) {
        delete frame;
    }

    // Clear frames for new sprite
    frames.clear();
    emit addInitialFrameSignal(this);
}

void Sprite::putFrameInList(Frame* f) {
    frames.push_back(f);
}

void Sprite::clearFrame() {
    frames.at(selectedFrame)->clear();
}

void Sprite::deleteFrame() {
    getSelectedFrame(selectedFrame)->hide();
    int currentFrameIndex = selectedFrame;
    // Updates the current frame in the sprite
    selectedFrame = std::max(0, currentFrameIndex - 1);
    frames.removeAt(currentFrameIndex);
    // Removes the frame button in the GUI
    emit removeFrameFromLayout(currentFrameIndex, this);
    getSelectedFrame(selectedFrame)->raise();
    getSelectedFrame(selectedFrame)->show();
}

void Sprite::updateSelectedFrame(Frame* frame) {
    selectedFrame = frames.indexOf(frame);
}

void Sprite::updateSelectedFrame(int frame) {
    selectedFrame = frame;
}

void Sprite::setColor(QColor color) {
    frames.at(selectedFrame)->setColor(color);
}

void Sprite::hideAllFrames() {
    for(Frame* f : frames) {
        f->hide();
    }
}

void Sprite::startPreview(int frameRate) {
    if (frames.isEmpty()) {
        return;
    }

    // Convert fps to milliseconds
    int interval = 1000 / frameRate;
    previewTimer->start(interval);
}

void Sprite::stopPreview() {
    previewTimer->stop();
}

void Sprite::updatePreviewFrame() {
    if (frames.isEmpty()) return;

    // Get the current frame
    Frame* currentFrame = frames[previewFrameIndex];
    if (!currentFrame) return;

    // Convert QImage to QPixmap and scale it to 200x200
    QPixmap pixmap = QPixmap::fromImage(currentFrame->getImage()).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Set it on the preview label
    previewLabel->setPixmap(pixmap);

    // Move to the next frame, looping back if necessary
    previewFrameIndex = (previewFrameIndex + 1) % frames.size();
}

void Sprite::setSquareTool(){
    frames.at(selectedFrame)->setTool(Frame::Square);
}
void Sprite::setCircleTool(){
    frames.at(selectedFrame)->setTool(Frame::Circle);
}
void Sprite::setFreehandTool(){
    frames.at(selectedFrame)->setTool(Frame::Freehand);
}
void Sprite::setEraserTool(){
    frames.at(selectedFrame)->setTool(Frame::Eraser);
}

void Sprite::setLineTool() {
    frames.at(selectedFrame)->setTool(Frame::Line);
}
