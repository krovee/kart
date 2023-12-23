#pragma once

#include <cstddef>

namespace kart::code_analysis::ast {
  /*
    simple utility helper to provide static incrementally new id.
  */
  struct __incremental_id_provider {
    inline auto operator()() noexcept {
      return ++_s_incremental_id;
    }
  private:
    static size_t _s_incremental_id;
  };

  /*
    to determine node kind(class) at runtime and not use any
    of RTTI you'd prefer this way, and labelless template
    class to make new instance of __node_base<T> and give
    it a new unique id.
  */
  template<class> 
  struct __node_base {
    static inline auto node_id = __incremental_id_provider()();
  };

  
}
