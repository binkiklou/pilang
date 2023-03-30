import sys
import lexer
import parse

if len(sys.argv) == 1:
    print("Specify a file bozo")
    sys.exit(-1)

# Tokenization
file = open(sys.argv[1])
tokens = lexer.tokenize(file.readlines())

print(tokens)

# Parsing
print('-----PARSING-----')
tree = parse.parse(tokens)