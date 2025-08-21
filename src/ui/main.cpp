#include <exception>
#include <format>

#include <QGuiApplication>
#include <QMessageLogContext>
#include <QQuickView>
#include <QString>
#include <QtLogging>

#include "common/assume.hpp"
#include "common/log.hpp"

int main(int argc, char** argv) {
    using namespace craie;

    try {
        qInstallMessageHandler([](QtMsgType level_qt, const QMessageLogContext& /*from*/, const QString& message) {
            log::Level level = [&] {
                switch (level_qt) {
                case QtDebugMsg: return log::Level::debug;
                case QtInfoMsg: return log::Level::info;
                case QtWarningMsg: return log::Level::warning;
                case QtCriticalMsg:
                case QtFatalMsg: return log::Level::error;
                }
                assume(false);
                return log::Level::debug;
            }();
            log::log(level, "Qt: {}", message.toStdString());
        });

        QGuiApplication app{argc, argv};
        QQuickView window;

        window.setSource({"qrc:qt/qml/craie/main.qml"});
        if (window.status() != QQuickView::Ready)
            return 1;

        window.setMinimumSize(window.initialSize());
        window.setResizeMode(QQuickView::SizeRootObjectToView);
        window.resize(1280, 720);

        window.show();
        return app.exec();
    }
    catch (const std::exception& ex) {
        log::error("{}", ex.what());
        return 1;
    }
}
