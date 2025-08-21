#pragma once

#include <QObject>
#include <QtQmlIntegration>

namespace craie::ui {

struct Context : QObject {
private:
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
};

}  // namespace craie::ui
