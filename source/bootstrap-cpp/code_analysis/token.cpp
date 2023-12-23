#include "token.h"

namespace kart::code_analysis {
  namespace {
    token::kind __char_to_kind(token::character_glue c) noexcept {
      switch (c) {
        case ' ':   return token::kSpace;
        case '\t':  return token::kTab;
        case '\n':  return token::kNewline;
        case '`':   return token::kApostrophe;
        case '~':   return token::kTilde;
        case '!':   return token::kBang;
        case '@':   return token::kCommat;
        case '#':   return token::kHash;
        case '$':   return token::kDollarSign;
        case '%':   return token::kPercent;
        case '^':   return token::kRoofsign;
        case '&':   return token::kAmpersand;
        case '*':   return token::kStar;
        case '(':   return token::kParenBegin;
        case ')':   return token::kParenEnd;
        case '-':   return token::kHyphen;
        case '_':   return token::kLowLine;
        case '=':   return token::kEquals;
        case '+':   return token::kPlus;
        case '[':   return token::kSquareBegin;
        case ']':   return token::kSquareEnd;
        case '{':   return token::kCurlyBegin;
        case '}':   return token::kCurlyEnd;
        case '\\':  return token::kBackSlash;
        case '|':   return token::kStraight;
        case ';':   return token::kSemicolon;
        case ':':   return token::kColon;
        case '\'':  return token::kSingleQuot;
        case '"':   return token::kDoubleQuot;
        case ',':   return token::kComma;
        case '.':   return token::kDot;
        case '<':   return token::kAngleBegin;
        case '>':   return token::kAngleEnd;
        case '/':   return token::kSolidus;
        case '?':   return token::kQuestion;

        default: break;
      }
      
      switch (c.raw()) {
        case L'№':  return token::kNumero;
      }
      
      return token::kBadToken;
    }
  }

  const token token::make_punctuation(token::character_glue c, std::string const& __filename, size_t __line, size_t __offset) noexcept {
    return token(__char_to_kind(c), __filename, __line, __offset);
  }

  const token token::make_identifier(const std::string &__text, const std::string &__filename, size_t __line, size_t __offset) noexcept {
    return token(token::kIdentidier, __text, __filename, __line, __offset);
  }

  const token token::make_hex_number(size_t __hex, const std::string &__filename, size_t __line, size_t __offset) noexcept {
    return token(token::kHexDigits, __hex, __filename, __line, __offset);
  }

  const token token::make_dec_number(int64_t __dec, const std::string &__filename, size_t __line, size_t __offset) noexcept {
    return token(token::kDecDigits, __dec, __filename, __line, __offset);
  }

  const token token::make_dec_number(size_t __dec, const std::string &__filename, size_t __line, size_t __offset) noexcept {
    return token(token::kDecDigits, __dec, __filename, __line, __offset);
  }

  bool token::has_rep() const noexcept  {
    return _kind == token::kIdentidier || _kind == token::kHexDigits || _kind == token::kDecDigits;
  }

  bool token::has_text_rep() const noexcept {
    return _kind == token::kIdentidier;
  }

  bool token::has_hex_rep() const noexcept  {
    return _kind == token::kHexDigits;
  }

  bool token::has_dec_rep() const noexcept  {
    return _kind == token::kDecDigits;
  }

}
