//
// Created by egonr on 2/14/2023.
//

#ifndef WINCAP11_QPICTUREVIEW_H
#define WINCAP11_QPICTUREVIEW_H

#include <QLabel>

class QPictureView : public QLabel {
Q_OBJECT

public:
    explicit QPictureView(QWidget *parent = nullptr);
    ~QPictureView() override;
    void SetImage(QImage *image);
    void SetPixmap(QPixmap *pixmap);

private:
    QPixmap sourcePixmap;

private:
    void paintEvent(QPaintEvent *event) override;

};


#endif //WINCAP11_QPICTUREVIEW_H
