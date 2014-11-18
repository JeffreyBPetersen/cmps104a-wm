%{
// Jeffrey Petersen | jebpeter@ucsc.edu

#include "lyutils.h"
#include "assert.h"
#include "astree.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT

%token TOK_BASETYPE TOK_CONSTANT TOK_EXPR

%right '='
%left TOK_LE TOK_GT

%start start

%%

start       : program             { yyparse_astree = $1; }
            ;
program     : program thing       { $$ = adopt1 ($1, $2); }
            | /* empty */         { $$ = new_parseroot (); }
            ;
thing       : basetype            { $$ = $1; }
            | block               { $$ = $1; }
            | expr                { $$ = $1; }
            | token               { $$ = $1; }
            ;
basetype    : TOK_VOID            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_BOOL            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_CHAR            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_INT             { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_STRING          { $$ = update_sym($1, TOK_BASETYPE); }
            ;
block       : block_rep '}'       { $$ = $1; }
            ;
block_rep   : block_rep thing     { $$ = adopt1($1, $2); }
            | '{'                 { $$ = update_sym($1, TOK_BLOCK); }
            ;
expr        : binop               { $$ = update_sym($1, TOK_EXPR); }
            | variable            { $$ = update_sym($1, TOK_EXPR); }
            | constant            { $$ = update_sym($1, TOK_EXPR); }
            ;
binop       : expr '=' expr       { $$ = adopt2($2, $1, $3); }
            | expr TOK_LE expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_GT expr    { $$ = adopt2($2, $1, $3); }
            ;
variable    : TOK_IDENT           { $$ = $1; }
            ;
constant    : TOK_INTCON          { $$ = update_sym($1, TOK_CONSTANT); }
            | TOK_CHARCON         { $$ = update_sym($1, TOK_CONSTANT); }
            | TOK_STRINGCON       { $$ = update_sym($1, TOK_CONSTANT); }
            | TOK_FALSE           { $$ = update_sym($1, TOK_CONSTANT); }
            | TOK_TRUE            { $$ = update_sym($1, TOK_CONSTANT); }
            | TOK_NULL            { $$ = update_sym($1, TOK_CONSTANT); }
            ;
token       : '(' | ')' | '[' | ']' | ';' | ',' | '.'
            | '+' | '-' | '*' | '/' | '%' | '!'
            | TOK_IF | TOK_ELSE | TOK_WHILE | TOK_RETURN | TOK_STRUCT
            | TOK_NEW | TOK_ARRAY
            | TOK_EQ | TOK_NE | TOK_LT | TOK_GE
            | TOK_ORD | TOK_CHR | TOK_ROOT
            ;

%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

static void* yycalloc (size_t size) {
   void* result = calloc (1, size);
   assert (result != NULL);
   return result;
}
