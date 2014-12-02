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
         //IMPLEMENT
         break;
      case TOK_IF:
         //IMPLEMENT
         break;
      case TOK_IFELSE:
         //IMPLEMENT
         break;
      case TOK_RETURN:
         //IMPLEMENT
         break;
      case TOK_VARDECL:
         //IMPLEMENT
         break;
      case TOK_WHILE:
         //IMPLEMENT
         break;
      case '=':
         //IMPLEMENT
         break;
      case TOK_EQ:
      case TOK_NE:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case TOK_GE:
      case TOK_GT:
      case TOK_LE:
      case TOK_LT:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case TOK_POS:
      case TOK_NEG:
         root->attributes[ATTR_int] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case '!':
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case TOK_ORD:
         root->attributes[ATTR_int] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case TOK_CHR:
         root->attributes[ATTR_char] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type check
         break;
      case TOK_NEW:
         //IMPLEMENT
         break;
      case TOK_CALL:
         //IMPLEMENT
         break;
      case TOK_IDENT:
         //IMPLEMENT
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
