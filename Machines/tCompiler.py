import struct
from Machines.tLexer import Lexer
from Machines.tParser import ParseResult, Parser
from Utility.tError import FileNotFound
import Utility.tGlobals as tGlobals
from Utility.tEnum import iota
from Utility.tToken import *

#Opcodes for virtual machine
OP_NOP = iota(True) #No-op

OP_NUMBER = iota() #Push a number onto stack

OP_ADD = iota() #Push top two numbers, adds them, and pushes them back
OP_SUB = iota() #Like add but with subtract
OP_MUL = iota() #Multiplication
OP_DIV = iota() #Division
OP_POW = iota() #Exponents
OP_MOD = iota() #Modulus

OP_NEG = iota() #Negates top of stack
OP_DUMP = iota() #Temporary instruction

#COMPILER CLASS - COMPILES AST DOWN TO BYTECODE FOR C VIRTUAL MACHINE
class Compiler:
    def __init__(self, path : str):
        self.ast = None
        self.data = []
        self.program = []
        self.path = path
        try:
            with open(path, 'r') as f:
                self.source = f.read()
        except FileNotFoundError:
            print(FileNotFound(f"File '{path}' not found"))
            tGlobals.has_error = True

    def lex(self) -> list[Token]:
        lexer = Lexer(self.path, self.source)
        tokens = lexer.lex()

        return tokens

    def parse(self, tokens : list[Token]) -> ParseResult:
        parser = Parser(tokens)
        return parser.parse()

    def visit(self, node):
        method_name = f'v{type(node).__name__}'
        method = getattr(self, method_name, self.no_visit)
        method(node)

    def no_visit(self, node):
        assert False, f"No v{type(node).__name__} method defined"

    def vNumberNode(self, node):
        self.program.append((OP_NUMBER, node.token.value))

    def vBinaryNode(self, node):
        self.visit(node.left)
        self.visit(node.right)

        op = node.operator.type
        if op == T_PLUS:
            self.program.append((OP_ADD, ))
        elif op == T_MINUS:
            self.program.append((OP_SUB, ))
        elif op == T_STAR:
            self.program.append((OP_MUL, ))
        elif op == T_SLASH:
            self.program.append((OP_DIV, ))
        elif op == T_CARET:
            self.program.append((OP_POW, ))
        elif op == T_PERCENT:
            self.program.append((OP_MOD, ))
        else:
            assert False, "Binary not implemented!"

    def vUnaryNode(self, node):
        self.visit(node.right)

        op = node.operator.type

        if op == T_MINUS:
            self.program.append((OP_NEG, ))
        elif op == T_PLUS:
            pass
        else:
            assert False, "Unary not implemented!"

    def write(self):
        for command in self.program:
            self.data.append(command[0])
            operands = command[1:]
            operand_bytes = []
            for operand in operands:
                operand_bytes = bytearray(struct.pack('f', operand))
                self.data.extend(bytes(operand_bytes))

        self.data.append(OP_DUMP)

        new_path = self.path.split('\\')[-1].split('.')[0] + '.timb'

        with open(new_path, 'wb') as f:
            #print(self.program)
            #print(self.data)
            f.write(bytes(self.data))

    def compile(self):
        tokens = self.lex()

        if tGlobals.has_error:
            return

        self.ast = self.parse(tokens)

        if self.ast.error:
            print(self.ast.error)
            return

        self.visit(self.ast.node) #Like visitor method in tree walk interpreter but instead of interpreting and having slow runtime having slower compile time with fast runtime
        self.write()