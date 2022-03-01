import sys
import subprocess
import pathlib
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
            path = pathlib.Path(filename)
            binary_path = path.parents[0] / (path.stem + '.timb')
            Timid.run(path)
            if tGlobals.has_error:
                continue
            subprocess.run([".\\TimidRuntime.exe", binary_path])
            tGlobals.has_error = False #Reset error flag

    @staticmethod
    def run(path): #Run method
        compiler = Compiler(path)
        compiler.compile()

if __name__ == "__main__":
    Timid.start()