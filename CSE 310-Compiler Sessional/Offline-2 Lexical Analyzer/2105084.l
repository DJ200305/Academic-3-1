%option noyywrap
%x STATE_FOR_CHARS
%x STATE_FOR_STRINGS
%x STATE_FOR_COMMMENTS1
%x STATE_FOR_COMMMENTS2
%{
   #include "bits/stdc++.h"
   #include "2105084_symbol_table.hpp"
   using namespace std;
   int line_cnt = 1;
   int error_cnt = 0;
   int curr_len = 0;
   int curr_len1 = 0;
   string text = "";
   string text1 = "\"";   
   string comments1 = "//";
   string comments2 = "/*";
   string charcheck = "";
   string charcheck1 = "";  
   ofstream out("output3_token.txt");
   ofstream lout("output3_log.txt");

SymbolTable* st = new SymbolTable(7,"SDBM",lout);

class lexeme_token{
    public:
        string lexeme;
        string token;
        lexeme_token(string lexeme, string token){
            this->lexeme = lexeme;
            this->token = token;
            
        }
        string to_string(){
            return "<"+token + ", " + lexeme+"> ";
        }
};

void capitalize(string &str) {
    for (char &c : str) {
        c = toupper(c);
    }
}

lexeme_token keywords(string text){
    string token = text;
    string lexeme = text;
    capitalize(token);
    return lexeme_token(lexeme, token);
}

void write_to_log(lexeme_token lt){
    lout << "Line no "<<line_cnt<<": Token "<<"<" << lt.token << "> "<<"Lexeme " << lt.lexeme <<" found"<< endl;
    lout<<endl;
}

void write_to_token(lexeme_token lt){
    out << lt.to_string();
}
void write_to_token_keywords(lexeme_token lt){
    out << "<" << lt.token << "> ";
}

void insertion(lexeme_token lt){
    bool done = st->Insert(lt.lexeme,lt.token);
    if(!done) {
        //lout<<"Line no "<<line_cnt<<": Error: "<<lt.lexeme<<" already declared in the current scope."<<endl;
        return;
    }
    if(lt.token == "ID" || lt.token == "CONST_INT" || lt.token == "CONST_FLOAT" || lt.token == "CONST_CHAR"){
        st->PrintAllScope();
        lout<<endl;
    }
}
    
%}

KEYWORDS "if"|"else"|"goto"|"for"|"while"|"long"|"do"|"break"|"short"|"int"|"char"|"static"|"float"|"double"|"unsigned"|"void"|"return"|"switch"|"case"|"default"|"continue"
ADDOP "+"|"-"
MULOP "*"|"/"|"%"
INCOP "++"|"--"
RELOP "<"|"<="|">"|">="|"=="|"!="
ASSIGNOP "="
LOGICOP "&&"|"||"
NOT "!"
LPAREN "("
RPAREN ")"
LCURL "{"
RCURL "}"
LTHIRD "["
RTHIRD "]"
COMMA ","
SEMICOLON ";"
WHITESPACE [ \t\n\v]+
ALPHABET [a-zA-Z]
DIGIT [0-9]
ALPHANUMERIC {ALPHABET}|{DIGIT}
CONST_INT {DIGIT}+
DECIMAL_FRACTION \.{DIGIT}+
FLOAT_NUMBER {DIGIT}*{DECIMAL_FRACTION}
EXPONENT [eE][+-]?{DIGIT}+
CONST_FLOAT ({DIGIT}+|{FLOAT_NUMBER}){EXPONENT}?
DECIMAL_FLOAT_ERRORS {DIGIT}*(\.{DIGIT}*){2,}{EXPONENT}?
FRACTIONAL_EXPONENT [eE][+-]?{DIGIT}*{DECIMAL_FRACTION}+
ILLFORMED_NUMBER ({DIGIT}+|{FLOAT_NUMBER}){FRACTIONAL_EXPONENT}
IDENTIFIER ({ALPHABET}|"_")({ALPHANUMERIC}|"_")*
INVALID_ID_SUFFIX_NUM_PREFIX ({DIGIT}+|{FLOAT_NUMBER}){IDENTIFIER}
BACKSLASH "\\"

SINGLE_QUOTE "\'"
SPECIAL_CHARACTER "\\0"|"\\v"|"\\b"|"\\r"|"\\f"|"\\a"|"\\\'"|"\\\\"|"\\t"|"\\n"|"\\\""

DOUBLE_QUOTE "\""
NEWLINE "\n"|"\r\n"
MULTICHAR_CONST '({ALPHABET}|{SPECIAL_CHARACTER}|{DIGIT}){2,}'
EMPTY_CHARACTER "\'\'"

%%



{NEWLINE} {
    line_cnt++;
}

{WHITESPACE} {
    //printf("Whitespace: %s\n", yytext);
    for(int i=0;i<strlen(yytext);i++){
        if(yytext[i] == '\n'){
            line_cnt++;
        }
    }
}


{DECIMAL_FLOAT_ERRORS} {
    lout<<"Error at line no "<<line_cnt<<": "<<"Too many decimal points "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
}

{EMPTY_CHARACTER} {
    lout<<"Error at line no "<<line_cnt<<": "<<"Empty character constant error "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
}

{ILLFORMED_NUMBER} {
    lout<<"Error at line no "<<line_cnt<<": "<<"Ill formed number "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
    
}

{INVALID_ID_SUFFIX_NUM_PREFIX} {
    lout<<"Error at line no "<<line_cnt<<": "<<"Invalid prefix on ID or invalid suffix on Number "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
    //line_cnt++;
}

{MULTICHAR_CONST} {
    //printf("Multi character constant: %s\n", yytext);
    lout<<"Error at line no "<<line_cnt<<": "<<"Multi character constant error "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
    //line_cnt++;
}

{KEYWORDS} {
    
    write_to_token_keywords(keywords(yytext));
    write_to_log(keywords(yytext));
}
{IDENTIFIER} {
    lexeme_token lt(yytext, "ID");
    write_to_token(lt);
    write_to_log(lt);
    insertion(lt);
    //insertion(lt);
}
{CONST_INT} {
    lexeme_token lt(yytext, "CONST_INT");
    write_to_token(lt);
    write_to_log(lt);
    insertion(lt);
    //insertion(lt);
}
{CONST_FLOAT} {
    lexeme_token lt(yytext, "CONST_FLOAT");
    write_to_token(lt);
    write_to_log(lt);
    insertion(lt);
    //insertion(lt);
}
{ADDOP} {
    lexeme_token lt(yytext, "ADDOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{MULOP} {
    lexeme_token lt(yytext, "MULOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{INCOP} {
    lexeme_token lt(yytext, "INCOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{RELOP} {
    lexeme_token lt(yytext, "RELOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{ASSIGNOP} {
    lexeme_token lt(yytext, "ASSIGNOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{LOGICOP} {
    lexeme_token lt(yytext, "LOGICOP");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{NOT} {
    lexeme_token lt(yytext, "NOT");
    write_to_token(lt);
    write_to_log(lt);
    //insertion(lt);
}
{LPAREN} {
    lexeme_token lt(yytext, "LPAREN");
    write_to_token(lt);
    write_to_log(lt);
    
}
{RPAREN} {
    lexeme_token lt(yytext, "RPAREN");
    write_to_token(lt);
    write_to_log(lt);
}
{LCURL} {
    lexeme_token lt(yytext, "LCURL");
    write_to_token(lt);
    write_to_log(lt);
    st->EnterScope();
}
{RCURL} {
    lexeme_token lt(yytext, "RCURL");
    write_to_token(lt);
    write_to_log(lt);
    st->ExitScope();
    
}
{LTHIRD} {
    lexeme_token lt(yytext, "LTHIRD");
    write_to_token(lt);
    write_to_log(lt);
}
{RTHIRD} {
    lexeme_token lt(yytext, "RTHIRD");
    write_to_token(lt);
    write_to_log(lt);
}
{COMMA} {
    lexeme_token lt(yytext, "COMMA");
    write_to_token(lt);
    write_to_log(lt);
}
{SEMICOLON} {
    lexeme_token lt(yytext, "SEMICOLON");
    write_to_token(lt);
    write_to_log(lt);
}

{SINGLE_QUOTE} {
    //printf("Single quote: %s\n", yytext);
    BEGIN STATE_FOR_CHARS;
    charcheck = yytext;
}
{DOUBLE_QUOTE} {
    curr_len = line_cnt;    
    //printf("Double quote: %s\n", yytext);
    BEGIN STATE_FOR_STRINGS;
}
"//" {
    
    BEGIN STATE_FOR_COMMMENTS1;
}   
"/*" {
    BEGIN STATE_FOR_COMMMENTS2;
    curr_len1 = line_cnt;
}


<STATE_FOR_CHARS>{
    {SINGLE_QUOTE} {
        if(charcheck.length() > 3){
            charcheck += yytext;
            lout<<"Error at line no "<<line_cnt<<": "<<"Multi character constant error "<<charcheck<<endl;
            lout<<endl;
            error_cnt++;
        }
        else{
            charcheck += yytext;
            lexeme_token lt(charcheck1, "CONST_CHAR");
            if(charcheck1 == "'\\n'" || charcheck1 == "'\\a'" || charcheck1 == "'\\f'" || charcheck1 == "'\\r'" || charcheck1 == "'\\b'" || charcheck1 == "'\\v'" || charcheck1 == "'\\\\'" || charcheck1 == "'\\\"'" || charcheck1 == "'\\t'" || charcheck1 == "'\\0'"){
                lout<<"Line no "<<line_cnt<<": Token "<<"<" << "CONST_CHAR" << "> "<<"Lexeme " << charcheck <<" found --> "<<"<CONST_CHAR, "<<charcheck1<<">"<< endl;
            }
            else{
                lout<<"Line no "<<line_cnt<<": Token "<<"<" << "CONST_CHAR" << "> "<<"Lexeme " << charcheck <<" found --> "<<"<CONST_CHAR, "<<charcheck1<<">"<< endl;
            }
            write_to_token(lt);
            lout<<endl;
            lexeme_token lt1(charcheck, "CONST_CHAR");
            insertion(lt1);

        }
        charcheck = "";
        charcheck1 = "";
        BEGIN INITIAL;
    }

    {NEWLINE} {
        charcheck += yytext;    
        lout<<"Error at line no "<<line_cnt<<": "<<"Unterminated character "<<charcheck<<endl;
        lout<<endl;
        error_cnt++;
        line_cnt++;
        charcheck = "";
        BEGIN INITIAL;
    }
    
    {SPECIAL_CHARACTER}{NEWLINE} {
        charcheck += yytext;
        lout<<"Error at line no "<<line_cnt<<": "<<"Unterminated character "<<charcheck<<endl;
        lout<<endl;
        error_cnt++;
        line_cnt++;
        charcheck = "";
        BEGIN INITIAL;
    }

    <<EOF>> {
        charcheck += yytext;
        lout<<"Error at line no "<<line_cnt<<": "<<"Unterminated character "<<charcheck<<endl;
        lout<<endl;
        error_cnt++;
        charcheck = "";
        //line_cnt++;
        BEGIN INITIAL;
    }
    
    {SPECIAL_CHARACTER} {
        charcheck += yytext;
        //printf("Special character: %s\n", yytext);
        char c = yytext[1];
        string token = "CHAR_CONST";
        string lexeme = "";
        string lexeme1 = "";
        lexeme1 +="'";
        lexeme1 += "\\";
        lexeme1 += c;
        lexeme1 +="'";
        if(c == 'n'){
            charcheck1 += '\n';
            
        }
        else if(c == 'a'){
            charcheck1 += '\a';
        }
        else if(c == 'f'){
            charcheck1 += '\f';
        }
        else if(c == 'r'){
            charcheck1 += '\r';
        }
        else if(c == 't'){
            charcheck1 += '\t';
        }
        else if(c == '0'){
            charcheck1 += '\0';
        }
        else if(c == 'b'){
            charcheck1 += '\b';
        }
        else if(c == 'v'){
            charcheck1 += '\v';
        }
        
        else if(c == '\\'){
            charcheck1 += '\\';
        }
        else if(c == '\"'){
            charcheck1 += '\"';
        }
        /* else{
            lexeme += '#';
        }
        lexeme_token lt(lexeme, token);
        string token1 = "CONST_CHAR";
        lexeme_token lt1(lexeme1, token1);
        write_to_token(lt);
        if(lexeme1 == "'\\n'" || lexeme1 == "'\\a'" || lexeme1 == "'\\f'" || lexeme1 == "'\\r'" || lexeme1 == "'\\b'" || lexeme1 == "'\\v'" || lexeme1 == "'\\\\'" || lexeme1 == "'\\\"'" || lexeme1 == "'\\t'" || lexeme1 == "'\\0'"){
            lout << "Line no "<<line_cnt<<": Token "<<"<" << "CONST_CHAR" << "> "<<"Lexeme " << lt1.lexeme <<" found --> "<<"<CONST_CHAR, "<<lexeme<<">"<< endl;
            lout<<endl;
            

        }
        insertion(lt1); */
        
    }
    
    [a-zA-Z0-9] {
        charcheck += yytext;
        charcheck1 += yytext;
    }
    
    [ ] {
        charcheck += yytext;
    }
}

<STATE_FOR_STRINGS>{
    {DOUBLE_QUOTE} {
        string token = "STRING";
        lexeme_token lt(text, token);
        write_to_token(lt);
        text1 += "\"";
        lexeme_token lt1(text1, "STRING");
        lout << "Line no "<<line_cnt<<": Token "<<"<" << lt1.token << "> "<<"Lexeme " <<lt1.lexeme<<" found --> "<<"<"<<lt.token<<", "<<lt.lexeme<<">"<< endl;
        lout<<endl;
        text = "";
        text1 = "\"";
        curr_len = 0;
        BEGIN INITIAL;
    }
    
    {NEWLINE} {
        lout<<"Error at line no "<<curr_len<<": "<<"Unterminated string "<<text1<<endl;
        lout<<endl;
        error_cnt++;
        line_cnt++;
        text = "";
        text1 = "\"";
        curr_len = 0;
        BEGIN INITIAL;
    }

    <<EOF>> {
        lout<<"Error at line no "<<curr_len<<": "<<"Unterminated string "<<text1<<endl;
        lout<<endl;
        error_cnt++;
        text = "";
        text1 = "\"";
        curr_len = 0;
        BEGIN INITIAL;
    }

    {BACKSLASH}{DOUBLE_QUOTE} {
        text += "\"";
        text1 += '\\';
        text1 += yytext[1];
    }
    {BACKSLASH}{NEWLINE} {
        curr_len = line_cnt;
        line_cnt++;
        string token = "STRING";
        text1 += '\\';
        text1 += yytext[1];
    }
    {SPECIAL_CHARACTER} {
        char c = yytext[1];
        if(c == 'n'){
            text += '\n';
            text1 += '\n';
        }
        else if(c == 'a'){
            text += '\a';
            text1 += '\a';
        }
        else if(c == 'f'){
            text += '\f';
            text1 += '\f';
        }
        else if(c == 'r'){
            text += '\r';
            text1 += '\r';
        }
        else if(c == 't'){
            text += '\t';
            text1 += '\t';
        }
        else if(c == '0'){
            text += '\0';
            text1 += '\0';
        }
        else if(c == 'b'){
            text += '\b';
            text1 += '\b';
        }
        else if(c == 'v'){
            text += '\v';
            text1 += '\v';
        }
        else if(c == '\\'){
            text += '\\';
            text1 += '\\';
        }
        else if(c == '\"'){
            text += '\"';
            text1 += '\"';
        }
        else{
            text += '#';
            text1 += '#';
        }
    }
    . {
        text += yytext;
        text1 += yytext;
    }
}
<STATE_FOR_COMMMENTS1>{
    {BACKSLASH}{NEWLINE} {
        line_cnt++;
        comments1 += '\\';
        comments1 += yytext[1];
        
    }
    {NEWLINE} {
        lexeme_token lt(comments1, "COMMENT");
        write_to_log(lt);
        line_cnt++;
        comments1 = "//";
        BEGIN INITIAL;
    }
    
    . {
        comments1 += yytext;
    }
}
<STATE_FOR_COMMMENTS2>{
    "*/" {
        comments2 += "*/";
        lexeme_token lt(comments2, "COMMENT");
        write_to_log(lt);
        comments2 = "/*";
        curr_len1 = 0;
        BEGIN INITIAL;
    }
    
    

    {BACKSLASH}{NEWLINE} {
        
        line_cnt++;
        comments2 += '\\';
        comments2 += yytext[1];
        
    }
    {NEWLINE} {
        
        
        line_cnt++;
        comments2 += yytext[0];
    }

    <<EOF>> {
        //cout<<curr_len1<<endl;
        lout<<"Error at line no "<<curr_len1<<": "<<"Unterminated comment "<<comments2<<endl;
        lout<<endl;
        error_cnt++;
        comments2 = "/*";
        curr_len1 = 0;
        BEGIN INITIAL;
    }
    . {
        
        comments2 += yytext;
    }
}

<<EOF>> {
    st->PrintAllScope();
    lout<<endl;
    lout<<"Total lines: "<<line_cnt<<endl;
    lout<<"Total errors: "<<error_cnt<<endl;
    return 0;
}
. {
    lout<<"Error at line no "<<line_cnt<<": "<<"Unrecognized character "<<yytext<<endl;
    lout<<endl;
    error_cnt++;
}

%%

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file) {
            yyin = file;
            yylex();
            fclose(yyin);
            out.close();
            lout.close();
        } else {
            cerr << "Error opening file: " << argv[1] << endl;
            return 1;
        }
    } else {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    return 0;
}