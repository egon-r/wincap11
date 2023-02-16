//
// Created by egonr on 2/11/2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "windows.h"
#include "../util/ScreenshotUtil.h"
#include "qpictureview.h"
#include <QScreen>
#include <QPainter>
#include <QRgb>
#include <QColorSpace>
#include <QDateTime>
#include <QFileDialog>
#include <QBuffer>
#include "util/Version.h"

MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle(QString("wincap11 (v%1.%2-%3)").arg(wincap11_VERSION_MAJOR).arg(wincap11_VERSION_MINOR).arg(wincap11_GIT_HASH));

    pictureView = QSharedPointer<QPictureView>(new QPictureView(ui->pictureViewContainer));
    pictureView->show();

    connect(ui->captureButton, SIGNAL(clicked()), this, SLOT(handleCaptureButton()));
    connect(&captureTimer, SIGNAL(timeout()), this, SLOT(captureTimerTimeout()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(handleSaveButton()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleSaveButton() {
    QString suggestedName = "Screenshot_" + currentScreenshotTime.toString("yyyyMMdd_HHmmss_zzz") + ".png";
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), suggestedName);
    currentScreenshot->save(saveFileName);
}

void MainWindow::handleCaptureButton() {
    SetUIStateCapturing();
    this->captureTimer.start(ui->delayBox->value());
}

void MainWindow::captureTimerTimeout() {
    ScreenshotUtil screenshotUtil;
    currentScreenshot = QSharedPointer<QImage>(screenshotUtil.CaptureForegroundWindow(WINDOWS_11));
    currentScreenshotTime = QDateTime::currentDateTime();
    pictureView->SetImage(currentScreenshot.get());

    this->captureTimer.stop();
    ui->delayBox->setValue(captureTimer.interval());

    SetUIStateCaptureFinished();
}

void MainWindow::SetUIStateCapturing() {
    ui->leftStatusLabel->setText("Capturing...");
    ui->rightStatusLabel->setText("");
    ui->captureButton->setEnabled(false);
}

void MainWindow::SetUIStateCaptureFinished() {
    // window to front
    raise();
    activateWindow();

    QBuffer tmpBuffer;
    currentScreenshot->save(&tmpBuffer, "PNG");

    ui->leftStatusLabel->setText(
            QString("%1 x %2").arg(currentScreenshot->width()).arg(currentScreenshot->height())
    );
    ui->rightStatusLabel->setText(
            QString("%1 (PNG)").arg(locale().formattedDataSize(tmpBuffer.size(), 1))
    );
    ui->captureButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->saveButton->setFocus();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if(this->captureTimer.isActive()) {
        ui->delayBox->setValue(this->captureTimer.remainingTime());
    }
    QWidget::paintEvent(event);
}