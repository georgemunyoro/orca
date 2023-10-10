
bool isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

bool isdigit(char c) { return c >= '0' && c <= '9'; }

bool isalpha(char c) { return c >= 'a' && c <= 'Z'; }

bool isalphanumeric(char c) { return isalpha(c) || isdigit(c); }
