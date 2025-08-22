#pragma once

#include <atomic>
#include <cstdint>
#include <thread>

#include <QObject>
#include <QString>
#include <QtQmlIntegration>

#include "core/generator.hpp"

namespace craie::ui {

struct Context : QObject {
public:
    Q_PROPERTY(bool generating READ generating NOTIFY generatingChanged)

    bool generating() const;

    Q_INVOKABLE int contextSize() const;
    Q_INVOKABLE double temperature() const;
    Q_INVOKABLE QString instructions() const;
    Q_INVOKABLE QString systemPrompt() const;

    Q_INVOKABLE void setContextSize(int context_size);
    Q_INVOKABLE void setTemperature(double temperature);
    Q_INVOKABLE void setInstructions(const QString& instructions);
    Q_INVOKABLE void setSystemPrompt(const QString& system_prompt);

    Q_INVOKABLE void startGenerating(QString text);
    Q_INVOKABLE void stopGenerating();

signals:
    void generatingChanged();
    void generatedText(const QString& text);

private:
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    enum class Generating : uint8_t {
        no,
        yes,
        cancelled
    };

    core::Generator generator_;

    std::atomic<Generating> generating_ = Generating::no;
    std::jthread generating_thread_;
};

}  // namespace craie::ui
