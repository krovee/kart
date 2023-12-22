#pragma once

#include "token.h"
#include <cstdlib>

namespace kart {
  class tokenizer {
  public:
    using tokens_stream = std::vector<token>;

    enum max_diagnostics_level : size_t {
      kNone,
      kWarning,
      kError
    };

    tokenizer(std::string const& __filename, std::string const& __text) noexcept 
      : _filename(__filename), _text(__text), _diagnostics(), _curr_line()
    {
      _current = _text.begin();
    }

    ~tokenizer() = default;

    auto get_diagnostics() const noexcept {
      return _diagnostics;
    }

    max_diagnostics_level tokenize(tokens_stream& __tokens) noexcept;
  private:
    void add_message(std::string const& msg) noexcept;
  private:
    diagnostics _diagnostics = {};
    std::string const& _filename;
    std::string const& _text;
    std::string::const_iterator _current = {};
    struct {
      size_t num = 1;
      size_t offset = 0;
    } _curr_line = {};
  };
}
