#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <sstream>
#include <unordered_map>
#include <string_view>
#include <variant>
#include "../diagnostics.h"

namespace kart {
  class token {
  public:
    enum kind : uint32_t {
      kIdentidier,  // a-zA-Z
      kDecDigits,   // 0-9
      kHexDigits,   // 0x0-0xf
      kBadToken,    // BAD TOKEN, NOT RECOGNIZED~!!!
      kSpace,       // ' '
      kTab,         // '  '
      kNewline,     // \n
      kApostrophe,  // `
      kTilde,       // ~
      kBang,        // !
      kCommat,      // @
      kHash,        // #
      kNumero,      // №
      kDollarSign,  // $
      kPercent,     // %
      kRoofsign,    // ^
      kAmpersand,   // &
      kStar,        // *
      kParenBegin,  // (
      kParenEnd,    // )
      kHyphen,      // -
      kLowLine,     // _
      kEquals,      // =
      kPlus,        // +
      kSquareBegin, // [
      kSquareEnd,   // ]
      kCurlyBegin,  // {
      kCurlyEnd,    // }
      kBackSlash,   // '\'
      kStraight,    // |
      kSemicolon,   // ;
      kColon,       // :
      kSingleQuot,  // '
      kDoubleQuot,  // "
      kComma,       // ,
      kDot,         // .
      kAngleBegin,  // <
      kAngleEnd,    // >
      kSolidus,     // /
      kQuestion,    // ?      
    };

    token() noexcept = default;
    ~token() = default;

    inline token(kind __kind, std::string const& __filename, size_t __line, size_t __offset) noexcept 
      : _kind(__kind), _location(__filename, __line, __offset)
    {}
    inline token(kind __kind, std::string const& __text, std::string const& __filename, size_t __line, size_t __offset) noexcept 
      : _kind(__kind), _rep(__text), _location(__filename, __line, __offset)
    {}
    inline token(kind __kind, size_t __hex, std::string const& __filename, size_t __line, size_t __offset) noexcept 
      : _kind(__kind), _rep(__hex), _location(__filename, __line, __offset)
    {}
    inline token(kind __kind, int64_t __dec, std::string const& __filename, size_t __line, size_t __offset) noexcept 
      : _kind(__kind), _rep(__dec), _location(__filename, __line, __offset)
    {}

    class character_glue {
    public:
      constexpr character_glue() noexcept = default;
      ~character_glue() noexcept = default;

      constexpr character_glue(const char __char) noexcept
        : _char(__char)
      {}

      constexpr operator char() const noexcept {
        return static_cast<char>(_char);
      }

      constexpr auto raw() const noexcept {
        return _char;
      }

    private:
      const int _char = {};
    };

    static const token make_punctuation(character_glue c,         std::string const& __filename, size_t __line, size_t __offset) noexcept;
    static const token make_identifier(std::string const& __text, std::string const& __filename, size_t __line, size_t __offset) noexcept;
    static const token make_hex_number(size_t __hex,              std::string const& __filename, size_t __line, size_t __offset) noexcept;
    static const token make_dec_number(int64_t __dec,             std::string const& __filename, size_t __line, size_t __offset) noexcept;
    static const token make_dec_number(size_t __dec,              std::string const& __filename, size_t __line, size_t __offset) noexcept;

    inline bool is_bad() const noexcept {
      return _kind == kBadToken;
    }

    inline operator kind() const noexcept {
      return _kind;
    }

    bool has_rep() const noexcept;
    bool has_text_rep() const noexcept;
    bool has_hex_rep() const noexcept;
    bool has_dec_rep() const noexcept;

    std::optional<std::string> get_text_rep() const noexcept {
      if (has_text_rep()) {
        return std::get<std::string>(_rep.value());
      }
      return {};
    }
    std::optional<size_t> get_hex_rep() const noexcept {
      if (has_hex_rep()) {
        return std::get<size_t>(_rep.value());
      }
      return {};
    }
    std::optional<int64_t> get_dec_rep() const noexcept {
      if (has_hex_rep()) {
        return std::get<int64_t>(_rep.value());
      }
      return {};
    }

    inline auto get_location() const noexcept {
      return _location;
    }
  private:
    kind _kind = token::kIdentidier;
    std::optional<std::variant<std::string, size_t, int64_t>> _rep = std::nullopt;
    diagnostics::location _location = {};
  };
}

namespace std {
  template<>
  struct hash<kart::token> {
    inline size_t operator()(kart::token const& t) const noexcept {
      const auto __location_hash = hash<kart::diagnostics::location>()(t.get_location());
      if (const auto __text = t.get_text_rep(); __text) {
        return hash<std::string>()(*__text) + __location_hash;
      }
      else if (const auto __hex = t.get_hex_rep(); __hex) {
        return __location_hash + *__hex;
      }
      else if (const auto __dec = t.get_dec_rep(); __dec) {
        return __location_hash + *__dec;
      }
      else {
        return __location_hash;
      }
    }
  };
}
