#include "ui/context.hpp"

#include <atomic>
#include <cstddef>
#include <string>
#include <thread>

#include <QString>

#include "common/assume.hpp"

namespace craie::ui {

bool Context::generating() const {
    return generating_.load(std::memory_order::relaxed) != Generating::no;
}

int Context::context_size() const {
    return generator_.context_size();
}

double Context::temperature() const {
    return generator_.temperature();
}

QString Context::instructions() const {
    return QString::fromStdString(generator_.instructions());
}

QString Context::system_prompt() const {
    return QString::fromStdString(generator_.system_prompt());
}

void Context::set_context_size(int context_size) {
    assume(!generating());
    generator_.set_context_size(context_size);
}

void Context::set_temperature(double temperature) {
    assume(!generating());
    generator_.set_temperature(temperature);
}

void Context::set_instructions(const QString& instructions) {
    assume(!generating());
    generator_.set_instructions(instructions.toStdString());
}

void Context::set_system_prompt(const QString& system_prompt) {
    assume(!generating());
    generator_.set_system_prompt(system_prompt.toStdString());
}

void Context::startGenerating(QString text) {
    assume(!generating());

    generating_.store(Generating::yes, std::memory_order::relaxed);
    emit generatingChanged();

    generating_thread_ = std::jthread{[this,
                                       text = std::move(text)] {
        generator_.generate(text.toStdString(), [&](const std::string& text) {
            emit generatedText(QString::fromStdString(text));
            return generating_.load(std::memory_order::relaxed) == Generating::yes;
        });

        generating_.store(Generating::no, std::memory_order::relaxed);
        emit generatingChanged();
    }};
}

void Context::stopGenerating() {
    generating_.store(Generating::cancelled, std::memory_order::relaxed);
}

}  // namespace craie::ui
