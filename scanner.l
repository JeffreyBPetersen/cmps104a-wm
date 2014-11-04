%{
// Jeffrey Petersen | jebpeter@ucsc.edu
#include <iostream>
using namespace std;
%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

%%

\[\] { cout << "TOK_ARRAY{" << yytext << "}\n"; }
\( { cout << "open_paren{" << yytext << "}\n"; }
\) { cout << "close_paren{" << yytext << "}\n"; }
\[ { cout << "open_bracket" << yytext << "}\n"; }
\] { cout << "close_bracket{" << yytext << "}\n"; }
{ { cout << "open_brace{" << yytext << "}\n"; }\
} { cout << "close_brace{" << yytext << "}\n"; }
; { cout << "semicolon{" << yytext << "}\n"; }
, { cout << "comma{" << yytext << "}\n"; }
\. { cout << "period{" << yytext << "}\n"; }
= { cout << "equals{" << yytext << "}\n"; }
== { cout << "TOK_EQ{" << yytext << "}\n"; }
!= { cout << "TOK_NE{" << yytext << "}\n"; }
\< { cout << "TOK_LT{" << yytext << "}\n"; }
\<= { cout << "TOK_LE{" << yytext << "}\n"; }
> { cout << "TOK_GT{" << yytext << "}\n"; }
>= { cout << "TOK_GE{" << yytext << "}\n"; }
\+ { cout << "plus{" << yytext << "}\n"; }
- { cout << "minus{" << yytext << "}\n"; }
\* { cout << "asterisk{" << yytext << "}\n"; }
\/ { cout << "forward_slash{" << yytext << "}\n"; }
% { cout << "modulus{" << yytext << "}\n"; }
! { cout << "exclamation{" << yytext << "}\n"; }
void { cout << "TOK_VOID{" << yytext << "}\n"; }
bool { cout << "TOK_BOOL{" << yytext << "}\n"; }
char { cout << "TOK_CHAR{" << yytext << "}\n"; }
int { cout << "TOK_INT{" << yytext << "}\n"; }
string { cout << "TOK_STRING{" << yytext << "}\n"; }
struct { cout << "TOK_STRUCT{" << yytext << "}\n"; }
if { cout << "TOK_IF{" << yytext << "}\n"; }
else { cout << "TOK_ELSE{" << yytext << "}\n"; }
while { cout << "TOK_WHILE{" << yytext << "}\n"; }
return { cout << "TOK_RETURN{" << yytext << "}\n"; }
false { cout << "TOK_FALSE{" << yytext << "}\n"; }
true { cout << "TOK_TRUE{" << yytext << "}\n"; }
null { cout << "TOK_NULL{" << yytext << "}\n"; }
ord { cout << "TOK_ORD{" << yytext << "}\n"; }
chr { cout << "TOK_CHR{" << yytext << "}\n"; }
new { cout << "TOK_NEW{" << yytext << "}\n"; }
[a-zA-Z_][a-zA-Z0-9_]* { cout << "TOK_IDENT{" << yytext << "}\n"; }
[0-9]+ { cout << "TOK_INTCON{" << yytext << "}\n"; }
('([^\\'\n]|\\[\\'"0nt])') { cout << "TOK_CHARCON{" << yytext << "}\n"; }
(\"([^\\"\n]|\\[\\'"0nt])*\") { cout << "TOK_STRINGCON{" << yytext << "}\n"; }
#[ ][0-9]+[ ](\"([^\\"\n]|\\[\\'"0nt])*\") { cout << "cpp_directive{" << yytext << "}\n"; }
[ \t\n] { }

%%
