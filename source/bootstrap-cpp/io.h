#pragma once

#include <string>

namespace kart {
  /*
    example implementation in kart:
    kart.read_file(__filename : string, __text : &string) : bool

    Description: 
      A simple syncronous api function to read file context if
      it is exists. 
  */
  bool read_file(std::string const& __filename, std::string& __text) noexcept;
}
