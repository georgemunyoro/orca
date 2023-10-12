#include "lexer.h"
#include <_ctype.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

char Lexer::advance() {
  ++this->current;
  return this->source[this->current - 1];
}

std::vector<Token> Lexer::scan_tokens() {
  while (!is_at_end()) {
    start = current;
    scan_token();
  }
  tokens.push_back(Token(E_O_F, "", Literal{}, 0));
  return tokens;
}

void Lexer::scan_number() {
  while (isdigit(get_curr_char()))
    advance();

  if (get_curr_char() == '.' && isdigit(get_next_char())) {
    do {
      advance();
    } while (isdigit(get_curr_char()));
  }

  std::string text = source.substr(start, current - start);
  Literal value = Literal(std::stof(text));
  add_token(NUMBER, text, value);
}

void Lexer::scan_identifier() {
  while (isalnum(get_curr_char())) {
    advance();
  }

  if (KEYWORDS.count(get_token_text()) > 0)
    return add_token(KEYWORDS.at(get_token_text()));

  add_token(IDENTIFIER);
}

void Lexer::scan_string() {
  while (get_curr_char() != '"' && !is_at_end()) {
    if (get_curr_char() == '\n')
      ++line;
    advance();
  }

  if (is_at_end())
    throw std::runtime_error("Unterminated string.");

  advance();

  std::string text = source.substr(start + 1, current - start - 2);
  Literal value = new Literal();
  value.value = text;
  value.type = STR;
  add_token(STRING, text, value);
}

char Lexer::get_next_char() {
  return current + 1 >= source.length() ? '\0' : source[current + 1];
}

char Lexer::get_curr_char() { return is_at_end() ? '\0' : source[current]; }

std::string Lexer::get_token_text() {
  return source.substr(start, current - start);
}

void Lexer::scan_token() {
  char c = advance();

  if (isspace(c)) {
    if (c == '\n')
      ++line;
    return;
  }

  if (isdigit(c))
    return scan_number();

  if (isalpha(c))
    return scan_identifier();

  switch (c) {
  case '(':
    add_token(LEFT_PAREN);
    break;
  case ')':
    add_token(RIGHT_PAREN);
    break;
  case '{':
    add_token(LEFT_BRACE);
    break;
  case '}':
    add_token(RIGHT_BRACE);
    break;
  case '[':
    add_token(LEFT_BRACK);
    break;
  case ']':
    add_token(RIGHT_BRACK);
    break;
  case ',':
    add_token(COMMA);
    break;
  case '.':
    add_token(DOT);
    break;
  case '-':
    add_token(MINUS);
    break;
  case '+':
    add_token(PLUS);
    break;
  case ';':
    add_token(SEMICOLON);
    break;
  case '*':
    add_token(STAR);
    break;

  case '!':
    add_token(is_current_char('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    add_token(is_current_char('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    add_token(is_current_char('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    add_token(is_current_char('=') ? GREATER_EQUAL : GREATER);
    break;

  case '/':
    if (is_current_char('/')) {
      while (get_curr_char() != '\n' && !is_at_end())
        advance();
    } else {
      add_token(SLASH);
    }
    break;

  case '"':
    scan_string();
    break;

  default:
    throw std::runtime_error("Unexpected character.");
  }
}

void Lexer::add_token(TokenType type, std::string lexeme, Literal literal) {
  tokens.push_back(
      Token(type, source.substr(start, current - start), literal, line));
}

void Lexer::add_token(TokenType type) {
  tokens.push_back(
      Token(type, source.substr(start, current - start), Literal{}, line));
}

bool Lexer::is_at_end() { return current >= source.length(); }

bool Lexer::is_current_char(char expected) {
  if (is_at_end() || source[current] != expected)
    return false;

  ++current;
  return true;
}

std::string token_type_as_str(TokenType type) { return TOK_STR.at(type); }
