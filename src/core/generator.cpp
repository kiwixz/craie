#include "core/generator.hpp"

#include <chrono>
#include <cstddef>
#include <string>
#include <string_view>
#include <thread>

#include <boost/compat/function_ref.hpp>

namespace craie::core {

size_t Generator::context_size() const {
    return context_size_;
}

double Generator::temperature() const {
    return temperature_;
}

const std::string& Generator::instructions() const {
    return instructions_;
}

const std::string& Generator::system_prompt() const {
    return system_prompt_;
}

void Generator::set_context_size(size_t context_size) {
    context_size_ = context_size;
}

void Generator::set_temperature(double temperature) {
    temperature_ = temperature;
}

void Generator::set_instructions(std::string instructions) {
    instructions_ = std::move(instructions);
}

void Generator::set_system_prompt(std::string system_prompt) {
    system_prompt_ = std::move(system_prompt);
}

void Generator::generate(std::string_view text, boost::compat::function_ref<bool(std::string&& text)> callback) {
    (void)text;

    while (true) {
        if (!callback("hello\n"))
            break;

        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

}  // namespace craie::core
