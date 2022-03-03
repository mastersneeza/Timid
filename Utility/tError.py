from Utility.tPosition import Position

class Error:
    def __init__(self, err_name : str, description : str, start : Position, end : Position):
        self.err_name = err_name
        self.description = description
        self.start = start
        self.end = end

    def __str__(self):
        error = f"{self.err_name} Error: {self.description}"
        error += f"\n\tFile '{self.start.filename}' @ ({self.start.line + 1}, {self.start.column + 1})"
        return error

class InvalidCharacter(Error):
    def __init__(self, description : str, start : Position, end : Position):
        super().__init__("Invalid Character", description, start, end)

class InvalidSyntax(Error):
    def __init__(self, description : str, start : Position, end : Position):
        super().__init__("Syntax", description, start, end)

class FileNotFound(Error):
    def __init__(self, description : str):
        super().__init__("File Not Found", description, None, None)

    def __str__(self):
        error = f"{self.err_name} Error: {self.description}"
        return error