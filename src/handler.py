# Makes good looking errors and stuff

from colorama import Fore, Style

class err_info:
    def __init__(self):
        # unkown - idk
        # lexer
        # parser
        self.area = "unkown"

        self.length = 0

        self.file = ""
        
        # If not none, then it is file location
        # [row, col]
        self.loc = None

        # Text message to display error
        self.message = ""

        self.intensity = "error"

def print_error(e=err_info):
    print(f"{Fore.RED}[ERROR]:{Style.RESET_ALL}{e.message}")
    
    loc_str = ""
    if e.loc is None:
        loc_str = "?:?"
    else:
        if e.loc[0] == -1:
            loc_str += '?'
        else:
            loc_str += str(e.loc[0])
        loc_str += ':'
        if e.loc[1] == -1:
            loc_str += '?'
        else:
            loc_str += str(e.loc[1])
    
    print(f"-->{e.file}:{loc_str}")

    if e.loc is None or e.loc[0] == -1:
        return None
    
    all_lines =  open(e.file).readlines()
    minl = 0
    maxl = 0

    if e.loc[0] == 1:
        minl = 0
    else:
        minl = e.loc[0] - 1
    if e.loc[0] == len(all_lines):
        maxl = len(all_lines)
    else:
        maxl = e.loc[0] + 1
    
    lines = all_lines[minl:maxl]
    i = minl + 1
    for x in lines:
        ol = f"{str(i)} | "
        if i == e.loc[0]:
            jl = ""
            for n in range(len(ol) + e.loc[1] - 1):
                jl += " "
            for n in range(e.length):
                jl += "^"
            ol += x
            print(ol)
            print(jl)
        else:
            ol += x
            print(ol)
        i+=1
