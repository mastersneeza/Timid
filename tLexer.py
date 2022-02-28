from string import whitespace, digits
from tPosition import Position
from tToken import *
from tError import InvalidCharacter
import tGlobals

#LEXER CLASS
class Lexer:
    def __init__(self, path, source):
        self.source = source
        self.pos = Position(-1, 0, -1, path)
        self.advance()

    @property
    def at_end(self) -> bool:
        return self.pos.index >= len(self.source)

    @property
    def current_char(self) -> str:
        return self.source[self.pos.index] if not self.at_end else '\0'

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

    def lex(self) -> list:
        tokens = []
        while not self.at_end:
            #Whitespace
            if self.current_char in whitespace:
                self.advance()
            #Literals
            elif self.current_char in digits:
                tokens.append(self.number())
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