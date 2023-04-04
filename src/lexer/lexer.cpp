#include "lexer.hpp"

#include "../shared/print.hpp"

#define SINGLE_CHAR_SYMBOL(a, b) case a: \
tkn.m_type = b;\
break;

lexer::lexer()
{
    m_src = nullptr;
}

lexer::~lexer()
{
    print_verbose("Deleting lexer");
    delete this->tokens;
}

bool lexer::tokenize()
{
    if(m_src == nullptr)
    {
        print_verbose("Cannot tokenize no src file");
        return false;
    }

    print_verbose("Started tokenization");

    _preprocess();

    this->tokens = new std::vector<token>;

    while(this->_position < _content.length() && _good) {
        _advance();
    }

    if(!_good){
        return false;
    }

    return true;
}

void lexer::_make_error(const std::string& msg)
{
    // Assume the error is at the current token
    std::string prep = this->m_src->filename + ':' + std::to_string(this->_line) + ':' + std::to_string(this->_col);
    print_error(prep + ' ' + msg);
    this->_good = false;
}

void lexer::_preprocess()
{
    // For the moment this only removes the comments
    for(const std::string& line: *m_src->get_lines())
    {
        std::string new_line = line;
        int i = 0;
        bool should_continue = true;
        while(i < line.length() && should_continue){
            char c = line[i];
            if(c == '#'){
                should_continue = false;
                new_line = line.substr(0, i);
            }
            i+=1;
        }
        new_line += '\n';
        _content += new_line;
    }
}

void lexer::_add_token(token& tkn)
{
    tkn.m_id = this->tokens->size();
    this->tokens->push_back(tkn);
}

location lexer::_get_loc()
{
    location loc;
    loc.col = _col;
    loc.line = _line;
    loc.src_ptr = m_src;
    return loc;
}

void lexer::_advance()
{
    // Does not check for out of range yet
    const char& c = _content[_position];

    if(_skip_space()) {}
    else if(_get_symbol()){}
    else if(_get_char_lit()){}
    else if(_get_string_lit()){}
    else if(_get_number()){}
    else if(_get_word()){}
    else{_make_error("Cannot tokenize character.");}
}

void lexer::_next()
{
    this->_position++;
    this->_col++;
}

bool lexer::_skip_space()
{
    char& c = _content[_position];
    if(isspace(c))
    {
        while(_position < _content.size() &&isspace(_content[_position])){
            if(_content[_position] == '\n'){
                _line++;
                _col = 0; // is 0 but will be set to 1 because it is incremented anyway
            }
            _position++;
            _col++;
        }
        return true;
    }
    return false;
}

bool lexer::_get_symbol()
{
    char& c = _content[_position];
    
    token tkn;
    tkn.m_type = UNKNOWN;
    word w;
    w.loc = _get_loc();
    
    // One-Character Symbols
    switch(c)
    {
        SINGLE_CHAR_SYMBOL('+',TKN_TYPE::MATH_OP)
        SINGLE_CHAR_SYMBOL('/',TKN_TYPE::MATH_OP)
        SINGLE_CHAR_SYMBOL('*',TKN_TYPE::MATH_OP)
        SINGLE_CHAR_SYMBOL('-',TKN_TYPE::MATH_OP) // Could be begining of an arrow
        SINGLE_CHAR_SYMBOL('[',TKN_TYPE::LBRACKET)
        SINGLE_CHAR_SYMBOL(']',TKN_TYPE::RBRACKET)
        SINGLE_CHAR_SYMBOL('{',TKN_TYPE::LBRACE)
        SINGLE_CHAR_SYMBOL('}',TKN_TYPE::RBRACE)
        SINGLE_CHAR_SYMBOL('(',TKN_TYPE::LPAREN)
        SINGLE_CHAR_SYMBOL(')',TKN_TYPE::RPAREN)
        SINGLE_CHAR_SYMBOL(':',TKN_TYPE::COLON)
        SINGLE_CHAR_SYMBOL(';',TKN_TYPE::SEMICOLON)
        SINGLE_CHAR_SYMBOL('.',TKN_TYPE::PERIOD)
        SINGLE_CHAR_SYMBOL(',',TKN_TYPE::COMMA)
        SINGLE_CHAR_SYMBOL('=',TKN_TYPE::EQUAL)
        SINGLE_CHAR_SYMBOL('<',TKN_TYPE::SMALLER)
        SINGLE_CHAR_SYMBOL('>',TKN_TYPE::GREATER)
    }

    if(tkn.m_type != UNKNOWN)
    {
        w.data = c;

        // Double Character symbol
        if(_position != this->_content.size() - 1)
        {
            if(c=='-' && _content[_position+1] == '>')
            {
                tkn.m_type = TKN_TYPE::SCALAR_ARROW;
                w.data = "->";
                _next();
            }
            else if(c=='=' && _content[_position+1] == '>')
            {
                tkn.m_type = TKN_TYPE::RIGHT_BIG_ARROW;
                w.data = "=>";
                _next();
            }
            else if(c == '<' && _content[_position+1] == '=')
            {
                tkn.m_type = TKN_TYPE::LEFT_BIG_ARROW;
                w.data = "<=";
                _next();
            }
        }

        tkn.m_word = w;
        _add_token(tkn);
        _next();

        return true;
    }

    return false;
}

// 'c'
bool lexer::_get_char_lit()
{
    char& c = _content[_position];
    
    if(c == '\'')
    {
        // For special characters like '\n'
        if(_position <= _content.size() - 3 && _content[_position + 2] == '\'' && _content[_position + 1] == '\\')
        {
            token tkn;
            word w;
            w.loc = _get_loc();
            w.data = "'";
            w.data += _content[_position + 1] + _content[_position + 2];
            w.data += "'";

            _add_token(tkn);
            _next(); // '
            _next(); // escape-character
            _next(); // n
            _next(); // '

            return true;
        }

        // For simple characters
        if(_position <= _content.size() - 2)
        {
            if(_content[_position + 1] == '\n'){
                _next();
                _make_error("Cannot make multiline character literal.");
                return true; // Returning true to skip
            }
            if(_content[_position + 1] == '\''){
                _next();
                _make_error("Cannot have empty character literal.");
                return true;
            }
            if(_content[_position + 2] != '\''){
                _next();
                _next();
                _make_error("Expected ending single quote.");
                return true;
            }

            token tkn;
            word w;
            w.loc = _get_loc();
            w.data += "'";
            w.data += _content[_position + 1];
            w.data += "'";
            tkn.m_word = w;
            tkn.m_type = CHAR_LIT;

            _add_token(tkn);
            _next(); // '
            _next(); // c
            _next(); // '

            return true;
        }
        _make_error("Cannot finish character literal.");
    }
    
    return false;
}

// "text"
bool lexer::_get_string_lit()
{
    char& c = _content[_position];

    if(c == '"')
    {
        _make_error("String literals are not implemented yet.");
        return true;
    }

    return false;
}

// 1.0
// 1.0f
// 10
bool lexer::_get_number()
{
    char& c = _content[_position];

    if(isdigit(c))
    {
        // double or float literal
        if(_position < _content.size() - 1 && _content[_position + 1] == '.')
        {
            if(_position == _content.size() - 2 || !isdigit(_content[_position + 2])){
                _make_error("Cannot finish number literal");
                return true;
            }

            token tkn;
            word w;
            w.data = _content[_position];
            w.data += '.';
            w.loc = _get_loc();
            _next();
            _next();

            while(_position < _content.size() && isdigit(_content[_position]))
            {
                w.data += _content[_position];
                _next();
            }

            tkn.m_type = TKN_TYPE::DOUBLE_LIT;

            if(_position < _content.size() && _content[_position] == 'f')
            {
                tkn.m_type = TKN_TYPE::FLOAT_LIT;
                w.data += 'f';
                _next();
            }

            tkn.m_word = w;
            _add_token(tkn);

            return true;
        }

        // int literal
        token tkn;
        word w;
        w.loc = _get_loc();

        while(_position < _content.size() && isdigit(_content[_position]))
        {
            w.data += _content[_position];
            _next();
        }

        tkn.m_type = TKN_TYPE::INT_LIT;
        tkn.m_word = w;
        _add_token(tkn);

        return true;
    }

    return false;
}

bool lexer::_get_word()
{
    char& c = _content[_position];

    if(isalpha(c))
    {
        word w;
        w.loc = _get_loc();

        while(_position < _content.size() && isalpha(_content[_position]))
        {
            w.data += _content[_position];
            _next();
        }

        token tkn;
        tkn.m_word = w;
        tkn.m_type = TKN_TYPE::UNKNOWN;

        if(w.data == "true" || w.data == "false")
        {
            tkn.m_type = TKN_TYPE::BOOL_LIT;
        }
        else
        {
            tkn.m_type = TKN_TYPE::IDENTIFIER;
        }

        _add_token(tkn);

        return true;
    }

    return false;
}