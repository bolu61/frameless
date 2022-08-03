#include <string>
#include <variant>

namespace frameless {

  class token {
  public:

    struct eof {};

    struct ident {
      std::string name;
      explicit ident(const std::string & name) noexcept;
    };

    struct lparen {};

    struct rparen {};

    using variant = std::variant<eof, ident, lparen, rparen>;

    template<typename token_type>
    explicit(false) constexpr token(const token_type & t) noexcept;

    template<typename token_type>
    explicit(false) constexpr token(token_type && t) noexcept;

    explicit(false) operator variant() const noexcept;

  private:
    variant v;
  };


  // implementations

  token::ident::ident(const std::string & name) noexcept : name(name) {
    // noop
  }

  template<typename token_type>
  constexpr token::token(const token_type & t) noexcept : v(t) {
    // noop
  }
  
  template<typename token_type>
  constexpr token::token(token_type && t) noexcept : v(std::forward<token_type>(t)) {
    // noop
  }

  token::operator variant() const noexcept {
    return v;
  }
}
