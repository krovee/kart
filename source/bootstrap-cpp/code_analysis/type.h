#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace kart::code_analysis {
  /*
    so, i wanna have a pretty simple typesystem,
    with a few basic kinds of data types,
    such as fundamental, derived, defined.
  */
  class type {
  public:
    enum kind : uint32_t {
      /*
        any simple predefined type (iXXX, uXXX, fXXX)
      */
      fundamental = 0x10000000,
      /*
        type(f/d) with any modificator, such as
        arrays, references, constants
      */
      derived     = 0x20000000,
      /*
        user-defined type (structures...)
      */
      defined     = 0x40000000,
    };

    enum modifier : uint32_t {
      modifier_const      = 0x01000000,
      modifier_reference  = 0x02000000,
    };

    virtual size_t              get_num_bytes() const noexcept = 0;
    virtual std::string const&  get_type_name() const noexcept = 0;
    
    auto is_fundamental() const noexcept {
      return (_kind & fundamental) == fundamental;
    }
    auto is_derived() const noexcept {
      return (_kind & derived) == derived;
    }
    auto is_defined() const noexcept {
      return (_kind & defined) == defined;
    }
  private:
    kind _kind = fundamental;
  };
}
