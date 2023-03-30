class token:
    def __init__(self,t=None,l=None):
        self.type = "undef"
        self.word = ""
        self.loc = [-1,-1]
        if type(t) != None:
            self.type = t
        if type(l) != None:
            self.loc = l
    def __repr__(self) -> str:
        return f"[|{str(self.loc[0])}:{str(self.loc[1])}|({self.type}){self.word}]"

def split_words(line, r):
    col = 1
    row = r
    # Split the sections into token-words
    syms = [
        '[',']','{','}','(',')','-','>','=','>',';',
        ':',';',',','.'
    ]
    words = []
    word = ['',r,0]
    for c in line:
        if c.isspace() or col == len(line) - 1:
            if not c.isspace():
                word[0] += str(c)
            if word[0] != "" and not word[0].isspace():
                words.append(word)
                word = ['',r,0]
        elif c in syms:
            if word[0] != "" and not word[0].isspace():
                words.append(word)
                word = ['',r,0]
            words.append([c,r,col])
        else:
            if word[0] == "":
                word[2] = col
                #s_col = col
            word[0] += str(c)
        col += 1

    # Merge multi-char/word tokens
    tkn_words = []

    i = 0
    while i < len(words):
        w = words[i]
        if w[0][0] == '#': # comments
            break
        elif w[0][0] == '"': # strings
            #new word
            nw = [w[0], w[1], w[2]]
            j = i
            while j < len(words) and words[j][0][len(words[j][0])-1] != '"':
                nw[0] += words[j][0]
                j+=1
            if j < len(words) and j != i:
                nw[0] += words[j][0]
            tkn_words.append(nw) 
        # symbols
        elif w[0] == '-' and i < len(words) - 1:
            if words[i+1][0] == '>':
                tkn_words.append(['->',w[1],w[2]])
                i+=1
        else:
            tkn_words.append(w)

        i+=1

    return tkn_words

def tokenize_line(line, r):
    tokens = []

    words = split_words(line, r)

    # Turn word into empty tokens
    for w in words:
        tkn = token('undef',[w[1],w[2]])
        tkn.word = w[0]
        tokens.append(tkn)
    
    # Give the tpes
    symbols = {
        '[':'lbracket',
        ']':'rbracket',
        '{':'lbrace',
        '}':'rbrace',
        '(':'lparen',
        ')':'rparen',
        ':':'colon',
        ';':'semicolon',
        '.':'period',
        ',':'comma',
        '->':'scalar_eq',
        '=>':'arr_eq'
    }
    datatypes = ['int','char','bool']
    keywords = ['col','row','scalar','shape','range']

    for t in tokens:
        if t.word in symbols:
            t.type = symbols[t.word]
        elif t.word in datatypes:
            t.type = 'datatype'
        elif t.word in keywords:
            t.type = t.word
        else:
            if t.word.isnumeric():
                t.type = 'num_lit'
            else:
                if t.word == 'true' or t.word == 'false':
                    t.type = 'bool_lit'
                elif  t.word[0] == '"':
                    t.type = 'str_lit'
                else:
                    t.type = 'ident'
    return tokens

def tokenize(lines):
    tokens = []

    i = 1 
    for x in lines:
        tokens += tokenize_line(x, i)
        i += 1

    return tokens