from Utility.tToken import *
from Utility.tError import InvalidSyntax
import Utility.tGlobals as tGlobals

#Most code taken from https://github.com/davidcallanan/py-myopl-code with some modifications

#NODES
class Node: #Base node class
    def __init__(self, start : Position, end : Position):
        self.start = start
        self.end = end

class NumberNode(Node): #Number literals
    def __init__(self, token : Token):
        self.token = token
        super().__init__(token.start, token.end)

    def __repr__(self):
        return f"{self.token.value}"

class BoolNode(Node): #Boolean literals
    def __init__(self, token : Token):
        self.token = token
        super().__init__(token.start, token.end)

    def __repr__(self):
        return f"{self.token.value}"

class NullNode(Node): #Null literal
    def __init__(self, token : Token):
        self.token = token
        super().__init__(token.start, token.end)

    def __repr__(self):
        return f"null"

class BinaryNode(Node): #Binary operations like addition, subtraction, etc
    def __init__(self, left : Node, operator : Token, right : Node):
        self.left = left
        self.operator = operator
        self.right = right

        super().__init__(left.start, right.end)

    def __repr__(self):
        return f"({self.left} {self.operator.value} {self.right})"

class UnaryNode(Node): #Unary operations like negation and not operator
    def __init__(self, operator : Token, right : Node):
        self.operator = operator
        self.right = right
        super().__init__(operator.start, right.end)

    def __repr__(self):
        return f"({self.operator.value} {self.right})"

class ParseResult: #I don't know how this works and can't be bothered to figure it out
    def __init__(self):
        self.error : InvalidSyntax = None
        self.node : Node = None
        self.last_registered_advance_count : int = 0
        self.advance_count : int = 0

    def register_advancement(self): #I don't know
        self.last_registered_advance_count = 1
        self.advance_count += 1

    def register(self, result): #Register another result, and set errors, and return node if no errors
        result : ParseResult = result
        self.last_registered_advance_count = result.last_registered_advance_count
        self.advance_count += result.advance_count
        if result.error:
            self.error = result.error
            tGlobals.has_error = True
        return result.node     

    def success(self, node : Node): #Set the node to return
        self.node = node
        return self

    def failure(self, error): #Keep the error to the first error encountered in code
        if not self.error or self.last_registered_advance_count == 0:
            self.error = error
            tGlobals.has_error = True
        return self

#PARSER CLASS
class Parser:
    def __init__(self, tokens : list[Token]):
        self.tokens = tokens
        self.index = -1
        self.advance()

    @property
    def at_end(self):
        return self.index >= len(self.tokens)

    @property
    def current_tok(self):
        return self.tokens[self.index] if not self.at_end else None

    def advance(self):
        self.index += 1
        return self.current_tok

    def advance_reg(self, res : ParseResult):
        res.register_advancement()
        self.advance()

    def parse(self):
        res = self.expr()

        if not res.error and self.current_tok.type != T_EOF:
            return res.failure(InvalidSyntax("Expected a statement or expression", self.current_tok.start, self.current_tok.end))

        return res

    def expr(self):
        return self.equality()

    def equality(self): #Equality testing
        return self.binop(self.comparison, (T_EE, T_NE))

    def comparison(self): #Comparisons
        return self.binop(self.sum, (T_LT, T_LTE, T_GT, T_GTE))

    def sum(self): #Addition and subtraction
        return self.binop(self.term, (T_PLUS, T_MINUS))

    def term(self): #Multiplication, division and modulus
        return self.binop(self.unary, (T_STAR, T_SLASH, T_PERCENT))

    def unary(self): #Negation
        res = ParseResult()
        operator = self.current_tok

        if operator.type in (T_PLUS, T_MINUS, T_NOT):
            self.advance_reg(res)
            factor = res.register(self.unary())
            if res.error: return res
            return res.success(UnaryNode(operator, factor))

        return self.power()

    def power(self): #Exponents
        return self.binop(self.atom, (T_CARET, ), self.unary)

    def atom(self): #Literals and grouping
        res = ParseResult()
        token = self.current_tok

        if token.type == T_NUMBER:
            self.advance_reg(res)
            return res.success(NumberNode(token))
        elif token.type in (T_TRUE, T_FALSE):
            self.advance_reg(res)
            return res.success(BoolNode(token))
        elif token.type == T_NULL:
            self.advance_reg(res)
            return res.success(NullNode(token))
        elif token.type == T_LPAR:
            self.advance_reg(res)
            expression = res.register(self.expr())
            if res.error: return res

            if self.current_tok.type != T_RPAR:
                return res.failure(InvalidSyntax("Expected a ')'", self.current_tok.start, self.current_tok.end))
            self.advance_reg(res)
            return res.success(expression)
        else:
            return res.failure(InvalidSyntax("Expected an expression", token.start, token.end))

    def binop(self, func_a, operators : tuple[int], func_b = None):
        res = ParseResult()
        if func_b == None:
            func_b = func_a

        left = res.register(func_a())
        if res.error: return res

        while self.current_tok.type in operators:
            op_tok = self.current_tok
            self.advance_reg(res)
            right = res.register(func_b())
            if res.error: return res
            left = BinaryNode(left, op_tok, right)

        return res.success(left)
