#include "ui/context.hpp"

#include <atomic>
#include <ranges>
#include <thread>

#include <boost/algorithm/string/replace.hpp>
#include <QString>

#include "common/assume.hpp"

namespace craie::ui {

bool Context::generating() const {
    return generating_.load(std::memory_order::relaxed) != Generating::no;
}

QString Context::systemPrompt() const {
    return QString::fromStdString(generator_.system_prompt());
}

QString Context::instructions() const {
    return QString::fromStdString(generator_.instructions());
}

QString Context::model() const {
    return QString::fromStdString(generator_.model());
}

QString Context::assistantPrefix() const {
    return QString::fromStdString(generator_.assistant_prefix());
}

int Context::contextSize() const {
    return generator_.context_size();
}

double Context::temperature() const {
    return generator_.temperature();
}

void Context::setSystemPrompt(const QString& system_prompt) {
    assume(!generating());
    generator_.set_system_prompt(system_prompt.toStdString());
}

void Context::setInstructions(const QString& instructions) {
    assume(!generating());
    generator_.set_instructions(instructions.toStdString());
}

void Context::setModel(const QString& model) {
    assume(!generating());
    generator_.set_model(model.toStdString());
}

void Context::setAssistantPrefix(const QString& assistant_prefix) {
    assume(!generating());

    std::string prefix = assistant_prefix.toStdString();
    boost::replace_all(prefix, "\\n", "\n");
    generator_.set_assistant_prefix(std::move(prefix));
}

void Context::setContextSize(int context_size) {
    assume(!generating());
    generator_.set_context_size(context_size);
}

void Context::setTemperature(double temperature) {
    assume(!generating());
    generator_.set_temperature(temperature);
}

std::vector<QString> Context::listModels() {
    return generator_.list_models() | std::ranges::to<std::vector<QString>>();
}

void Context::startGenerating(QString text) {
    assume(!generating());

    generating_.store(Generating::yes, std::memory_order::relaxed);
    emit generatingChanged();

    generating_thread_ = std::jthread{[this,
                                       text = std::move(text)] {
        generator_.generate(text.toStdString(), [&](const std::string_view text) {
            emit generatedText(QString::fromUtf8(text.data(), text.size()));
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
