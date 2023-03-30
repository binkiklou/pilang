import lexer
import handler

class node:
    def __init__(self, name, tkn=None):
        self.name = name
        self.token = None
        self.children = []
        if type(tkn) != None:
            self.token = tkn
    def __repr__(self):
        # Turns into sketchy json
        txt = f"\"{self.name}_{self.token}\""
        if len(self.children) > 0:
            txt += ":{"
            i =0 
            for c in self.children:
                txt += repr(c)
                if i != len(self.children) - 1:
                    txt +=','
                i+=1
            txt += "}"
        else:
            txt += ":0"
        return txt

class parser:
    def __init__(self, tkns):
        self.tokens = tkns
        self.pos = 0
        self.good = True
        self.state = "OK"
        self.err_info = None
    def match(self, t):
        if self.good:
            if self.pos >= 0 and self.pos < len(self.tokens):
                if self.tokens[self.pos].type == t:
                    self.pos += 1
                    return True
            return False
        else:
            print("Match: Parser not safe mode")
            return False
    def expect(self, t):
        if self.good:
            if self.match(t):
                return True
            else:
                self.good = False
                self.state = "ERR"
                print("Unexpected token")
        else:
            print("Expect: Parser not safe mode")
    def peekb(self):
        if self.pos > 0 and self.pos <= len(self.tokens):
            return self.tokens[self.pos-1]
        return lexer.token('eof')

def parse(tkns):
    root = node('root')
    p = parser(tkns)
    
    lp = 0
    lc = 0 # loop counter 
    while p.pos < len(p.tokens):
        # Loop check
        if lp == p.pos:
            lc += 1
        else:
            lc = 0
        lp = p.pos
        if lc >= 3:
            print("INFINITE LOOP STOPPED")
            break
    print('{' + str(root) + '}')