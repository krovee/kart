#include "io.h"
#include "code_analysis/tokenizer.h"
#include <iostream>
#include <vector>
#include <string_view>

auto main(int argc, char** argv) -> int {
  std::vector<std::string_view> args(argv, argv + argc);

  // if (1 == args.size())  {
  //   std::cout << "Usage: kart --ibeg <inputs> --iend [--o <output>]" << std::endl;
  //   return 0;
  // }
  // else
  {
    std::vector<std::string> __input_filenames = {};
    std::string __output_filename = "a.out";

    for (auto it = args.begin(); it != args.end(); ++it) {
      if (*it == "--ibeg") {
        ++it; // skip
        while (it != args.end() && "--iend" != *it) {
          __input_filenames.emplace_back(*it++);
        }
      }
    } 

    #ifndef NDEBUG
    __input_filenames.emplace_back("tests/cases/tokenizer_test.kart");
    #endif

    // work for every file (compile every file to a target)
    // [context_t ctx] <- here all compiled symols goes...

    for (auto&& __filename : __input_filenames) {

      std::string __text_buf;
      if (not kart::read_file(__filename, __text_buf)) {
        std::cerr << "GNRL0001: Unable to read file: [ " << __filename << " ]! skipping..." << std::endl;
        continue;
      }

      kart::code_analysis::tokenizer __tokenizer(__filename, __text_buf);
      auto __tokens = kart::code_analysis::tokenizer::tokens_stream();
      const auto __max_diagnostics_level = __tokenizer.tokenize(__tokens);
      auto __ostream_ptr = static_cast<std::ostream*>(nullptr);
      if (__max_diagnostics_level == kart::code_analysis::tokenizer::kError) {
        __ostream_ptr = &std::cerr;
      }
      else if (__max_diagnostics_level == kart::code_analysis::tokenizer::kWarning) {
        __ostream_ptr = &std::cout;
      }

      if (__ostream_ptr) {
        // ok, we need to output some messages!
        auto& __ostream = *__ostream_ptr;
        auto const&& __diagnostics = __tokenizer.get_diagnostics();
        for (auto&& msg : __diagnostics) {
          __ostream << msg.loc.as_string() << msg.text << std::endl;
        }
      }
      
    }

  }
  
  return 0; 
}
