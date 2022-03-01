from Utility.tEnum import iota
from Utility.tPosition import Position

#TOKEN TYPES
#T stands for Token Type
T_NUMBER = iota(True)

T_PLUS = iota() #+
T_MINUS = iota() #-
T_STAR = iota() #*
T_SLASH = iota() #/
T_CARET = iota() #^
T_PERCENT = iota() #%

T_LPAR = iota() #(
T_RPAR = iota() #)

T_EOF = iota() #End of file token

#TOKEN CLASS
class Token:
    def __init__(self, type : int, value : any, start : Position, end : Position = None):
        self.type = type
        self.value = value
        self.start = start.copy()

        if end != None:
            self.end = end.copy()
        else:
            self.end = self.start.copy().advance()

    def __repr__(self):
        return f"Token({self.type}, {self.value}, {self.start}, {self.end})"

    def __str__(self):
        return f"{self.type} : '{self.value}'"