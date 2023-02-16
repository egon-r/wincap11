//
// Created by egonr on 2/14/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QPictureView.h" resolved

#include <QPainter>
#include "qpictureview.h"


QPictureView::QPictureView(QWidget *parent) : QLabel(parent) {
    setAlignment(Qt::AlignmentFlag::AlignCenter);
}


QPictureView::~QPictureView() {
}

void QPictureView::paintEvent(QPaintEvent *event) {
    setGeometry(0,0,parentWidget()->width(), parentWidget()->height());

    QPainter painter;
    painter.begin(this);
    painter.fillRect(rect(), Qt::black);

    if(!sourcePixmap.isNull()) {
        int scaleWidth = this->width();
        int scaleHeight = this->height();
        if(this->width() > sourcePixmap.width()) {
            scaleWidth = sourcePixmap.width();
        }
        if(this->height() > sourcePixmap.height()) {
            scaleHeight = sourcePixmap.height();
        }
        setPixmap(sourcePixmap.scaled(scaleWidth, scaleHeight, Qt::KeepAspectRatio));
    } else {
        setText("...");
    }

    QLabel::paintEvent(event);
}

void QPictureView::SetImage(QImage *image) {
    QPictureView::SetPixmap(new QPixmap(QPixmap::fromImage(*image)));
}

void QPictureView::SetPixmap(QPixmap *pixmap) {
    sourcePixmap = *pixmap;
    QLabel::setPixmap(sourcePixmap);
}

