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

%token TOK_BASETYPE TOK_CONSTANT TOK_EXPR TOK_FUNCTION TOK_PARAMLIST TOK_VARDECL

%right '='
%left TOK_LE TOK_GT
%left '-'
%left '*'

%start start

%%

start       : program             { yyparse_astree = $1; }
            ;
program     : program thing       { $$ = adopt1 ($1, $2); }
            | /* empty */         { $$ = new_parseroot (); }
            ;
thing       : function            { $$ = $1; }
            | vardecl             { $$ = $1; }
            | statement           { $$ = $1; }
            | expr                { $$ = $1; }
            | token               { $$ = $1; }
            ;
basetype    : TOK_VOID            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_BOOL            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_CHAR            { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_INT             { $$ = update_sym($1, TOK_BASETYPE); }
            | TOK_STRING          { $$ = update_sym($1, TOK_BASETYPE); }
            ;
function    : identdecl paramlist ')' block
                                  { free_ast($3);
                                    $$ = new_function($1, $2, $4); }
            ;
paramlist   : paramlist identdecl { $$ = adopt1($1, $2); }
            | '('                 { $$ = update_sym($1, TOK_PARAMLIST); }
            ;
identdecl   : basetype TOK_IDENT  { $$ = adopt1($1, $2); }
            ;
block       : block_rep '}'       { $$ = $1; }
            ;
block_rep   : block_rep thing     { $$ = adopt1($1, $2); }
            | '{'                 { $$ = update_sym($1, TOK_BLOCK); }
            ;
statement   : block               { $$ = $1; }
            | while               { $$ = $1; }
            | return              { $$ = $1; }
            | expr ';'            { free_ast($2);
                                    $$ = $1; }
            ;
vardecl     : identdecl '=' expr ';'
                                  { free_ast($4);
                                    $$ = adopt2sym($2, $1, $3, TOK_VARDECL); }
            ;
while       : TOK_WHILE '(' expr ')' statement
                                  { free_ast2($2, $4);
                                    $$ = adopt2($1, $3, $5); }
            ;
return      : TOK_RETURN expr ';' { free_ast($3);
                                    $$ = adopt1($1, $2); }
            | TOK_RETURN ';'      { free_ast($2);
                                    $$ = $1; }
            ;
expr        : binop               { $$ = update_sym($1, TOK_EXPR); }
            | call                { $$ = $1; }
            | variable            { $$ = update_sym($1, TOK_EXPR); }
            | constant            { $$ = $1; }
            ;
binop       : expr '=' expr       { $$ = adopt2($2, $1, $3); }
            | expr TOK_LE expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_GT expr    { $$ = adopt2($2, $1, $3); }
            | expr '-' expr       { $$ = adopt2($2, $1, $3); }
            | expr '*' expr       { $$ = adopt2($2, $1, $3); }
            ;
call        : TOK_IDENT '(' call_rep ')'
                                  { free_ast($4);
                                    $$ = adopt2sym($2, $1, $3, TOK_CALL); }
            | TOK_IDENT '(' ')'   { free_ast($3);
                                    $$ = adopt1sym($2, $1, TOK_CALL); }
            ;
call_rep    : call_rep ',' expr   { free_ast($2);
                                    $$ = adopt1($1, $3); }
            | expr                { $$ = $1; }
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
token       : '[' | ']' | ',' | '.'
            | '+' | '/' | '%' | '!'
            | TOK_IF | TOK_ELSE | TOK_STRUCT
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
