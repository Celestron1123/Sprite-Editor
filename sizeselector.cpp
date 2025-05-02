#include "sizeselector.h"
// Checked by Ryan Hamerman, Steven Luo, and Elijah Potter

#include "ui_sizeselector.h"

SizeSelector::SizeSelector(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SizeSelector)
{
    ui->setupUi(this);
    connect(ui->button8, &QPushButton::clicked, this, &SizeSelector::handleSizeSelection);
    connect(ui->button16, &QPushButton::clicked, this, &SizeSelector::handleSizeSelection);
    connect(ui->button32, &QPushButton::clicked, this, &SizeSelector::handleSizeSelection);
    connect(ui->button64, &QPushButton::clicked, this, &SizeSelector::handleSizeSelection);
    connect(ui->button128, &QPushButton::clicked, this, &SizeSelector::handleSizeSelection);
}

SizeSelector::~SizeSelector()
{
    delete ui;
}

void SizeSelector::handleSizeSelection()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) {
        return;
    }
    int calculatedPixelSize = 0;
    if (clickedButton == ui->button8) {
        calculatedPixelSize = 64;
    } else if (clickedButton == ui->button16) {
        calculatedPixelSize = 32;
    } else if (clickedButton == ui->button32) {
        calculatedPixelSize = 16;
    } else if (clickedButton == ui->button64) {
        calculatedPixelSize = 8;
    } else if (clickedButton == ui->button128) {
        calculatedPixelSize = 4;
    }
    emit sizeSelected(calculatedPixelSize);
    close();
}
