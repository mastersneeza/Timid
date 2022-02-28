from tCompiler import Compiler

class Timid:
    @staticmethod
    def run(path): #Run method
        compiler = Compiler(path)
        compiler.compile()