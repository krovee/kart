#include "tokenizer.h"
#include <cctype>
#include <cstdint>

#define elif else if

namespace kart {
  tokenizer::max_diagnostics_level 
  tokenizer::tokenize(tokenizer::tokens_stream& stream) noexcept {

    const auto __text_begin = _text.begin();
    const auto __text_end = _text.end();

    const auto id_cont_check = [__text_end](std::string::const_iterator const& it) -> bool 
    { return it != __text_end && (std::isalnum(*it) || '_' == *it); };

    const auto digit_start_check = [__text_end](std::string::const_iterator const& it) -> uint32_t {
      const auto __r0 = it != __text_end;
      if (__r0) {
        const auto __r1 = static_cast<bool>(std::isdigit(*it));
        const auto __r4 = '#' == *it;
        if (__r1) {
          return 1;
        }
        const auto __r2 = (it + 1) != __text_end;
        if (__r2) {
          const auto __it = it + 1;
          const auto __r3 = ('-' == *it) && std::isdigit(*__it);
          if (__r3) {
            return 2;
          }
          else if (__r4 && std::isdigit(*__it)) {
            return 3;
          }
        }
      }
      return 0;
    };

    const auto hex_digit_cont_check = [__text_end](std::string::const_iterator const& it, size_t& x_count) -> bool 
    { 
      const auto r0 =  it != __text_end;
      const auto r1 = 'x' == *it || 'X' == *it;
      const auto r2 = static_cast<bool>(std::isdigit(*it));
      if (true == r1) { ++x_count; }
      return (r0 && r2) || r1;
    };

    #define __process_punctuation(ch) \
    elif (ch == *_current) { \
      while (_current != __text_end && ch == *_current) { ++_current; ++_curr_line.offset; } \
      stream.push_back(token::make_punctuation(ch, _filename, _curr_line.num, _curr_line.offset)); \
    }

    for (; _current != __text_end;) {
      if ('\n' == *_current) {
        while (_current != __text_end && '\n' == *_current) { 
          ++_current; 
          ++_curr_line.num; 
          _curr_line.offset = 0; 
        }
        stream.push_back(token::make_punctuation('\n', _filename, _curr_line.num - 1, _curr_line.offset)); 
      }
      __process_punctuation(' ')
      __process_punctuation('!')
      __process_punctuation('`')
      __process_punctuation('~')
      __process_punctuation('@')
      __process_punctuation('#')
      __process_punctuation('$')
      __process_punctuation('%')
      __process_punctuation('^')
      __process_punctuation('&')
      __process_punctuation('*')
      __process_punctuation('(')
      __process_punctuation(')')
      __process_punctuation('-')
      __process_punctuation('_')
      __process_punctuation('=')
      __process_punctuation('+')
      __process_punctuation('[')
      __process_punctuation(']')
      __process_punctuation('{')
      __process_punctuation('}')
      __process_punctuation('\\')
      __process_punctuation('|')
      __process_punctuation(';')
      __process_punctuation(':')
      __process_punctuation('\'')
      __process_punctuation('"')
      __process_punctuation(',')
      __process_punctuation('.')
      __process_punctuation('<')
      __process_punctuation('>')
      __process_punctuation('/')
      __process_punctuation('?')
      elif (std::isalpha(*_current)) {
        // ok, this is an identifier start!
        const auto __start = _current; 
        const auto __line_offset = _curr_line.offset;
        while (id_cont_check(_current)) { ++_current; ++_curr_line.offset; }
        const auto __end = _current;
        const auto __text = 
          _text.substr(__start - __text_begin, __end - __start);
        
        stream.push_back(token::make_identifier(__text, _filename, _curr_line.num, __line_offset));
      }
      elif (const auto __case = digit_start_check(_current); __case) {
        const auto __start = _current;
        const auto __line_offset = _curr_line.offset;
        size_t x_count = 0;
        while (hex_digit_cont_check(_current, x_count)) { ++_current; ++_curr_line.offset; }
        const auto __end = _current;
        const auto __text_rep = 
          _text.substr(__start - __text_begin, __end - __start);
        
        if (3 == __case) {
          // MUST be a hex
          if (0 == x_count) {
            // #123 -> it is valid
            std::string __text_hex(__text_rep.size(), 0);
            __text_hex += "0x";
            __text_hex += __text_rep.substr(1);
            const auto __hex_rep = std::stoull(__text_hex);

            stream.push_back(token::make_hex_number(__hex_rep, _filename, _curr_line.num, __line_offset));
          }
          else {
            // #1x321 -> it is invalid!
            add_message("TKN0003: Invalid hash-beginning hex digit (probably something like #1x123)!");
            return kError;
          }
        }
        elif (0 == x_count) {
          // normal decimal value (signed/unsigned ?)
          if (1 == __case) {
            // unsigned
            const auto __dec_rep = std::stoull(__text_rep);
            stream.push_back(
              token::make_dec_number(static_cast<size_t>(__dec_rep), _filename, _curr_line.num, _curr_line.offset));
          }
          elif (2 == __case) {
            // signed
            const auto __dec_rep = std::stoll(__text_rep);
            stream.push_back(
              token::make_dec_number(static_cast<int64_t>(__dec_rep), _filename, _curr_line.num, _curr_line.offset));
          }
        }
        elif (1 == x_count) {
          if (__case == 1) {
            // 0x134223 -> normal hex
          }
          else {
            // 0x-132312 -> invalid hex
            add_message("TKN0004: Invalid hex digit (probably something like 0x-123...)!");
          }
        }
        else {
          add_message("TKN0002: Invalid hex digit (probably something like 0x12x3x...)!");
          return kError;
        }
      }
      else {
        add_message("TKN001: Unrecognized character!");
        return kError;
      }
    }

    return kNone;
  }

  void tokenizer::add_message(std::string const& msg) noexcept {
    _diagnostics += diagnostics::message{ diagnostics::location(_filename, _curr_line.num, _curr_line.offset), msg };
  }
}
