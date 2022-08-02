#pragma once

#include <iterator>
#include <sstream>
#include <limits>
#include <string>
#include <memory>
#include <variant>
#include <regex>
#include <type_traits>

#include "core.hxx"

namespace frameless::lexer {

  struct eof {
    operator std::string() const;
  };

  struct ident : public std::string {
    using std::string::string;
    ident(const std::string & str) noexcept;
  };

  struct lparen {
    operator std::string() const;
  };

  struct rparen {
    operator std::string() const;
  };

  using lexemes = std::variant<lparen, rparen, ident, eof>;
  
  struct lexeme : public lexemes {
    using lexemes::variant;
    operator std::string() const;
  };

  class lex {
  public:
    class iterator {
    public:
      iterator(std::istream & input, const lexeme & lexeme) noexcept;
      const lexeme & operator*();
      iterator & operator++();
      iterator & operator++(int);
      bool operator!=(const iterator & other);

    private:
      std::istream & inputs;
      std::string buffer;
      lexeme cur;
      size_t pos;
    };

    explicit lex(std::istream & input);

    iterator begin();
    const iterator end();

  private:
    std::istream & inputs;
  };


  // implementations

  eof::operator std::string() const {
    return "eof";
  }

  ident::ident(const std::string & str) noexcept : std::string(str) {}
  
  lparen::operator std::string() const {
    return "(";
  }

  rparen::operator std::string() const {
    return ")";
  }

  lexeme::operator std::string() const {
    return std::visit([](const auto & x) { return (std::string)x; }, (lexemes)*this);
  }

  lex::lex(std::istream & is) : inputs(is) {}

  lex::iterator::iterator(std::istream & is, const lexeme & lexeme = eof()) noexcept : inputs(is), pos(0) {}

  const lexeme & lex::iterator::operator*() {
    return cur;
  }

  lex::iterator & lex::iterator::operator++() {
    if (buffer.empty() && !(inputs >> buffer)) {
      pos = 0;
      cur = eof();
      return *this;
    }

    ++pos;

    std::smatch result;

    if (std::regex_search(buffer, result, std::regex("^\\s*([a-zA-Z_][a-zA-Z0-9_]*)"))) {
      cur = ident(result[1]);
    }

    else if (std::regex_search(buffer, result, std::regex("^\\s*\\("))) {
      cur = lparen();
    }

    else if (std::regex_search(buffer, result, std::regex("^\\s*\\)"))) {
      cur = rparen();
    }

    buffer = result.suffix();
    return *this;
  }

  lex::iterator & lex::iterator::operator++(int n) {
    for (int i = 0; i < n; i++) {
      ++ * this;
    }
    return *this;
  }

  bool lex::iterator::operator!=(const iterator & other) {
    return pos != other.pos;
  }

  lex::iterator lex::begin() {
    auto iter = lex::iterator(inputs);
    iter.operator++();
    return iter;
  }

  const lex::iterator lex::end() {
    return lex::iterator(inputs);
  }

};
