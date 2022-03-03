import os
import sys, subprocess, pathlib, getopt
import Utility.tGlobals as tGlobals
from Machines.tCompiler import Compiler

class Timid:
    VERSION = 1
    COMPILE_ONLY = False
    PRINT_VERSION = False
    PRINT_HELP = False
    DEV_MODE = False

    @staticmethod
    def print_version():
        print("Timid Compiler version %d" % Timid.VERSION)

    @staticmethod
    def print_usage():
        Timid.print_version()
        print("Usage: Timid [-c | --compile] [-d | --dev] [-h | --help] [-v | --version] <.timid files>")
        print("Options:")
        print("-c, --compile:\tcompiles program without running it")
        print("-d, --dev:\tenables debug messages")
        print("-h, --help:\tprints this help message")
        print("-v, --version:\tprints Timid's version")

    @staticmethod
    def start():
        if len(sys.argv) < 2: #Check if user entered correct parameters
            Timid.print_usage()
            exit(64)

        try:
            options, filenames = getopt.getopt(sys.argv[1:], 'cdhv', ['compile', 'help', 'version', 'dev']) #Ignore first argument
        except getopt.GetoptError as e:
            print("ERROR: ", e)
            sys.exit(1),

        for option, arg in options:
            if option in ('-c', '--compile'):
                Timid.COMPILE_ONLY = True
            if option in ('-d', '--dev'):
                Timid.DEV_MODE = True
                tGlobals.dev_mode = True
            elif option in ('-h', '--help'):
                Timid.PRINT_HELP = True
                Timid.print_usage()
                exit(64)
            elif option in ('-v', '--version'):
                Timid.PRINT_VERSION = True
                Timid.print_version()
                exit(64)

        for filename in filenames:
            path = pathlib.Path(filename)
            binary_path = path.parents[0] / (path.stem + '.timb')
            Timid.run(path)
            if tGlobals.has_error:
                continue

            if not Timid.COMPILE_ONLY:
                args = [".\\TimidRuntime"]
                if Timid.DEV_MODE: args.append('-d')
                if Timid.PRINT_HELP: args.append('-h')
                if Timid.PRINT_VERSION: args.append('-v')
                args.append(binary_path)

                subprocess.run(args)
            tGlobals.has_error = False #Reset error flag

    @staticmethod
    def run(path): #Run method
        compiler = Compiler(path)
        compiler.compile()

if __name__ == "__main__":
    Timid.start()