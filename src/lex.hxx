#pragma once

#include <iterator>
#include <limits>
#include <string>
#include <memory>
#include <variant>
#include <regex>
#include <type_traits>
#include <optional>

#include "core.hxx"
#include "token.hxx"

namespace frameless {

  template<typename istream>
  class lex {

  public:

    class iterator {

    public:

      explicit iterator(lex<istream> * lexer) noexcept;
      explicit iterator() noexcept;

      const token & operator*() const;

      iterator & operator++();

      iterator & operator++(int);

      bool operator!=(const iterator & other) const;

    private:
      lex<istream> * lexeme;
      size_t pos;
      token cur;
    };

    explicit lex(istream && source) noexcept;

    iterator begin();

    iterator end();

  private:

    istream source;

    std::string buffer;

    token get();
  };


  // implementations

  template<typename istream>
  lex<istream>::lex(istream && source) noexcept : source(std::move(source)) {
    // noop
  };

  template<typename istream>
  token lex<istream>::get() {
    if (buffer.empty() && !(source >> buffer)) {
      return token::eof();
    }

    if (std::smatch result; std::regex_search(buffer, result, std::regex("^\\s*([a-zA-Z_][a-zA-Z0-9_]*)"))) {
      token out(token::ident(result[1].str()));
      buffer = result.suffix().str();
      return out;
    }

    else if (std::regex_search(buffer, result, std::regex("^\\s*\\("))) {
      buffer = result.suffix();
      return token::lparen();
    }

    else if (std::regex_search(buffer, result, std::regex("^\\s*\\)"))) {
      buffer = result.suffix();
      return token::rparen();
    }
  };

  template<typename istream>
  lex<istream>::iterator::iterator(lex<istream> * lexer) noexcept
    : lexeme(lexer), pos(0), cur(lexer->get()) {
    // noop
  };

  template<typename istream>
  lex<istream>::iterator::iterator() noexcept
    : lexeme(nullptr), pos(std::numeric_limits<size_t>::max()), cur(token::eof()) {
    // noop
  };

  template<typename istream>
  const token & lex<istream>::iterator::operator*() const {
    return cur;
  }

  template<typename istream>
  typename lex<istream>::iterator & lex<istream>::iterator::operator++() {
    if (lexeme != nullptr) {
    cur = lexeme->get();
      if (std::holds_alternative<token::eof>((token::variant)cur)) {
        pos = std::numeric_limits<size_t>::max();
      } else {
        ++pos;
      }
    }
    return *this;
  }

  template<typename istream>
  typename lex<istream>::iterator & lex<istream>::iterator::operator++(int n) {
    while (n-- > 0) {
      ++*this;
    }
    return *this;
  }

  template<typename istream>
  bool lex<istream>::iterator::operator!=(const iterator & other) const {
    return pos != other.pos;
  }

  template<typename istream>
  typename lex<istream>::iterator lex<istream>::begin() {
    return lex::iterator(this);
  }

  template<typename istream>
  typename lex<istream>::iterator lex<istream>::end() {
    return lex::iterator();
  }

};
