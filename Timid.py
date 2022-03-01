import sys
import subprocess
import Utility.tGlobals as tGlobals
from Machines.tCompiler import Compiler

class Timid:
    @staticmethod
    def print_usage():
        print("Usage: Timid files")

    @staticmethod
    def start():
        if len(sys.argv) < 2: #Check if user entered correct parameters
            Timid.print_usage()
            exit(64)

        filenames = sys.argv[1:] #Ignore first argument

        for filename in filenames:
            Timid.run(filename)
            new_path = filename.split('\\')[-1].split('.')[0] + '.timb'
            subprocess.run([".\\TimidRuntime.exe", new_path])
            tGlobals.has_error = False #Reset error flag

    @staticmethod
    def run(path): #Run method
        compiler = Compiler(path)
        compiler.compile()

if __name__ == "__main__":
    Timid.start()