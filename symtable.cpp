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

void output_symnode(symbol* node){
   for(int i = 0; i < (int)symbol_stack.size() - 1; i++)
      sym_output << "   ";
   sym_output << "PLACEHOLDER_identifier" << " (" << node->filenr <<
      "." << node->linenr << "." << node->offset << ") {" <<
      node->blocknr << "}";
   if(node->attributes[ATTR_void])
      sym_output << " void";
   if(node->attributes[ATTR_bool])
      sym_output << " bool";
   if(node->attributes[ATTR_char])
      sym_output << " char";
   if(node->attributes[ATTR_int])
      sym_output << " int";
   if(node->attributes[ATTR_null])
      sym_output << " null";
   if(node->attributes[ATTR_string])
      sym_output << " string";
   if(node->attributes[ATTR_struct])
      sym_output << " struct \"" << "PLACEHOLDER_struct_name" << "\"";
   if(node->attributes[ATTR_array])
      sym_output << " array";
   if(node->attributes[ATTR_function])
      sym_output << " function";
   if(node->attributes[ATTR_variable])
      sym_output << " variable";
   if(node->attributes[ATTR_field])
      sym_output << " field {" << "PLACEHOLDER_struct_name" << "}";
   if(node->attributes[ATTR_typeid])
      sym_output << " typeid";
   if(node->attributes[ATTR_param])
      sym_output << " param";
   if(node->attributes[ATTR_lval])
      sym_output << " lval";
   if(node->attributes[ATTR_const])
      sym_output << " const";
   if(node->attributes[ATTR_vreg])
      sym_output << " vreg";
   if(node->attributes[ATTR_vaddr])
      sym_output << " vaddr";
   sym_output << endl;
}

void gen_symtable_rec(astree* root){
   switch(root->symbol){
      case TOK_BLOCK:
         //IMPLEMENT push_back to symbol_stack
         //IMPLEMENT recurse on children
         break;
      case TOK_IF:
         //IMPLEMENT type check condition
         //IMPLEMENT recurse on block
         break;
      case TOK_IFELSE:
         //IMPLEMENT type check condition
         //IMPLEMENT recurse on block
         //IMPLEMENT recurse on else
         break;
      case TOK_RETURN:
         //IMPLEMENT type check
         // return nothing when at global scope
         // return return_type when in function
         //  IMPLEMENT checking return type of function
         break;
      case TOK_VARDECL:
         struct symbol* variable;
         variable = new struct symbol;
         // last child (DECLID) of first child {type}
         //    is identifier with coordinates
         variable->filenr = root->children[0]->children.back()->filenr;
         variable->linenr = root->children[0]->children.back()->linenr;
         variable->offset = root->children[0]->children.back()->offset;
         //IMPLEMENT set variable attributes
         //IMPLEMENT type checking
         //switch on variable type
         switch(root->children[0]->symbol){
            case TOK_ARRAY:
               //IMPLEMENT
               break;
            case TOK_BOOL:
               //IMPLEMENT
               break;
            case TOK_CHAR:
               //IMPLEMENT
               break;
            case TOK_INT:
               //IMPLEMENT
               break;
            case TOK_STRING:
               //IMPLEMENT
               break;
            case TOK_TYPEID:
               //IMPLEMENT
               break;
         }
         if(symbol_stack.back() == nullptr)
            symbol_stack.back() = new symbol_table;
         //IMPLEMENT check for conflicting definition
         if(false){
            //IMPLEMENT error message
         }
         else 
            symbol_stack.back()->insert(
            {root->children[0]->lexinfo,variable});
         output_symnode(variable);
         break;
      case TOK_WHILE:
         //IMPLEMENT type check condition
         //IMPLEMENT recurse on block
         break;
      case '=':
         //IMPLEMENT type checking
         break;
      case TOK_EQ:
      case TOK_NE:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type checking
         break;
      case TOK_GE:
      case TOK_GT:
      case TOK_LE:
      case TOK_LT:
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type checking
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
         //IMPLEMENT type checking
         break;
      case '!':
         root->attributes[ATTR_bool] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type checking
         break;
      case TOK_ORD:
         root->attributes[ATTR_int] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type checking
         break;
      case TOK_CHR:
         root->attributes[ATTR_char] = true;
         root->attributes[ATTR_vreg] = true;
         //IMPLEMENT type checking
         break;
      case TOK_NEW:
         //IMPLEMENT switch for varieties of new
         //IMPLEMENT type checking
         break;
      case TOK_CALL:
         //IMPLEMENT lookup function
         //IMPLEMENT type checking on parameters
         //IMPLEMENT set attributes by return type
         break;
      case TOK_IDENT:
         //IMPLEMENT lookup identifier
         //IMPLEMENT lookup identifier with field
         //IMPLEMENT set attributes by type
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
}

void gen_symtable(astree* root){
   for(int i = 0; i < 5; i++)
      type_name_table.insert({&reserved_words[i],nullptr});
   symbol_stack.push_back(nullptr);
   for(size_t child = 0; child < root->children.size(); ++child){
      switch(root->children[child]->symbol){
         case TOK_FUNCTION:
            //IMPLEMENT check for conflicting definition
            //IMPLEMENT add to global identifiers
            //IMPLEMENT enter block, push_back parameters, and recurse
            break;
         case TOK_PROTOTYPE:
            //IMPLEMENT check for conflicting definition
            //IMPLEMENT add to global identifiers if absent
            break;
         case TOK_STRUCT:
            //IMPLEMENT check for conflicting definitions
            //IMPLEMENT add to type_name_table
            //IMPLEMENT add fields if full definition
            break;
         default:
            gen_symtable_rec(root->children[child]);
            break;
      }
   }
}
