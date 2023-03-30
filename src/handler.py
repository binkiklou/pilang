# Makes good looking errors and stuff

class err_info:
    def __init__(self):
        # unkown - idk
        # lexer
        # parser
        self.area = "unkown"

        self.file = ""
        
        # If not none, then it is file location
        self.loc = None

        # Text message to display error
        self.message = ""

        self.intensity = "error"

def print_error(e=err_info):
    pass