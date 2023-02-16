//
// Created by egonr on 2/13/2023.
//

#include "ScreenshotUtil.h"
#include <QGuiApplication>
#include <QPainter>
#include <QScreen>
#include "windows.h"

QImage *ScreenshotUtil::CaptureForegroundWindow(CropStyle cropStyle) {
    auto windowRect = GetForegroundWindowRect(cropStyle);
    if (windowRect.second) {
        return ScreenshotRect(windowRect.first);
    } else {
        auto img = ScreenshotRect(windowRect.first);
        return CropCorners(CropEdges(img, cropStyle), cropStyle);
    }
}

QPair<QRect, bool> ScreenshotUtil::GetForegroundWindowRect(CropStyle cropStyle) {
    HWND hwnd = GetForegroundWindow();

    WINDOWPLACEMENT hwndPlacement;
    GetWindowPlacement(hwnd, &hwndPlacement);
    bool isMaximized = hwndPlacement.showCmd == SW_MAXIMIZE;

    RECT hwndRect;
    GetWindowRect(hwnd, &hwndRect);
    QRect hwndQRect(hwndRect.left, hwndRect.top,
                    abs(hwndRect.right - hwndRect.left),
                    abs(hwndRect.bottom - hwndRect.top)
    );

    if (isMaximized) {
        if (cropStyle == WINDOWS_11) {
            hwndQRect = QRect(hwndRect.left + 8, hwndRect.top + 8,
                              abs(hwndRect.right - hwndRect.left - 16),
                              abs(hwndRect.bottom - hwndRect.top - 16)
            );
        }
    }

    return {hwndQRect, isMaximized};
}

QImage *ScreenshotUtil::ScreenshotRect(QRect windowRect) {
    auto windowImg = new QImage(windowRect.width(), windowRect.height(), QImage::Format_ARGB32);
    QPainter painter(windowImg);
    auto screens = QGuiApplication::screens();
    for (auto screen: screens) {
        if (screen->geometry().intersects(windowRect)) {
            auto intersectRect = screen->geometry().intersected(windowRect);
            auto intersectPix = screen->grabWindow(0,
                                                   abs(intersectRect.left() - screen->geometry().left()),
                                                   abs(intersectRect.top() - screen->geometry().top()),
                                                   intersectRect.width(), intersectRect.height()
            );
            painter.drawPixmap(intersectRect.left() - windowRect.left(),
                               intersectRect.top() - windowRect.top(),
                               intersectPix
            );
        }
    }
    return windowImg;
}

QImage *ScreenshotUtil::CropEdges(QImage *img, CropStyle cropStyle) {
    return new QImage(img->copy(7, 0, img->width() - 14, img->height() - 7));
}

QImage *ScreenshotUtil::CropCorners(QImage *img, CropStyle cropStyle) {
    // opacity map for antialiased border (0-100%)
    // 00, 00, 00, 00, 25, 56, 73
    // 00, 00, 00, 48
    // 00, 00, 67
    // 00, 48
    // 25
    // 56
    // 73
    QPainter painter(img);

    // 1. remove corner and add fully transparent pixels
    painter.setBrush(Qt::NoBrush);
    painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Clear);
    painter.setPen(Qt::transparent);
    // top left
    painter.drawLine(0, 0, 6, 0);
    painter.drawLine(0, 0, 0, 6);
    painter.drawLine(0, 1, 3, 1);
    painter.drawLine(0, 2, 2, 2);
    painter.drawLine(0, 3, 1, 3);
    // bot left
    painter.drawLine(0, img->height() - 7, 0, img->height() - 1);
    painter.drawLine(0, img->height() - 1, 6, img->height() - 1);
    painter.drawLine(0, img->height() - 2, 3, img->height() - 2);
    painter.drawLine(0, img->height() - 3, 2, img->height() - 3);
    painter.drawLine(0, img->height() - 4, 1, img->height() - 4);
    // top right
    painter.drawLine(img->width() - 7, 0, img->width() - 1, 0);
    painter.drawLine(img->width() - 1, 0, img->width() - 1, 6);
    painter.drawLine(img->width() - 4, 1, img->width() - 1, 1);
    painter.drawLine(img->width() - 3, 2, img->width() - 1, 2);
    painter.drawLine(img->width() - 2, 3, img->width() - 1, 3);
    // bot right
    painter.drawLine(img->width() - 1, img->height() - 1, img->width() - 7, img->height() - 1);
    painter.drawLine(img->width() - 1, img->height() - 1, img->width() - 1, img->height() - 7);
    painter.drawLine(img->width() - 1, img->height() - 2, img->width() - 4, img->height() - 2);
    painter.drawLine(img->width() - 1, img->height() - 3, img->width() - 3, img->height() - 3);
    painter.drawLine(img->width() - 1, img->height() - 4, img->width() - 2, img->height() - 4);

    // 2. add partially transparent pixels
    auto line = reinterpret_cast<QRgb *>(img->scanLine(0));
    auto baseColor = line[10];
    auto bc25 = qRgba(qRed(baseColor), qGreen(baseColor), qBlue(baseColor), 0xFF * 0.25);
    auto bc56 = qRgba(qRed(baseColor), qGreen(baseColor), qBlue(baseColor), 0xFF * 0.56);
    auto bc73 = qRgba(qRed(baseColor), qGreen(baseColor), qBlue(baseColor), 0xFF * 0.73);
    auto bc48 = qRgba(qRed(baseColor), qGreen(baseColor), qBlue(baseColor), 0xFF * 0.48);
    auto bc67 = qRgba(qRed(baseColor), qGreen(baseColor), qBlue(baseColor), 0xFF * 0.67);

    // top left
    img->setPixel(4, 0, bc25);
    img->setPixel(5, 0, bc56);
    img->setPixel(6, 0, bc73);

    img->setPixel(3, 1, bc48);
    img->setPixel(2, 2, bc67);
    img->setPixel(1, 3, bc48);

    img->setPixel(0, 4, bc25);
    img->setPixel(0, 5, bc56);
    img->setPixel(0, 6, bc73);

    // bot left
    img->setPixel(4, img->height() - 1, bc25);
    img->setPixel(5, img->height() - 1, bc56);
    img->setPixel(6, img->height() - 1, bc73);

    img->setPixel(3, img->height() - 2, bc48);
    img->setPixel(2, img->height() - 3, bc67);
    img->setPixel(1, img->height() - 4, bc48);

    img->setPixel(0, img->height() - 5, bc25);
    img->setPixel(0, img->height() - 6, bc56);
    img->setPixel(0, img->height() - 7, bc73);

    // top right
    img->setPixel(img->width() - 5, 0, bc25);
    img->setPixel(img->width() - 6, 0, bc56);
    img->setPixel(img->width() - 7, 0, bc73);

    img->setPixel(img->width() - 4, 1, bc48);
    img->setPixel(img->width() - 3, 2, bc67);
    img->setPixel(img->width() - 2, 3, bc48);

    img->setPixel(img->width() - 1, 4, bc25);
    img->setPixel(img->width() - 1, 5, bc56);
    img->setPixel(img->width() - 1, 6, bc73);

    // bot right
    img->setPixel(img->width() - 5, img->height() - 1, bc25);
    img->setPixel(img->width() - 6, img->height() - 1, bc56);
    img->setPixel(img->width() - 7, img->height() - 1, bc73);

    img->setPixel(img->width() - 4, img->height() - 2, bc48);
    img->setPixel(img->width() - 3, img->height() - 3, bc67);
    img->setPixel(img->width() - 2, img->height() - 4, bc48);

    img->setPixel(img->width() - 1, img->height() - 5, bc25);
    img->setPixel(img->width() - 1, img->height() - 6, bc56);
    img->setPixel(img->width() - 1, img->height() - 7, bc73);

    return img;
}