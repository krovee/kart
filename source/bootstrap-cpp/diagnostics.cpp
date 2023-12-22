#include "diagnostics.h"
#include <sstream>
#include <utility>

namespace kart {
  std::string_view diagnostics::location::get_filename() const noexcept {
    return _filename;
  }
  
  size_t diagnostics::location::get_line_num() const noexcept {
    return _line;
  }

  size_t diagnostics::location::get_offset() const noexcept {
    return _offset;
  }

  std::string diagnostics::location::as_string() const noexcept {
    std::stringstream ss;
    ss << _filename << ':' << _line << ':' << _offset << ':';
    return ss.str();
  }

  diagnostics& diagnostics::operator+=(diagnostics const& r) noexcept {
    _messages.insert(_messages.end(), r._messages.begin(), r._messages.end());
    return *this;
  }

  diagnostics diagnostics::operator+(diagnostics const& r) const noexcept {
    return diagnostics(*this) += r;
  }

  diagnostics& diagnostics::operator+=(message &&msg) noexcept {
    _messages.emplace_back(std::move(msg));
    return *this;
  }

  void diagnostics::clear() noexcept {
    _messages.clear();
  }

  std::ostream& operator<<(std::ostream& os, const diagnostics& d) noexcept {
    // TODO: <diagnostics.cpp> - improve diagnostics printing!!!
    for (auto&& msg : d) {
      os << msg.loc.as_string() << msg.text << std::endl;
    }
    return os;
  }

}
