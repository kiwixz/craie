#include "core/generator.hpp"

#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/compat/function_ref.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value.hpp>

#include "common/exception.hpp"

namespace craie::core {

const std::string& Generator::system_prompt() const {
    return system_prompt_;
}

const std::string& Generator::instructions() const {
    return instructions_;
}

const std::string& Generator::model() const {
    return model_;
}

const std::string& Generator::assistant_prefix() const {
    return assistant_prefix_;
}

size_t Generator::context_size() const {
    return context_size_;
}

double Generator::temperature() const {
    return temperature_;
}

void Generator::set_system_prompt(std::string system_prompt) {
    system_prompt_ = std::move(system_prompt);
}

void Generator::set_instructions(std::string instructions) {
    instructions_ = std::move(instructions);
}

void Generator::set_model(std::string model) {
    model_ = std::move(model);
}

void Generator::set_assistant_prefix(std::string assistant_prefix) {
    assistant_prefix_ = std::move(assistant_prefix);
}

void Generator::set_context_size(size_t context_size) {
    context_size_ = context_size;
}

void Generator::set_temperature(double temperature) {
    temperature_ = temperature;
}

std::vector<std::string> Generator::list_models() {
    boost::asio::ip::tcp::socket socket{io_};
    socket.connect({boost::asio::ip::make_address_v4("127.0.0.1"), 11434});

    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, "/api/tags", 11};
    req.set(boost::beast::http::field::host, "127.0.0.1");
    boost::beast::http::write(socket, req);

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::string_body> res;
    boost::beast::http::read(socket, buffer, res);
    if (res.result() != boost::beast::http::status::ok)
        throw Exception{"Could not list models: HTTP status {} {}", static_cast<int>(res.result()), std::string_view{res.reason()}};

    return boost::json::parse(res.body()).at("models").as_array()
           | std::views::transform([&](const boost::json::value& model) {
                 return std::string{model.at("name").as_string()};
             })
           | std::ranges::to<std::vector>();
}

void Generator::generate(std::string_view text, boost::compat::function_ref<bool(std::string_view text)> callback) {
    boost::json::array messages = {{{"role", "system"},
                                    {"content", system_prompt_}}};

    std::string_view remaining_text = text;
    while (!remaining_text.empty()) {
        size_t message_end = remaining_text.find("\n\n");
        if (message_end == std::string_view::npos)
            break;

        std::string_view line = remaining_text.substr(0, message_end);
        messages.push_back({{"role", "user"},
                            {"content", line}});

        remaining_text = remaining_text.substr(message_end + 2);
    }

    messages.push_back({{"role", "user"},
                        {"content", instructions_}});

    if (!assistant_prefix_.empty() || !remaining_text.empty()) {
        messages.push_back({{"role", "assistant"},
                            {"content", std::format("{}{}", assistant_prefix_, remaining_text)}});
    }

    boost::asio::ip::tcp::socket socket{io_};
    socket.connect({boost::asio::ip::make_address_v4("127.0.0.1"), 11434});

    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::post, "/api/chat", 11};
    req.set(boost::beast::http::field::host, "127.0.0.1");
    req.body() = boost::json::serialize(boost::json::object{{"model", model_},
                                                            {"messages", std::move(messages)},
                                                            {"think", false},
                                                            {"options",
                                                             {{"num_ctx", context_size_},
                                                              {"temperature", temperature_}}}});
    req.prepare_payload();
    boost::beast::http::write(socket, req);

    boost::beast::flat_buffer buffer;
    boost::beast::http::response_parser<boost::beast::http::string_body> parser;
    boost::beast::http::read_header(socket, buffer, parser);
    if (parser.get().result() != boost::beast::http::status::ok)
        throw Exception{"Could not generate text: HTTP status {} {}", static_cast<int>(parser.get().result()), std::string_view{parser.get().reason()}};

    while (!parser.is_done()) {
        boost::beast::http::read_some(socket, buffer, parser);

        std::string_view body = parser.get().body();
        size_t eol = body.find('\n');
        if (eol == std::string_view::npos)
            continue;

        boost::json::value json = boost::json::parse(body.substr(0, eol));
        if (json.at("done").as_bool())
            break;

        if (!callback(json.at("message").at("content").as_string()))
            break;

        parser.get().body().erase(0, eol + 1);
    }
}

}  // namespace craie::core
