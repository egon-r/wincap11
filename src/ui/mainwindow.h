//
// Created by egonr on 2/11/2023.
//

#ifndef WINCAP11_MAINWINDOW_H
#define WINCAP11_MAINWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>
#include <QGraphicsPixmapItem>
#include "qpictureview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QTimer captureTimer;
    QSharedPointer<QPictureView> pictureView;
    QDateTime currentScreenshotTime;
    QSharedPointer<QImage> currentScreenshot;
    QSharedPointer<QGraphicsPixmapItem> graphicsItem;


private:
    void SetUIStateCapturing();
    void SetUIStateCaptureFinished();
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleCaptureButton();
    void captureTimerTimeout();
    void handleSaveButton();
};


#endif //WINCAP11_MAINWINDOW_H
