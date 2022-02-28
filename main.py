import sys
from Timid import Timid
import tGlobals

def print_usage():
    print("Usage: Timid files")

def main():
    if len(sys.argv) < 2: #Check if user entered correct parameters
        print_usage()
        exit(64)

    filenames = sys.argv[1:] #Ignore first argument

    for filename in filenames:
        Timid.run(filename)
        tGlobals.has_error = False

if __name__ == "__main__":
    main()