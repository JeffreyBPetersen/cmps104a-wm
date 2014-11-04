%{
// Jeffrey Petersen | jebpeter@ucsc.edu
#include "lyutils.h"
#include "yyparse.h"
%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

%%

\[\] { return yylval_token(TOK_ARRAY); }
\( { return yylval_token('('); }
\) { return yylval_token(')'); }
\[ { return yylval_token('['); }
\] { return yylval_token(']'); }
{ { return yylval_token('{'); }
} { return yylval_token('}'); }
; { return yylval_token(';'); }
, { return yylval_token(','); }
\. { return yylval_token('.'); }
= { return yylval_token('='); }
== { return yylval_token(TOK_EQ); }
!= { return yylval_token(TOK_NE); }
\< { return yylval_token(TOK_LT); }
\<= { return yylval_token(TOK_LE); }
> { return yylval_token(TOK_GT); }
>= { return yylval_token(TOK_GE); }
\+ { return yylval_token('+'); }
- { return yylval_token('-'); }
\* { return yylval_token('*'); }
\/ { return yylval_token('/'); }
% { return yylval_token('%'); }
! { return yylval_token('!'); }
void { return yylval_token(TOK_VOID); }
bool { return yylval_token(TOK_BOOL); }
char { return yylval_token(TOK_CHAR); }
int { return yylval_token(TOK_INT); }
string { return yylval_token(TOK_STRING); }
struct { return yylval_token(TOK_STRUCT); }
if { return yylval_token(TOK_IF); }
else { return yylval_token(TOK_ELSE); }
while { return yylval_token(TOK_WHILE); }
return { return yylval_token(TOK_RETURN); }
false { return yylval_token(TOK_FALSE); }
true { return yylval_token(TOK_TRUE); }
null { return yylval_token(TOK_NULL); }
ord { return yylval_token(TOK_ORD); }
chr { return yylval_token(TOK_CHR); }
new { return yylval_token(TOK_NEW); }
[a-zA-Z_][a-zA-Z0-9_]* { return yylval_token(TOK_IDENT); }
[0-9]+ { return yylval_token(TOK_INTCON); }
('([^\\'\n]|\\[\\'"0nt])') { return yylval_token(TOK_CHARCON); }
(\"([^\\"\n]|\\[\\'"0nt])*\") { return yylval_token(TOK_STRINGCON); }
#[ ][0-9]+[ ](\"([^\\"\n]|\\[\\'"0nt])*\") { scanner_include(); }
[ \t\n] { }

%%
