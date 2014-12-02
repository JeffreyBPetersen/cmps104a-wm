// Jeffrey Petersen | jebpeter@ucsc.edu

#include <fstream>
#include <string>

#include "astree.h"
#include "lyutils.h"
#include "symtable.h"

ofstream sym_output;
vector<symbol_table*> symbol_stack;
symbol_table type_name_table;
int next_block;

string reserved_words[] = {"void", "bool", "char", "int", "string"};

void gen_symtable_rec(astree* root){
   switch(root->symbol){
      case TOK_BLOCK:
         break;
      case TOK_IF:
         break;
      case TOK_IFELSE:
         break;
      case TOK_RETURN:
         break;
      case TOK_VARDECL:
         break;
      case TOK_WHILE:
         break;
      case '=':
         break;
      case TOK_EQ:
      case TOK_NE:
         break;
      case TOK_GE:
      case TOK_GT:
      case TOK_LE:
      case TOK_LT:
         break;
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case TOK_POS:
      case TOK_NEG:
         break;
      case '!':
         break;
      case TOK_ORD:
         break;
      case TOK_CHR:
         break;
      case TOK_NEW:
         break;
      case TOK_CALL:
         break;
      case TOK_IDENT:
         break;
      case TOK_INTCON:
         root->attributes[ATTR_const] = true;
         root->attributes[ATTR_int] = true;
         break;
      case TOK_CHARCON:
         root->attributes[ATTR_char] = true;
         root->attributes[ATTR_const] = true;
         break;
      case TOK_STRINGCON:
         root->attributes[ATTR_const] = true;
         root->attributes[ATTR_string] = true;
         break;
      case TOK_FALSE:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_const] = true;
         break;
      case TOK_TRUE:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_const] = true;
         break;
      case TOK_NULL:
         root->attributes[ATTR_const] = true;
         root->attributes[ATTR_null] = true;
         break;
   }
   /*
   for(int i = 0; i < (int)symbol_stack.size() - 1; i++)
      cout << "   ";
   sym_output << "PLACEHOLDER\n";
   */
}

void gen_symtable(astree* root){
   for(int i = 0; i < 5; i++)
      type_name_table.insert({&reserved_words[i],nullptr});
   for(size_t child = 0; child < root->children.size(); ++child){
      switch(root->children[child]->symbol){
         case TOK_FUNCTION:
            //IMPLEMENT
            break;
         case TOK_PROTOTYPE:
            //IMPLEMENT
            break;
         case TOK_STRUCT:
            //IMPLEMENT
            break;
         default:
            gen_symtable_rec(root->children[child]);
            break;
      }
   }
}
