#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <iostream>
#include <QFileInfo>
#include <QThread>
#include "ui/mainwindow.h"
#include "util/ScreenshotUtil.h"
#include "ui/stylesheet.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet(stylesheet);

    // cmd app
    QCommandLineParser cmdParser;
    auto helpOption = cmdParser.addHelpOption();
    QCommandLineOption outputFileOption(
            "o", QCoreApplication::translate("main", "Output file or directory."),
            "output-file"
    );
    cmdParser.addOption(outputFileOption);
    QCommandLineOption delayOption(
            "d", QCoreApplication::translate("main", "Delay before creating a screenshot."),
            "delay"
    );
    cmdParser.addOption(delayOption);
    cmdParser.process(app);

    if(cmdParser.isSet(helpOption)) {
        return 0;
    } else if(cmdParser.isSet(outputFileOption)) {
        auto outFileName = cmdParser.value(outputFileOption);
        ScreenshotUtil screenshotUtil;
        int delay = cmdParser.value(delayOption).toInt();
        if(delay <= 0) {
            delay = 3000;
        }
        std::cout << "Screenshot of active window in" << delay << "ms ..." << std::endl;
        QThread::msleep(delay);
        auto windowImg = screenshotUtil.CaptureForegroundWindow();
        windowImg->save(outFileName);
        std::cout << outFileName.toStdString() << std::endl;
        return 0;
    }

    // gui app
    MainWindow mainWindow;
    mainWindow.show();

    return QApplication::exec();
}
