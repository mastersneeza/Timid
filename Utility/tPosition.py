class Position:
    def __init__(self, index : int, line : int, column : int, filename : str):
        self.index = index
        self.line = line
        self.column = column
        self.filename = filename

    def advance(self, current_char = '\0'):
        self.index += 1
        self.column += 1
        
        if current_char == '\n':
            self.line += 1
            self.column = 0

        return self

    def copy(self):
        copy : Position = Position(self.index, self.line, self.column, self.filename)
        return copy

    def __repr__(self):
        return f"Position({self.index}, {self.line}, {self.column}, '...')"