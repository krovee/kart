#pragma once 

#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <map>

namespace kart {
  class diagnostics {
  public:
    class location {
    public:
      location() noexcept = default;
      ~location() = default;

      location(const std::string& fname, size_t line = 0, size_t offset = 0) noexcept
        : _filename(fname), _line(line), _offset(offset)
      {}

      std::string_view get_filename() const noexcept;
      size_t get_line_num() const noexcept;
      size_t get_offset() const noexcept;

      std::string as_string() const noexcept;

    private:
      std::string _filename = {};
      size_t _line = 0;
      size_t _offset = {};
    };

    struct message {
      location loc = {};
      std::string text = {};
    };

    diagnostics() noexcept = default;
    ~diagnostics() = default;

    diagnostics& operator+=(diagnostics const& r) noexcept;
    diagnostics  operator+ (diagnostics const& r) const noexcept;
    diagnostics& operator+=(message &&msg) noexcept;

    void clear() noexcept;

    auto begin() const noexcept {
      return _messages.cbegin();
    }
    auto end() const noexcept {
      return _messages.cend();
    }
  private:
    std::vector<message> _messages = {};
  };

  std::ostream& operator<<(std::ostream& os, const diagnostics& d) noexcept;

}

namespace std {
  template<>
  struct hash<kart::diagnostics::location> {
    inline size_t operator()(kart::diagnostics::location const& __location) const noexcept {
      return hash<std::string_view>()(__location.get_filename())  
        + __location.get_line_num()  + __location.get_offset();
    }
  };
}
