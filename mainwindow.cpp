/**
 * A view class that handles everything displayed in the main window of the UI.
 *
 * @author Ryan Hamerman, Steven Luo, Julia Maia, Yudith Mendoza, William Ngo, Elijah Potter
 * @version March 31, 2025
 *
 * Checked by Julia Maia
 */

#include "mainwindow.h"
#include "frame.h"
#include "help.h"
#include "sizeselector.h"
#include "sprite.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(Sprite *sprite, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setFixedSize(1120, 630);
    this->setWindowTitle("Sprite Editor");
    theZoomFactor = 200;

    // Connect new menu button to pop up sprite size selection window
    connect(ui->actionNew, &QAction::triggered, this,
            [this, sprite]() { showSizeSelector(sprite); });

    // Connect the add frame button to add a new frame
    connect(ui->actionAdd_Frame, &QAction::triggered, this, [this, sprite]() {
        addFrame(sprite);
        Frame *currentFrame = sprite->getSelectedFrame(sprite->getSelectedFrameIndex());
        updateUndoRedoButtons(currentFrame);
    });

    // Connect the addInitialFrameSignal to add an initial new frame
    connect(sprite, &Sprite::addInitialFrameSignal, this, [this, sprite]() {
        addFrame(sprite);
        enableButtons();
        Frame *currentFrame = sprite->getSelectedFrame(sprite->getSelectedFrameIndex());
        updateUndoRedoButtons(currentFrame);
    });

    // Connect the delete frame button to delete the current frame
    connect(ui->actionDelete_Frame, &QAction::triggered, sprite,
            &Sprite::deleteFrame);

    // Connect the sprite delete slot to the main window
    connect(sprite, &Sprite::removeFrameFromLayout, this,
            &MainWindow::removeFrameFromLayout);

    // Connect the clear button to clear the current frame
    connect(ui->actionClear, &QAction::triggered, sprite, &Sprite::clearFrame);

    // Connect the colorChanged signal to Sprite setColor
    connect(this, &MainWindow::colorChanged, sprite, &Sprite::setColor);

    // Connect the save button to the main window
    connect(ui->actionSave, &QAction::triggered, this, [this, sprite]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Sprite", "", "Sprite Sheet Project (*.ssp)");
        if (!fileName.isEmpty()) {
            saveSprite(sprite, fileName);
        }
    });

    // Connect the load button to the main window
    connect(ui->actionLoad, &QAction::triggered, this, [this, sprite]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Load Sprite", "", "Sprite Sheet Project (*.ssp)");
        if (!fileName.isEmpty()) {
            loadSprite(sprite, fileName);
        }
    });

    // Connect undo button to the main window
    connect(ui->actionUndo, &QAction::triggered, this, [sprite, this]() {
        Frame *currentFrame = sprite->getSelectedFrame(sprite->getSelectedFrameIndex());
        if (currentFrame) {
            currentFrame->undo();
            updateUndoRedoButtons(currentFrame);
        }
    });

    // Connect redo button to the main window
    connect(ui->actionRedo, &QAction::triggered, this, [sprite, this]() {
        Frame *currentFrame = sprite->getSelectedFrame(sprite->getSelectedFrameIndex());
        if (currentFrame) {
            currentFrame->redo();
            updateUndoRedoButtons(currentFrame);
        }
    });

    // Preview Creation

    // Create a QLabel dynamically (for preview)
    QLabel *previewLabel = new QLabel(this);
    previewLabel->setGeometry(900, 40, 200, 200);
    previewLabel->setStyleSheet("border: 2px solid black; background-color: transparent;");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->show();

    // Create a QTimer dynamically (for animation)
    QTimer *previewTimer = new QTimer(this);
    int *previewFrameIndex = new int(0);

    // Set default FPS to 5 (200 ms interval)
    int defaultFPS = 5;
    previewTimer->setInterval(1000 / defaultFPS);

    // Function to update the preview display
    auto updatePreview = [this, sprite, previewLabel, previewFrameIndex]() {
        int frameCount = sprite->getFrameCount();

        if (frameCount == 0) {
            previewLabel->clear();
            return;
        }

        // Prevent previewFrameIndex from pointing out of bounds
        if (*previewFrameIndex >= frameCount)
            *previewFrameIndex = 0;

        Frame *currentFrame = sprite->getSelectedFrame(*previewFrameIndex);
        if (!currentFrame)
            return;

        QPixmap pixmap =
            QPixmap::fromImage(currentFrame->getImage())
                             .scaled(theZoomFactor, theZoomFactor, Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);

        previewLabel->setPixmap(pixmap);

        *previewFrameIndex = (*previewFrameIndex + 1) % frameCount;
    };

    // Connect the FPS slider to dynamically update animation speed
    connect(ui->FPSSlider, &QSlider::valueChanged, this, [previewTimer, updatePreview](int value) {
        if (value == 0) {
            previewTimer->stop();
            updatePreview();
        } else {
            previewTimer->setInterval(1000 / value);
            if (!previewTimer->isActive()) {
                previewTimer->start();
            }
        }
    });

    // Connect the timer to update the preview
    connect(previewTimer, &QTimer::timeout, this, updatePreview);

    // Start preview automatically when a new sprite is created
    connect(sprite, &Sprite::addInitialFrameSignal, this, [updatePreview, previewFrameIndex, previewTimer]() {
        *previewFrameIndex = 0;
        // Show the first frame immediately
        updatePreview();
        // Start the preview automatically with the default FPS
        previewTimer->start();
    });

    // Connect the square button to the Sprite
    connect(ui->square, &QPushButton::clicked, sprite, &Sprite::setSquareTool);

    // Connect the circle button to the Sprite
    connect(ui->circle, &QPushButton::clicked, sprite, &Sprite::setCircleTool);

    // Connect the paintBrush button to the Sprite
    connect(ui->paintBrush, &QPushButton::clicked, sprite, &Sprite::setFreehandTool);

    // Connect the eraser button to the Sprite
    connect(ui->eraser, &QPushButton::clicked, sprite, &Sprite::setEraserTool);

    // Connect the line button to the Sprite
    connect(ui->line, &QPushButton::clicked, sprite, &Sprite::setLineTool);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::enableButtons() {
    ui->actionAdd_Frame->setEnabled(true);
    ui->actionClear->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->paintBrush->setEnabled(true);
    ui->eraser->setEnabled(true);
    ui->line->setEnabled(true);
    ui->square->setEnabled(true);
    ui->circle->setEnabled(true);
    ui->zoomIn->setEnabled(true);
    ui->zoomOut->setEnabled(true);
    ui->reset->setEnabled(true);
    ui->color->setEnabled(true);
}

void MainWindow::addFrame(Sprite *s) {
    // Get the widget inside the scroll area
    QWidget *contentWidget = ui->frames->widget();

    // Get or create the layout inside the widget
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
    if (!layout) {
        layout = new QVBoxLayout(contentWidget);
        layout->setAlignment(Qt::AlignTop);
        contentWidget->setLayout(layout);
    }

    Frame *frame = new Frame(s->getPixelSize(), this);
    frame->setFixedSize(512, 512);
    frame->move(250, 50);
    s->hideAllFrames();
    frame->show();

    s->putFrameInList(frame);
    s->updateSelectedFrame(frame);

    connect(frame, &Frame::frameChanged, this, [this, frame] {
        updateUndoRedoButtons(frame);
    });

    QPushButton *frameButton = new QPushButton(QString("Frame " + QString::number(s->getFrameCount())));

    // Connect the frameButton to the current frame
    connect(frameButton, &QPushButton::clicked, this, [frame, s, this]() {
        updateUndoRedoButtons(frame);
        // Bring the clicked frame to the front
        frame->raise();
        s->hideAllFrames();
        frame->show();
        s->updateSelectedFrame(frame);
        update();
    });

    layout->addWidget(frameButton);

    if (s->getFrameCount() > 1) {
        ui->actionDelete_Frame->setEnabled(true);
    }
}

void MainWindow::clearFrames() {
    // Get the widget inside the scroll area
    QWidget *contentWidget = ui->frames->widget();

    // Get the layout inside the widget
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
    if (layout) {
        // Clear the layout by removing all widgets
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }
}

void MainWindow::removeFrameFromLayout(int frameIndex, Sprite *s) {
    // Get the widget inside the scroll area
    QWidget *contentWidget = ui->frames->widget();

    // Get the layout inside the widget
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
    if (layout) {
        QLayoutItem *item = layout->takeAt(frameIndex);
        if (item) {
            delete item->widget();
            delete item;
        }

        // Update the text of remaining frame buttons
        for (int i = frameIndex; i < layout->count(); ++i) {
            QPushButton *frameButton =
                qobject_cast<QPushButton *>(layout->itemAt(i)->widget());
            if (frameButton) {
                int newIndex = i + 1;
                frameButton->setText(QString("Frame " + QString::number(newIndex)));
            }
        }

        // Disable delete frame button if there is one frame left
        if (s->getFrameCount() < 2) {
            ui->actionDelete_Frame->setEnabled(false);
        }
    }

    // Update undo redo buttons state
    Frame *currentFrame = s->getSelectedFrame(s->getSelectedFrameIndex());
    updateUndoRedoButtons(currentFrame);

    update();
}

void MainWindow::showSizeSelector(Sprite *s) {
    SizeSelector *selector = new SizeSelector(this);
    // Clear frames before creating new sprite
    connect(selector, &SizeSelector::sizeSelected, this, &MainWindow::clearFrames);
    connect(selector, &SizeSelector::sizeSelected, s, &Sprite::createSprite);

    selector->setWindowTitle("Size Selector");
    selector->setModal(true);
    selector->exec();
}

void MainWindow::on_color_clicked() {
    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color",
                                          QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        emit colorChanged(color);
    }
}

void MainWindow::saveSprite(Sprite *sprite, const QString &fileName) {
    QJsonArray framesArray;

    // Iterate through each frame in the Sprite
    for (int i = 0; i < sprite->getFrameCount(); i++) {
        Frame *frame = sprite->getSelectedFrame(i);
        QImage image = frame->getImage();

        // Convert QIMAGE to PNG in order to store it into a byte array
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Store PNG data to base64 (necessary for JSON)
        QJsonObject frameObj;
        frameObj["imageData"] = QString(byteArray.toBase64());
        framesArray.append(frameObj);
    }

    // Create a JSON object with the pixel size and frames array
    QJsonObject rootObj;
    rootObj["pixelSize"] = sprite->getPixelSize();
    rootObj["frames"] = framesArray;

    // Save the data to a file
    QFile saveFile(fileName);
    if (saveFile.open(QIODevice::WriteOnly)) {
        saveFile.write(QJsonDocument(rootObj).toJson());
        saveFile.close();
    }
}

void MainWindow::loadSprite(Sprite *sprite, const QString &fileName) {
    // Open the file; else exit
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    // Parse JSON data from file
    QJsonDocument doc(QJsonDocument::fromJson(loadFile.readAll()));
    QJsonObject rootObj = doc.object();

    // Extract pixelSize and array from JSON
    int pixelSize = rootObj["pixelSize"].toInt();
    QJsonArray framesArray = rootObj["frames"].toArray();

    if (framesArray.isEmpty()) {
        return;
    }

    // Clear for incoming load
    clearFrames();

    // Create new Sprite and retrieve first frame from it
    sprite->createSprite(pixelSize);
    Frame *firstFrame = sprite->getSelectedFrame(0);

    // Load the data from JSON
    QJsonObject firstFrameObj = framesArray[0].toObject();
    QByteArray firstImageData = QByteArray::fromBase64(firstFrameObj["imageData"].toString().toUtf8());

    // Create a QImage from the image data
    QImage loadedImage;
    loadedImage.loadFromData(firstImageData, "PNG");

    firstFrame->setImage(loadedImage);
    firstFrame->update();
    firstFrame->show();
    firstFrame->raise();

    sprite->updateSelectedFrame(firstFrame);

    for (int i = 1; i < framesArray.size(); i++) {
        addFrame(sprite);
    }

    for (int i = 1; i < framesArray.size(); i++) {
        Frame *frame = sprite->getSelectedFrame(i);

        // Load the data from JSON
        QJsonObject FrameObj = framesArray[i].toObject();
        QByteArray ImageData =
            QByteArray::fromBase64(FrameObj["imageData"].toString().toUtf8());

        // Create a QImage from the image data
        QImage loadedImage;
        loadedImage.loadFromData(ImageData, "PNG");

        frame->setImage(loadedImage);
        frame->update();
        frame->show();
        frame->raise();
    }

    sprite->hideAllFrames();
    sprite->updateSelectedFrame(sprite->getFrameCount() - 1);
    sprite->getSelectedFrame(sprite->getSelectedFrameIndex())->show();
}

void MainWindow::on_zoomOut_clicked() {
    if (theZoomFactor > 10) {
        theZoomFactor -= 10;
    } else {
        theZoomFactor = 1;
    }
}

void MainWindow::on_zoomIn_clicked() {
    if (theZoomFactor < 1000) {
        theZoomFactor += 10;
    } else {
        theZoomFactor = 1000;
    }
}

void MainWindow::on_reset_clicked() {
    theZoomFactor = 200;
}

void MainWindow::updateUndoRedoButtons(Frame *frame) {
    ui->actionUndo->setEnabled(frame->canUndo());
    ui->actionRedo->setEnabled(frame->canRedo());
}

void MainWindow::on_help_clicked() {
    Help dialog(this);
    dialog.setWindowTitle("Help");
    dialog.exec();
}
