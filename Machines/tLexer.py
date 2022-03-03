from string import ascii_letters, whitespace, digits
from Utility.tPosition import Position
from Utility.tToken import *
from Utility.tError import InvalidCharacter
import Utility.tGlobals as tGlobals

#LEXER CLASS
class Lexer:
    def __init__(self, path, source):
        self.source = source
        self.pos = Position(-1, 0, -1, path)
        self.advance()

    @property
    def at_end(self) -> bool:
        result = self.pos.index >= len(self.source)
        return result

    @property
    def current_char(self) -> str:
        try:
            return self.source[self.pos.index]
        except IndexError:
            return '\0'

    def advance(self) -> str:
        self.pos.advance(self.current_char)
        return self.current_char

    def make_token(self, type : int, value : any, start : Position = None, end : Position = None) -> Token:
        if start == None:
            start = self.pos
        token = Token(type, value, start, end)
        return token

    def number(self) -> Token: #code taken directly from https://github.com/davidcallanan/py-myopl-code
        num_str = ''
        dot_count = 0
        start = self.pos.copy()

        while not self.at_end and self.current_char in digits + '.':
            if self.current_char == '.':
                if dot_count == 1: break
                dot_count += 1
            num_str += self.current_char
            self.advance()

        return Token(T_NUMBER, float(num_str), start, self.pos)

    def two_char_token(self, one_char_token, double_char_token, match):
        tt = one_char_token
        start = self.pos.copy()
        value = self.current_char

        self.advance()

        if self.current_char == match:
            value += self.current_char
            tt = double_char_token
            self.advance()

        return Token(tt, value, start, self.pos)

    def identifier(self):
        id_str = ''
        pos_start = self.pos.copy()

        keywords = {
            'true': T_TRUE,
            'false': T_FALSE,
            'null': T_NULL,
        }

        while self.current_char != None and self.current_char in ascii_letters + digits + '_':
            id_str += self.current_char
            self.advance()

        tok_type = keywords.get(id_str, T_IDENTIFIER)
        return Token(tok_type, id_str, pos_start, self.pos)

    def lex(self) -> list:
        tokens = []
        while not self.at_end:
            #Whitespace
            if self.current_char in whitespace:
                self.advance()
            #Literals
            elif self.current_char in digits:
                tokens.append(self.number())
            #Identifiers and keywords
            elif self.current_char in ascii_letters + '_':
                tokens.append(self.identifier())
            #Operators
            elif self.current_char == '+':
                tokens.append(self.make_token(T_PLUS, self.current_char))
                self.advance()
            elif self.current_char == '-':
                tokens.append(self.make_token(T_MINUS, self.current_char))
                self.advance()
            elif self.current_char == '*':
                tokens.append(self.make_token(T_STAR, self.current_char))
                self.advance()
            elif self.current_char == '/':
                tokens.append(self.make_token(T_SLASH, self.current_char))
                self.advance()
            elif self.current_char == '^':
                tokens.append(self.make_token(T_CARET, self.current_char))
                self.advance()
            elif self.current_char == '%':
                tokens.append(self.make_token(T_PERCENT, self.current_char))
                self.advance()
            elif self.current_char == '=':
                tokens.append(self.two_char_token(T_EQ, T_EE, '='))
            elif self.current_char == '<':
                tokens.append(self.two_char_token(T_LT, T_LTE, '='))
            elif self.current_char == '>':
                tokens.append(self.two_char_token(T_GT, T_GTE, '='))
            elif self.current_char == '!':
                tokens.append(self.two_char_token(T_NOT, T_NE, '='))
            #Parentheses
            elif self.current_char == '(':
                tokens.append(self.make_token(T_LPAR, self.current_char))
                self.advance()
            elif self.current_char == ')':
                tokens.append(self.make_token(T_RPAR, self.current_char))
                self.advance()
            else:
                print(InvalidCharacter(f"'{self.current_char}'", self.pos, self.pos))
                tGlobals.has_error = True
                self.advance()

        tokens.append(self.make_token(T_EOF, '\0'))

        return tokens