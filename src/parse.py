import lexer
import handler

class node:
    def __init__(self, name, tkn=None):
        self.name = name
        self.token = None
        self.children = []
        if tkn is not None:
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
        self.filename = ""
        self.errors = []
    def match(self, t):
        if self.good:
            if self.pos >= 0 and self.pos < len(self.tokens):
                if self.tokens[self.pos].type == t:
                    self.pos += 1
                    return True
            return False
        print("Match: Parser not safe mode")
        return False
    def expect(self, t):
        if self.good:
            if self.match(t):
                return True
            else:
                self.good = False
                print("Unexpected token")
        else:
            print("Expect: Parser not safe mode")
    def now(self):
        if self.pos >= 0 and self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return lexer.token('eof')
    def peekb(self):
        if self.pos > 0 and self.pos <= len(self.tokens):
            return self.tokens[self.pos-1]
        return lexer.token('eof')
    def make_error(self, msg=str):
        e = [self.pos, self.now().loc, msg]
        self.errors.append(e)

    def dump_err_info(self):
        errs = []
        for x in self.errors:
            e = handler.err_info()
            e.area = "parser"
            e.file = self.filename
            e.loc = x[1]
            e.message = x[2]
            e.intensity = "error"
            e.length = x[0]
            errs.append(e)
        return errs

def parse_declspec(p=parser, n=node):
    p.where = "declspec"
    if p.match(t='datatype'):
        n.children.append(node('datatype',p.peekb()))
        return True
    return False

def parse_arr_decl(p=parser, n=node):
    n = node('undef')
    p.where = "arr_decl"
    if not parse_declspec(p,n):
        return False
    return True

def parse(tkns, filename):
    root = node('root')
    p = parser(tkns)
    p.filename = filename
    
    lp = 0
    lc = 0 # loop counter 
    while p.pos < len(p.tokens):
        n = node('undef')
        if parse_arr_decl(p,n):
            root.children.append(n)
        else:
            p.make_error(
                f"Expected top-level statement, not token \"{p.now().word}\" "
                )
            break
        # Loop check
        if lp == p.pos:
            lc += 1
        else:
            lc = 0
        lp = p.pos
        if lc >= 3:
            print("INFINITE LOOP STOPPED")
            break
    
    for e in p.dump_err_info():
        handler.print_error(e)
    
    print('{' + str(root) + '}')
    return root