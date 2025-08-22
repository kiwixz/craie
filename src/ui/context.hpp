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
    Q_PROPERTY(int contextSize READ context_size WRITE set_context_size)
    Q_PROPERTY(double temperature READ temperature WRITE set_temperature)
    Q_PROPERTY(QString instructions READ instructions WRITE set_instructions)
    Q_PROPERTY(QString systemPrompt READ system_prompt WRITE set_system_prompt)

    bool generating() const;
    int context_size() const;
    double temperature() const;
    QString instructions() const;
    QString system_prompt() const;

    void set_context_size(int context_size);
    void set_temperature(double temperature);
    void set_instructions(const QString& instructions);
    void set_system_prompt(const QString& system_prompt);

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
