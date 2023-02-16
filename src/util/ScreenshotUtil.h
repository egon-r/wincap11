//
// Created by egonr on 2/13/2023.
//

#ifndef WINCAP11_SCREENSHOTUTIL_H
#define WINCAP11_SCREENSHOTUTIL_H

#include <QRect>
#include <QImage>

enum CropStyle {
    WINDOWS_11,
    NONE
};

class ScreenshotUtil {
public:
    QImage *CaptureForegroundWindow(CropStyle cropStyle = WINDOWS_11);

private:
    /*!
     * @return First value contains the geometry of foreground window.\n\n
     * Second value indicates maximized status of the foreground window.
     */
    QPair<QRect, bool> GetForegroundWindowRect(CropStyle cropStyle = WINDOWS_11);

    QImage *ScreenshotRect(QRect windowRect);

    QImage *CropCorners(QImage *img, CropStyle cropStyle = WINDOWS_11);

    QImage *CropEdges(QImage *img, CropStyle cropStyle = WINDOWS_11);

};


#endif //WINCAP11_SCREENSHOTUTIL_H
