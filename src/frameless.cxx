#include <iostream>
#include <sstream>
#include <variant>

#include "lex.hxx"

int main (int argc, char* argv[])
{
  for (auto prog = std::istringstream("hello (world)"); auto && lexeme : frameless::lexer::lex(prog)) {
    std::cout << (std::string)lexeme << " ";
  }
  std::cout << std::endl;
  return 0;
}
