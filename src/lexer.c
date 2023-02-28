// Define the maximum length of an identifier or number
#define MAX_TOKEN_LEN 100

// Define the list of reserved keywords
char * keywords[] = {
  "if",
  "else",
  "while",
  "for",
  "int",
  "float",
  "char",
  "void"
};

// Define the list of token types
typedef enum {
  TOKEN_UNKNOWN,
  TOKEN_EOF,
  TOKEN_IDENTIFIER,
  TOKEN_KEYWORD,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_OPERATOR,
  TOKEN_DELIMITER
}
token_type;

// Define the token structure
typedef struct {
  token_type type;
  char value[MAX_TOKEN_LEN];
}
token;

// Define the lexer state
typedef struct {
  char * input;
  char * current;
  int line;
}
lexer_state;

// Initialize the lexer state
void init_lexer(lexer_state * state, char * input) {
  state -> input = input;
  state -> current = input;
  state -> line = 1;
}

// Get the next token from the input stream
token get_next_token(lexer_state * state) {
  token t = {
    TOKEN_UNKNOWN,
    ""
  };

  // Skip whitespace and comments
  while (isspace( * state -> current) || * state -> current == '#') {
    if ( * state -> current == '\n') {
      state -> line++;
    }
    if ( * state -> current == '#') {
      while ( * state -> current != '\n' && * state -> current != '\0') {
        state -> current++;
      }
    }
    state -> current++;
  }

  // Check for end of input
  if ( * state -> current == '\0') {
    t.type = TOKEN_EOF;
    return t;
  }

  // Check for identifiers and keywords
  if (isalpha( * state -> current)) {
    int i = 0;
    while (isalnum( * state -> current) || * state -> current == '_') {
      t.value[i++] = * state -> current++;
    }
    t.value[i] = '\0';
    for (i = 0; i < sizeof(keywords) / sizeof( * keywords); i++) {
      if (strcmp(t.value, keywords[i]) == 0) {
        t.type = TOKEN_KEYWORD;
        return t;
      }
    }
    t.type = TOKEN_IDENTIFIER;
    return t;
  }

  // Check for numbers
  if (isdigit( * state -> current)) {
    int i = 0;
    while (isdigit( * state -> current)) {
      t.value[i++] = * state -> current++;
    }
    if ( * state -> current == '.') {
      t.value[i++] = * state -> current++;
      while (isdigit( * state -> current)) {
        t.value[i++] = * state -> current++;
      }
      t.type = TOKEN_NUMBER;
      return t;
    }
    t.type = TOKEN_NUMBER;
    return t;
  }

  // Check for strings
  if ( * state -> current == '\"') {
    int i = 0;
    state -> current++;
    while ( * state -> current != '\"' && * state -> current != '\0') {
      t.value[i++] = * state -> current++;
    }
    if ( * state -> current == '\"') {
      state -> current++;
      t.type = TOKEN_STRING;
      return t;
    }
    // TODO: handle unterminated string error
  }

  // Check for operators and delimiters
  switch ( * state -> current) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '&':
  case '|':
  case '^':
  case '~':
  case '!':
    t.type = TOKEN_OPERATOR;
    t.value[0] = * state -> current++;
    if ( * state -> current == '=') {
      t.value[1] = * state -> current++;
      t.value[2] = '\0';
    } else {
      t.value[1] = '\0';
    }
    return t;
  case '<':
  case '>':
  case '=':
  case '?':
  case ':':
  case ',':
  case ';':
  case '.':
  case '(':
  case ')':
  case '[':
  case ']':
  case '{':
  case '}':
    t.type = TOKEN_DELIMITER;
    t.value[0] = * state -> current++;
    t.value[1] = '\0';
    return t;
  default:
    // TODO: handle unknown token error
    break;
  }
  return t;
}
int main() {
  char input[] = "int main() {\n printf("
  Hello, World!\n ");\n return 0;\n}";
  lexer_state state;
  init_lexer( & state, input);
  token t;
  do {
    t = get_next_token( & state);
    switch (t.type) {
    case TOKEN_UNKNOWN:
      printf("Unknown token: %s\n", t.value);
      break;
    case TOKEN_EOF:
      printf("End of input\n");
      break;
    case TOKEN_IDENTIFIER:
      printf("Identifier: %s\n", t.value);
      break;
    case TOKEN_KEYWORD:
      printf("Keyword: %s\n", t.value);
      break;
    case TOKEN_NUMBER:
      printf("Number: %s\n", t.value);
      break;
    case TOKEN_STRING:
      printf("String: " % s "\n", t.value);
      break;
    case TOKEN_OPERATOR:
      printf("Operator: %s\n", t.value);
      break;
    case TOKEN_DELIMITER:
      printf("Delimiter: %s\n", t.value);
      break;
    }
  } while (t.type != TOKEN_EOF);
  return 0;
}
