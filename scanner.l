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
[ \t\n] { cout << "whitespace{" << yytext << "}\n"; }
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

%%
main() {
   yylex();
}
