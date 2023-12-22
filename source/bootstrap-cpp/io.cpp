#include "io.h"
#include <fstream>

namespace kart {
  bool read_file(std::string const& __filename, std::string& __text) noexcept {

    // Open input file stream...
    std::ifstream __if(__filename, std::ios::binary);
    if (false == __if.is_open()) {
      return false;
    }

    // Gather file size...
    __if.seekg(0, std::ios::end);
    const size_t __size = __if.tellg();
    __if.seekg(0, std::ios::beg);

    // Reserve buffer and read file into...
    __text.resize(__size);
    __if.read(__text.data(), __size);

    // Close input file stream...
    __if.close();

    return true;
  }
}
