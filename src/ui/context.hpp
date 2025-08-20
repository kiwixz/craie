#pragma once

#include <QImage>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

namespace craie::ui {

struct Context : QObject {
private:
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
};

}  // namespace craie::ui
