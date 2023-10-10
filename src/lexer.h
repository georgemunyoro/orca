#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  E_O_F
};

static const std::unordered_map<TokenType, std::string> TOK_STR = {
    // Single-character tokens.
    std::make_pair(LEFT_PAREN, "LEFT_PAREN"),
    std::make_pair(RIGHT_PAREN, "RIGHT_PAREN"),
    std::make_pair(LEFT_BRACE, "LEFT_BRACE"),
    std::make_pair(RIGHT_BRACE, "RIGHT_BRACE"), std::make_pair(COMMA, "COMMA"),
    std::make_pair(DOT, "DOT"), std::make_pair(MINUS, "MINUS"),
    std::make_pair(PLUS, "PLUS"), std::make_pair(SEMICOLON, "SEMICOLON"),
    std::make_pair(SLASH, "SLASH"), std::make_pair(STAR, "STAR"),

    // One or two character tokens.
    std::make_pair(BANG, "BANG"), std::make_pair(BANG_EQUAL, "BANG_EQUAL"),
    std::make_pair(EQUAL, "EQUAL"), std::make_pair(EQUAL_EQUAL, "EQUAL_EQUAL"),
    std::make_pair(GREATER, "GREATER"),
    std::make_pair(GREATER_EQUAL, "GREATER_EQUAL"),
    std::make_pair(LESS, "LESS"), std::make_pair(LESS_EQUAL, "LESS_EQUAL"),

    // Literals.
    std::make_pair(IDENTIFIER, "IDENTIFIER"), std::make_pair(STRING, "STRING"),
    std::make_pair(NUMBER, "NUMBER"),

    // Keywords.
    std::make_pair(AND, "AND"), std::make_pair(CLASS, "CLASS"),
    std::make_pair(ELSE, "ELSE"), std::make_pair(FALSE, "FALSE"),
    std::make_pair(FUN, "FUN"), std::make_pair(FOR, "FOR"),
    std::make_pair(IF, "IF"), std::make_pair(NIL, "NIL"),
    std::make_pair(OR, "OR"), std::make_pair(PRINT, "PRINT"),
    std::make_pair(RETURN, "RETURN"), std::make_pair(SUPER, "SUPER"),
    std::make_pair(THIS, "THIS"), std::make_pair(TRUE, "TRUE"),
    std::make_pair(VAR, "VAR"), std::make_pair(WHILE, "WHILE"),

    std::make_pair(E_O_F, "EOF")

};

std::string token_type_as_str(TokenType type);

enum LiteralType { NUM, STR, BOOL, NIL_ };

class Literal {
public:
  Literal() {}
  Literal(float i) : value(i), type(NUM) {}
  Literal(std::string s) : value(s), type(STR) {}
  Literal(bool b) : value(b), type(BOOL) {}

  LiteralType getType() { return type; };
  std::variant<std::monostate, float, std::string, bool> getValue() {
    return value;
  }

  LiteralType type = NIL_;
  std::variant<std::monostate, float, std::string, bool> value;

private:
};

class Token {
public:
  Token(TokenType token, std::string lexeme, Literal literal, int line)
      : type(token), lexeme(lexeme), literal(literal), line(line){};

  TokenType type;
  std::string lexeme;
  Literal literal;
  int line;
};

static const std::unordered_map<std::string, TokenType> KEYWORDS = {
    std::make_pair("and", AND),       std::make_pair("class", CLASS),
    std::make_pair("else", ELSE),     std::make_pair("false", FALSE),
    std::make_pair("for", FOR),       std::make_pair("fun", FUN),
    std::make_pair("if", IF),         std::make_pair("nil", NIL),
    std::make_pair("or", OR),         std::make_pair("print", PRINT),
    std::make_pair("return", RETURN), std::make_pair("super", SUPER),
    std::make_pair("this", THIS),     std::make_pair("true", TRUE),
    std::make_pair("var", VAR),       std::make_pair("while", WHILE),
};

class Lexer {
public:
  Lexer(std::string source) : source(source), start(0), current(0), line(1){};

  std::vector<Token> scan_tokens();
  void scan_token();
  void scan_identifier();
  void scan_string();
  void scan_number();
  char advance();
  char get_next_char();
  char get_curr_char();
  bool is_current_char(char expected);
  void add_token(TokenType type, std::string lexeme, Literal literal);
  void add_token(TokenType type);
  bool is_at_end();
  std::string get_token_text();

private:
  std::string source;
  int line;
  int start;
  int current;
  std::vector<Token> tokens;
};
