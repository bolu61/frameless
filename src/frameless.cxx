#include <iostream>
#include <sstream>
#include <variant>

#include "lex.hxx"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main(int argc, char * argv[])
{
  for (auto && token : frameless::lex(std::istringstream("hello (world)"))) {
    std::cout << std::visit(overloaded {
      [](frameless::token::eof) { return "eof"; },
      [](frameless::token::ident ident) { return ident.name.data(); },
      [](frameless::token::lparen) { return "("; },
      [](frameless::token::rparen) { return ")"; },
    }, (frameless::token::variant)token);

    std::cout << " ";
  }
  std::cout << "eof" << std::endl;
  return 0;
}
