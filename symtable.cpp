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
int current_block;
//current_function is used to typecheck return statements
symbol* current_function = nullptr;

string reserved_words[] = {"void", "bool", "char", "int", "string"};

void output_symnode(symbol* node){
   for(int i = 0; i < (int)symbol_stack.size() - 1; i++)
      sym_output << "   ";
   sym_output << node->ident << " (" << node->filenr <<
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
      sym_output << " struct \"" << node->struct_name << "\"";
   if(node->attributes[ATTR_array])
      sym_output << " array";
   if(node->attributes[ATTR_function])
      sym_output << " function";
   if(node->attributes[ATTR_variable])
      sym_output << " variable";
   if(node->attributes[ATTR_field])
      sym_output << " field {" << node->field_of << "}";
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

void enter_block(astree* root){
   /*
   if(block is present)
      push nullptr to symbol_stack
      save current_block to temporary variable
      set current_block to next_block
      increment next_block
      recurse on block
      pop symbol_stack
      restore current_block to temporary variable
   */
}

void gen_symtable_rec(astree* root){
   for(size_t child = 0; child < root->children.size(); ++child){
      switch(root->children[child]->symbol){
         case TOK_STRUCT:
            /* IMPLEMENT
            if(type name table has duplicate declaration)
               duplicate declaration error
            add symbol to type name table by TYPEID
            loop through children
               recurse on child
               add symbol node for child to fields
            */
            break;
         
         case TOK_PROTOTYPE:
            /* IMPLEMENT
            set attributes by return type
            set name by DECLID in return type
            recurse on parameters
            if(global symbol table has duplicate declaration)
               typecheck against duplicate
                  return type
                  parameters
            else
               populate symbol node parameters with parameters
               add symbol node to global symbol table
            */
            break;
         case TOK_FUNCTION:
            /* IMPLEMENT
            set attributes by return type
            set name by DECLID in return type
            if(global symbol table has duplicate function declaration)
               duplicate declaration error
            save current_block to temporary variable
            set current_block to next_block
            increment next_block
            recurse on parameters
            if(global symbol table has matching prototype declaration)
               typecheck against duplicate
                  return type
                  parameters
            else
               populate symbol node parameters with parameters
               add symbol node to global symbol table
            enter_block(block)
            */
            break;
         
         case TOK_BLOCK:
            /* IMPLEMENT
            enter_block({block})
            */
            break;
         case TOK_IF:
            /* IMPLEMENT
            recurse on expression
            typecheck expression
            enter_block({if block})
            */
            break;
         case TOK_IFELSE:
            /* IMPLEMENT
            recurse on expression
            typecheck expression
            enter_block({if block})
            enter_block({else block})
            */
            break;
         case TOK_WHILE:
            /* IMPLEMENT
            recurse on expression
            typecheck expression
            enter_block({while block})
            */
            break;
            
         case TOK_VARDECL:
            /* IMPLEMENT
            check top of symbol stack for duplicate declaration
            ...?
            typecheck right operand against left operand
            ...?
            */
            break;
            
         case TOK_CALL:
            /* IMPLEMENT
            recurse on arguments
            lookup function by identifier
            typecheck arguments against parameters
            set type attributes by function
            */
            break;
         case TOK_RETURN:
            /* IMPLEMENT
            if(no operand)
               if(current_function != nullptr)
                  typecheck current_function has type void
            else
               recurse on operand
               typecheck operand against current_function
            */
            break;
            
         case TOK_IDENT:
            /* IMPLEMENT
            check symbol table
            set type attributes
            */
            break;
         case '.':
            /* IMPLEMENT
            recurse on left operand (struct)
            typecheck left operand
            check {left operand}->fields for field
            set type attributes by field
            */
            break;
         case TOK_INDEX:
            /* IMPLEMENT
            recurse on indexed
            recurse on index
            typecheck indexed
            typecheck index
            set type attributes
            */
            break;
            
         case '=':
            /* IMPLEMENT
            recurse on operands
            typecheck operands
            set type attributes
            */
            break;
         
         case TOK_EQ:
         case TOK_NE:
            /* IMPLEMENT
            recurse on operands
            typecheck operands
            set type attributes
            */
            break;
         case TOK_GE:
         case TOK_GT:
         case TOK_LE:
         case TOK_LT:
            /* IMPLEMENT
            recurse on operands
            typecheck operands
            set type attributes
            */
            break;
         
         case '+':
         case '-':
         case '*':
         case '/':
         case '%':
            /* IMPLEMENT
            recurse on operands
            typecheck operands
            set type attributes
            */
            break;
         case TOK_POS:
         case TOK_NEG:
            /* IMPLEMENT
            recurse on operand
            typecheck operand
            set type attributes
            */
            break;
         
         case '!':
            /* IMPLEMENT
            recurse on operand
            typecheck operand
            set type attributes
            */
            break;
         case TOK_ORD:
            /* IMPLEMENT
            recurse on operand
            typecheck operand
            set type attributes
            */
            break;
         case TOK_CHR:
            /* IMPLEMENT
            recurse on operand
            typecheck operand
            set type attributes
            */
            break;
         
         case TOK_INTCON:
            /* IMPLEMENT
            set type attributes
            */
            break;
         case TOK_CHARCON:
            /* IMPLEMENT
            set type attributes
            */
            break;
         case TOK_STRINGCON:
            /* IMPLEMENT
            set type attributes
            */
            break;
         case TOK_FALSE:
            /* IMPLEMENT
            set type attributes
            */
            break;
         case TOK_TRUE:
            /* IMPLEMENT
            set type attributes
            */
            break;
         case TOK_NULL:
            /* IMPLEMENT
            set type attributes
            */
            break;
            
         case ';':
            //ignore
            break;
         
         default:
            cerr << "ERROR: rec_gen_symtable() did not handle " << 
               get_yytname(root->children[child]->symbol) << endl;
            gen_symtable_rec(root->children[child]);
            break;
      }
   }
}

/*void gen_symtable_rec(astree* root){
   switch(root->symbol){
      case TOK_BLOCK:
         symbol_stack.push_back(nullptr);
         next_block++;
         for(size_t child = 0; child < root->children.size(); ++child){
            gen_symtable_rec(root->children[child]);
         }
         symbol_stack.pop_back();
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
}*/

void gen_symtable(astree* root){
   for(int i = 0; i < 5; i++)
      type_name_table.insert({&reserved_words[i],nullptr});
   symbol_stack.push_back(nullptr);
   gen_symtable_rec(root);
   /*for(size_t child = 0; child < root->children.size(); ++child){
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
   }*/
}
