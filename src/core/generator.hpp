#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include <boost/compat/function_ref.hpp>

namespace craie::core {

struct Generator {
    size_t context_size() const;
    double temperature() const;
    const std::string& instructions() const;
    const std::string& system_prompt() const;

    void set_context_size(size_t context_size);
    void set_temperature(double temperature);
    void set_instructions(std::string instructions);
    void set_system_prompt(std::string system_prompt);

    void generate(std::string_view text, boost::compat::function_ref<bool(std::string&& text)> callback);

private:
    size_t context_size_ = 4096;
    double temperature_ = 0.8;
    std::string instructions_;
    std::string system_prompt_;
};

}  // namespace craie::core
