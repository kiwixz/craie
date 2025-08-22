#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include <boost/asio/io_context.hpp>
#include <boost/compat/function_ref.hpp>

namespace craie::core {

struct Generator {
    const std::string& system_prompt() const;
    const std::string& instructions() const;
    const std::string& model() const;
    const std::string& assistant_prefix() const;
    size_t context_size() const;
    double temperature() const;

    void set_system_prompt(std::string system_prompt);
    void set_instructions(std::string instructions);
    void set_model(std::string model);
    void set_assistant_prefix(std::string assistant_prefix);
    void set_context_size(size_t context_size);
    void set_temperature(double temperature);

    std::vector<std::string> list_models();
    void generate(std::string_view text, boost::compat::function_ref<bool(std::string_view text)> callback);

private:
    std::string system_prompt_;
    std::string instructions_;
    std::string model_;
    std::string assistant_prefix_;
    size_t context_size_ = 4096;
    double temperature_ = 0.8;

    boost::asio::io_context io_{BOOST_ASIO_CONCURRENCY_HINT_UNSAFE};
};

}  // namespace craie::core
