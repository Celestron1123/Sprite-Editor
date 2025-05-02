#include "frame.h"

Frame::Frame(int pixelSize, QWidget *parent)
    : QWidget{parent}, pixelSize(pixelSize)
{
    canvas = QImage(512, 512, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);
    color = Qt::black;
    currentTool = Freehand;
}

void Frame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, canvas);
    if (showGrid)
        drawGrid(painter);

    // Ensures user can see the shape as they drag across canvas
    if (drawingShape)
    {
        if (currentTool == Square)
        {
            painter.setPen(QPen(color, pixelSize, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
            QRect fixedShape = rect.adjusted(pixelSize / 2, pixelSize / 2, -pixelSize / 2, -pixelSize / 2);
            painter.drawRect(fixedShape);
        }
        else if (currentTool == Circle){
            drawCircle(painter);
        }
        else if (currentTool == Line)
            drawLine(painter);
        }

}

void Frame::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(Qt::gray, 1));
    int width = canvas.width();
    int height = canvas.height();

    for (int x = 0; x <= width; x += pixelSize)
        painter.drawLine(x, 0, x, height);

    for (int y = 0; y <= height; y += pixelSize)
        painter.drawLine(0, y, width, y);
}

bool Frame::isOnGridLine(int x, int y) const
{
    return (x % pixelSize == 0) || (y % pixelSize == 0);
}

void Frame::setPixelSize(int size)
{
    pixelSize = size;
}

void Frame::clear()
{
    // Update undo stack
    pushUndo();
    canvas = QImage(512, 512, QImage::Format_ARGB32);
    canvas.fill(Qt::transparent);
    pixels.clear();
    update();
}

QImage Frame::getImage()
{
    return canvas;
}

void Frame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int x = event->pos().x();
        int y = event->pos().y();

        if (blockDrawingOnGrid && isOnGridLine(x, y))
            return;

        QPainter painter(&canvas);
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);

        if (currentTool == Freehand)
        {
            // Update undo stack
            pushUndo();
            painter.drawRect((x / pixelSize) * pixelSize,
                             (y / pixelSize) * pixelSize,
                             pixelSize, pixelSize);
            emit frameChanged();
        }
        else if (currentTool == Square || currentTool == Circle)
        {
            drawingShape = true;
            start = QPoint((x / pixelSize) * pixelSize, (y / pixelSize) * pixelSize);
            rect = QRect(start, QSize(pixelSize, pixelSize));
        }
        else if(currentTool == Eraser) {
            // Update undo stack
            pushUndo();
            QRect rect((x / pixelSize) * pixelSize,
                       (y / pixelSize) * pixelSize,
                       pixelSize, pixelSize);
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.fillRect(rect, Qt::transparent);
        }
        else if (currentTool == Line) {
            drawingShape = true;
            start = QPoint((x / pixelSize) * pixelSize, (y / pixelSize) * pixelSize);
            rect = QRect(start, QSize(pixelSize, pixelSize));
        }

        update();
        pixels.append(Pixel(color.red(), color.green(), color.blue(), color.alpha(), x, y));
    }
}

void Frame::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int x = event->pos().x();
        int y = event->pos().y();

        if (blockDrawingOnGrid && isOnGridLine(x, y))
            return;

        QPainter painter(&canvas);
        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        QRect eraseRect(x / pixelSize * pixelSize, y / pixelSize * pixelSize, pixelSize, pixelSize);

        if (currentTool == Freehand)
        {
            painter.drawRect((x / pixelSize) * pixelSize,
                             (y / pixelSize) * pixelSize,
                             pixelSize, pixelSize);
        }
        else if (currentTool == Square || currentTool == Circle || currentTool == Line)
        {
            end = QPoint((x / pixelSize) * pixelSize, (y / pixelSize) * pixelSize);
            rect = QRect(start, end);
        }
        else if(currentTool == Eraser) {
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.fillRect(eraseRect, Qt::transparent);
        }

        update();
        pixels.append(Pixel(color.red(), color.green(), color.blue(), color.alpha(), x, y));
    }
}

void Frame::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawingShape)
    {
        // Update undo stack
        pushUndo();
        drawingShape = false;

        QPainter painter(&canvas);

        if (currentTool == Circle)
        {
            drawCircle(painter);
            emit frameChanged();
        }
        else if (currentTool == Square)
        {
            painter.setPen(QPen(color, pixelSize, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
            QRect fixedShape = rect.adjusted(pixelSize / 2, pixelSize / 2, -pixelSize / 2, -pixelSize / 2);
            painter.drawRect(fixedShape);
            emit frameChanged();
        }
        else if (currentTool == Line) {
            drawLine(painter);
            emit frameChanged();
        }

        update();
    }
}

void Frame::drawLine(QPainter &painter) {
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    int x0 = start.x() / pixelSize;
    int y0 = start.y() / pixelSize;
    int x1 = end.x() / pixelSize;
    int y1 = end.y() / pixelSize;

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        painter.drawRect(x0 * pixelSize, y0 * pixelSize, pixelSize, pixelSize);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Frame::drawCircle(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    // Get center and radius in pixels
    int x0 = (rect.left() + rect.right()) / 2 / pixelSize;
    int y0 = (rect.top() + rect.bottom()) / 2 / pixelSize;
    int radius = std::abs(rect.width()) / 2 / pixelSize;

    // Midpoint circle algorithm
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;

    auto drawBlock = [&](int px, int py)
    {
        painter.drawRect(px * pixelSize, py * pixelSize, pixelSize, pixelSize);
    };

    while (y <= x)
    {
        drawBlock(x0 + x, y0 + y);
        drawBlock(x0 + y, y0 + x);
        drawBlock(x0 - x, y0 + y);
        drawBlock(x0 - y, y0 + x);
        drawBlock(x0 - x, y0 - y);
        drawBlock(x0 - y, y0 - x);
        drawBlock(x0 + x, y0 - y);
        drawBlock(x0 + y, y0 - x);

        y++;
        if (decisionOver2 <= 0)
        {
            decisionOver2 += 2 * y + 1;
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
        }
    }
}

void Frame::setColor(QColor newColor)
{
    color = newColor;
}

void Frame::setImage(const QImage &img)
{
    canvas = img;
    update();
}

void Frame::setTool(Tool newTool)
{
    currentTool = newTool;
}

void Frame::pushUndo()
{
    undoStack.push(canvas.copy());
    redoStack.clear();
}
void Frame::undo()
{
    if (canUndo())
    {
        redoStack.push(canvas.copy());
        canvas = undoStack.pop();
        update();
    }
}
void Frame::redo()
{
    if (canRedo())
    {
        undoStack.push(canvas.copy());
        canvas = redoStack.pop();
        update();
    }
}
bool Frame::canUndo()
{
    return !undoStack.empty();
}
bool Frame::canRedo()
{
    return !redoStack.empty();
}
