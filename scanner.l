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

\[\] { scanner_useraction();
return yylval_token(TOK_ARRAY); }
\( { scanner_useraction();
return yylval_token('('); }
\) { scanner_useraction();
return yylval_token(')'); }
\[ { scanner_useraction();
return yylval_token('['); }
\] { scanner_useraction();
return yylval_token(']'); }
{ { scanner_useraction();
return yylval_token('{'); }
} { scanner_useraction();
return yylval_token('}'); }
; { scanner_useraction();
return yylval_token(';'); }
, { scanner_useraction();
return yylval_token(','); }
\. { scanner_useraction();
return yylval_token('.'); }
= { scanner_useraction();
return yylval_token('='); }
== { scanner_useraction();
return yylval_token(TOK_EQ); }
!= { scanner_useraction();
return yylval_token(TOK_NE); }
\< { scanner_useraction();
return yylval_token(TOK_LT); }
\<= { scanner_useraction();
return yylval_token(TOK_LE); }
> { scanner_useraction();
return yylval_token(TOK_GT); }
>= { scanner_useraction();
return yylval_token(TOK_GE); }
\+ { scanner_useraction();
return yylval_token('+'); }
- { scanner_useraction();
return yylval_token('-'); }
\* { scanner_useraction();
return yylval_token('*'); }
\/ { scanner_useraction();
return yylval_token('/'); }
% { scanner_useraction();
return yylval_token('%'); }
! { scanner_useraction();
return yylval_token('!'); }
void { scanner_useraction();
return yylval_token(TOK_VOID); }
bool { scanner_useraction();
return yylval_token(TOK_BOOL); }
char { scanner_useraction();
return yylval_token(TOK_CHAR); }
int { scanner_useraction();
return yylval_token(TOK_INT); }
string { scanner_useraction();
return yylval_token(TOK_STRING); }
struct { scanner_useraction();
return yylval_token(TOK_STRUCT); }
if { scanner_useraction();
return yylval_token(TOK_IF); }
else { scanner_useraction();
return yylval_token(TOK_ELSE); }
while { scanner_useraction();
return yylval_token(TOK_WHILE); }
return { scanner_useraction();
return yylval_token(TOK_RETURN); }
false { scanner_useraction();
return yylval_token(TOK_FALSE); }
true { scanner_useraction();
return yylval_token(TOK_TRUE); }
null { scanner_useraction();
return yylval_token(TOK_NULL); }
ord { scanner_useraction();
return yylval_token(TOK_ORD); }
chr { scanner_useraction();
return yylval_token(TOK_CHR); }
new { scanner_useraction();
return yylval_token(TOK_NEW); }
[a-zA-Z_][a-zA-Z0-9_]* { scanner_useraction();
return yylval_token(TOK_IDENT); }
[0-9]+ { scanner_useraction();
return yylval_token(TOK_INTCON); }
('([^\\'\n]|\\[\\'"0nt])') { scanner_useraction();
return yylval_token(TOK_CHARCON); }
(\"([^\\"\n]|\\[\\'"0nt])*\") { scanner_useraction();
return yylval_token(TOK_STRINGCON); }
#[ ][0-9]+[ ](\"([^\\"\n]|\\[\\'"0nt])*\") { scanner_useraction();
scanner_include(); }
[0-9][a-zA-Z0-9_]* { scanner_useraction();
scanner_badtoken(yytext);
set_exitstatus(EXIT_FAILURE); }
('([^\\'\n]|\\[\\'"0nt])) { scanner_useraction();
scanner_badtoken(yytext);
set_exitstatus(EXIT_FAILURE); }
(\"([^\\"\n]|\\[\\'"0nt])*) { scanner_useraction();
scanner_badtoken(yytext);
set_exitstatus(EXIT_FAILURE); }
('\\') { scanner_useraction();
scanner_badtoken(yytext);
set_exitstatus(EXIT_FAILURE); }
(\"([^\\"\n]|\\)*\") { scanner_useraction();
scanner_badtoken(yytext);
set_exitstatus(EXIT_FAILURE); }
[ \t] { scanner_useraction(); }
\n { scanner_newline(); }
. { scanner_useraction();
scanner_badchar(*yytext);
set_exitstatus(EXIT_FAILURE); }

%%
