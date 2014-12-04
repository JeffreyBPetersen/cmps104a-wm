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

%token TOK_FUNCTION TOK_PARAMLIST
%token TOK_DECLID TOK_VARDECL TOK_PROTOTYPE
%token TOK_NEWSTRING TOK_INDEX

%right TOK_IF TOK_ELSE
%right '='
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_ORD TOK_CHR
%left '[' '.'

%start start

%%

start       : program             { yyparse_astree = $1; }
            ;
program     : program structdef   { $$ = adopt1 ($1, $2); }
            | program function    { $$ = adopt1 ($1, $2); }
            | program statement   { $$ = adopt1 ($1, $2); }
            | /* empty */         { $$ = new_parseroot (); }
            ;
structdef   : struct_rep '}'
                                  { free_ast($2);
                                    $$ = $1; }
            ;
struct_rep  : struct_rep fielddecl ';'
                                  { free_ast($3);
                                    adopt1($1, $2); }
            | TOK_STRUCT TOK_IDENT '{'
                                  { free_ast($3);
                                    update_sym($2, TOK_TYPEID);
                                    $$ = adopt1 ($1, $2); }
            ;
fielddecl   : basetype TOK_IDENT  { update_sym($2, TOK_FIELD);
                                    $$ = adopt1($1, $2); }
            | basetype TOK_ARRAY TOK_IDENT
                                  { update_sym($3, TOK_FIELD);
                                    $$ = adopt2($2, $1, $3); }
            ;
basetype    : TOK_VOID            { $$ = $1; }
            | TOK_BOOL            { $$ = $1; }
            | TOK_CHAR            { $$ = $1; }
            | TOK_INT             { $$ = $1; }
            | TOK_STRING          { $$ = $1; }
            | TOK_IDENT           { $$ = update_sym($1, TOK_TYPEID); }
            ;
function    : identdecl paramlist ')' block
                                  { free_ast($3);
                                    $$ = new_function($1, $2, $4); }
            ;
paramlist   : paramlist ',' identdecl
                                  { free_ast($2);
                                    $$ = adopt1($1, $3); }
            | '(' identdecl       { $$ = adopt1sym($1, $2, 
                                       TOK_PARAMLIST); }
            | '('                 { $$ = update_sym($1,
                                       TOK_PARAMLIST); }
            ;
identdecl   : basetype TOK_IDENT  { update_sym($2, TOK_DECLID);
                                    $$ = adopt1($1, $2); }
            | basetype TOK_ARRAY TOK_IDENT
                                  { update_sym($3, TOK_DECLID);
                                    $$ = adopt2($2, $1, $3); }
            ;
block       : block_rep '}'       { $$ = $1; }
            | ';'                 { $$ = $1; }
            ;
block_rep   : block_rep statement { $$ = adopt1($1, $2); }
            | '{'                 { $$ = update_sym($1, TOK_BLOCK); }
            ;
statement   : block               { $$ = $1; }
            | vardecl             { $$ = $1; }
            | while               { $$ = $1; }
            | ifelse              { $$ = $1; }
            | return              { $$ = $1; }
            | expr ';'            { free_ast($2);
                                    $$ = $1; }
            ;
vardecl     : identdecl '=' expr ';'
                                  { free_ast($4);
                                    $$ = adopt2sym($2, $1, $3,
                                       TOK_VARDECL); }
            ;
while       : TOK_WHILE '(' expr ')' statement
                                  { free_ast2($2, $4);
                                    $$ = adopt2($1, $3, $5); }
            ;
ifelse      : TOK_IF '(' expr ')' statement %prec TOK_IF
                                  { free_ast2($2, $4);
                                    $$ = adopt2($1, $3, $5); }
            | TOK_IF '(' expr ')' statement TOK_ELSE statement
                                  { free_ast2($2, $4);
                                    free_ast($6);
                                    adopt2sym($1, $3, $5, TOK_IFELSE);
                                    $$ = adopt1($1, $7); }
            ;
return      : TOK_RETURN expr ';' { free_ast($3);
                                    $$ = adopt1($1, $2); }
            | TOK_RETURN ';'      { free_ast($2);
                                    $$ = $1; }
            ;
expr        : binop               { $$ = $1; }
            | unop                { $$ = $1; }
            | allocator           { $$ = $1; }
            | call                { $$ = $1; }
            | '(' expr ')'        { free_ast2($1, $3);
                                    $$ = $2; }
            | variable            { $$ = $1; }
            | constant            { $$ = $1; }
            ;
binop       : expr '=' expr       { $$ = adopt2($2, $1, $3); }
            | expr TOK_EQ expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_NE expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_LT expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_LE expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_GT expr    { $$ = adopt2($2, $1, $3); }
            | expr TOK_GE expr    { $$ = adopt2($2, $1, $3); }
            | expr '+' expr       { $$ = adopt2($2, $1, $3); }
            | expr '-' expr       { $$ = adopt2($2, $1, $3); }
            | expr '*' expr       { $$ = adopt2($2, $1, $3); }
            | expr '/' expr       { $$ = adopt2($2, $1, $3); }
            | expr '%' expr       { $$ = adopt2($2, $1, $3); }
            ;
unop        : '+' expr            { $$ = adopt1sym($1, $2, TOK_POS); }
            | '-' expr            { $$ = adopt1sym($1, $2, TOK_NEG); }
            | '!' expr            { $$ = adopt1($1, $2); }
            | TOK_ORD expr        { $$ = adopt1($1, $2); }
            | TOK_CHR expr        { $$ = adopt1($1, $2); }
            ;
allocator   : TOK_NEW TOK_IDENT '(' ')'
                                  { free_ast2($3, $4);
                                    update_sym($2, TOK_TYPEID);
                                    $$ = adopt1($1, $2); }
            | TOK_NEW TOK_STRING '(' expr ')'
                                  { free_ast($1);
                                    free_ast2($3, $5);
                                    $$ = adopt1sym($2, $4,
                                       TOK_NEWSTRING); }
            | TOK_NEW basetype '[' expr ']'
                                  { free_ast2($3, $5);
                                    update_sym($2, TOK_TYPEID);
                                    adopt2sym($1, $2, $4,
                                       TOK_NEWARRAY); }
            ;
call        : TOK_IDENT '(' call_rep ')'
                                  { free_ast($4);
                                    $$ = adopt2sym($2, $1, $3,
                                       TOK_CALL); }
            | TOK_IDENT '(' ')'   { free_ast($3);
                                    $$ = adopt1sym($2, $1, TOK_CALL); }
            ;
call_rep    : call_rep ',' expr   { free_ast($2);
                                    $$ = adopt1($1, $3); }
            | expr                { $$ = $1; }
            ;
variable    : TOK_IDENT           { $$ = $1; }
            | expr '[' expr ']'   { free_ast($4);
                                    $$ = adopt2sym($2, $1, $3,
                                       TOK_INDEX); }
            | expr '.' TOK_IDENT  { free_ast($2);
                                    update_sym($3, TOK_FIELD);
                                    $$ = adopt1($1, $3); }
            ;
constant    : TOK_INTCON          { $$ = $1; }
            | TOK_CHARCON         { $$ = $1; }
            | TOK_STRINGCON       { $$ = $1; }
            | TOK_FALSE           { $$ = $1; }
            | TOK_TRUE            { $$ = $1; }
            | TOK_NULL            { $$ = $1; }
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
