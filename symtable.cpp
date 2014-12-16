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
	if(node->attributes[ATTR_field])
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

void undeclared_error(astree* node, string message){
   string name = get_yytname(node->symbol);
   if(name.substr(0,4).compare("TOK_") == 0)
      name = name.substr(4);
   cerr << "ERROR: Undeclared identifier @ " << name << " \"" <<
      *node->lexinfo << "\" (" << node->filenr << "." <<
      node->linenr << "." << node->offset << ") {" <<
      node->blocknr << "} Cause: " << message << endl;
   set_exitstatus(EXIT_FAILURE);
}

void typecheck_error(astree* node, string message){
   string name = get_yytname(node->symbol);
   if(name.substr(0,4).compare("TOK_") == 0)
      name = name.substr(4);
   cerr << "ERROR: Invalid Type @ " << name << " \"" <<
      *node->lexinfo << "\" (" << node->filenr << "." <<
      node->linenr << "." << node->offset << ") {" <<
      node->blocknr << "} Cause: " << message << endl;
   set_exitstatus(EXIT_FAILURE);
}

void redeclaration_error(astree* node, string message){
   string name = get_yytname(node->symbol);
   if(name.substr(0,4).compare("TOK_") == 0)
      name = name.substr(4);
   cerr << "ERROR: Duplicate declaration @ " << name << " \"" <<
      *node->lexinfo << "\" (" << node->filenr << "." <<
      node->linenr << "." << node->offset << ") {" <<
      node->blocknr << "} Cause: " << message << endl;
   set_exitstatus(EXIT_FAILURE);
}

void add_symnode(astree* node, symbol* symnode, symbol_table* table){
   symnode->ident = *node->lexinfo;
   symnode->attributes = node->attributes;
   symnode->filenr = node->filenr;
   symnode->linenr = node->linenr;
   symnode->offset = node->offset;
   symnode->blocknr = node->blocknr;
	symnode->struct_name = node->struct_name;
   symnode->field_of = node->field_of;
   table->insert({node->lexinfo, symnode});
   output_symnode(symnode);
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
      set current_block to temporary variable
   */
}

void declare_identifier(astree* type, astree* ident, symbol* structure){
   symbol* identifier = new symbol;
	bool is_struct = false;
   switch(type->symbol){
      case TOK_BOOL:
         ident->attributes[ATTR_bool] = true;
         break;
      case TOK_CHAR:
         ident->attributes[ATTR_char] = true;
         break;
      case TOK_INT:
         ident->attributes[ATTR_int] = true;
         break;
      case TOK_STRING:
         ident->attributes[ATTR_string] = true;
         break;
      case TOK_TYPEID:
         ident->attributes[ATTR_struct] = true;
			ident->struct_name = structure->struct_name;
         is_struct = true;
         break;
   }
   if(ident->symbol == TOK_DECLID){
		// ADD error if is_struct and not defined or fields are null
      if(symbol_stack.back() != nullptr &&
         symbol_stack.back()->find(ident->lexinfo) != 
         symbol_stack.back()->end())
         redeclaration_error(ident,
            "Identifier already used in local scope");
      ident->attributes[ATTR_variable] = true;
      ident->attributes[ATTR_lval] = true;
      if(symbol_stack.back() == nullptr)
         symbol_stack.back() = new symbol_table;
      add_symnode(ident, identifier, symbol_stack.back());
   }
   else if(ident->symbol == TOK_FIELD){
		if(structure->fields->find(ident->lexinfo) != structure->fields->end())
			redeclaration_error(ident, "Identifier already used in struct");
		ident->attributes[ATTR_field] = true;
		ident->field_of = structure->struct_name;
		add_symnode(ident, identifier, structure->fields);
   }
   else
      cerr << "ERROR: Invalid variable declaration token\n";
}

bool is_compatible_primitive(astree* left, astree* right){
   return
      (left->attributes[ATTR_bool] && right->attributes[ATTR_bool]) ||
      (left->attributes[ATTR_char] && right->attributes[ATTR_char]) ||
      (left->attributes[ATTR_int] && right->attributes[ATTR_int]);
}

bool is_compatible_reference(astree* left, astree* right){
   return
      (left->attributes[ATTR_null] &&
      (right->attributes[ATTR_null] || right->attributes[ATTR_string] ||
      right->attributes[ATTR_struct])) ||
      (right->attributes[ATTR_null] &&
      (left->attributes[ATTR_null] || left->attributes[ATTR_string] ||
      left->attributes[ATTR_struct])) ||
      (left->attributes[ATTR_string] &&
      right->attributes[ATTR_string]) ||
      (left->attributes[ATTR_struct] &&
      right->attributes[ATTR_struct] &&
      left->struct_name.compare(right->struct_name) == 0);
}

bool is_compatible(astree* left, astree* right){
   return
      left->attributes[ATTR_array] == right->attributes[ATTR_array] &&
      (is_compatible_primitive(left, right) ||
      is_compatible_reference(left, right));
}

void gen_symtable_rec(astree* root){
   astree* child;
   for(size_t child_num = 0;
      child_num < root->children.size();
      ++child_num){
      child = root->children[child_num];
		symbol* structure;
      switch(child->symbol){
         case TOK_STRUCT:
				structure = new symbol;
				if(type_name_table.find(child->lexinfo) !=
					type_name_table.end()){
					structure = type_name_table.find(child->lexinfo)->second;
					if(structure->fields != nullptr)
						redeclaration_error(child, "Structure already defined");
					structure->filenr = child->children[0]->filenr;
					structure->linenr = child->children[0]->linenr;
					structure->offset = child->children[0]->offset;
					output_symnode(structure);
				}
				else{
					child->children[0]->attributes[ATTR_struct] = true;
					child->children[0]->struct_name = *(child->children[0]->lexinfo);
					structure->fields = new symbol_table;
					add_symnode(child->children[0], structure, &type_name_table);
					child->symnode = structure;
				}
				gen_symtable_rec(child);
				sym_output << endl;
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
				sym_output << endl;
            enter_block(block)
				sym_output << endl;
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
            gen_symtable_rec(child);
            if(!is_compatible(child->children[1],
               child->children[0]->children.back()))
                  typecheck_error(child, "operands not matching types");
            break;
         
         case TOK_ARRAY:
            child->children[1]->attributes[ATTR_array] = true;
            declare_identifier(child->children[0],
               child->children[1],
					root->symnode);
            break;
         case TOK_BOOL:
         case TOK_CHAR:
         case TOK_INT:
         case TOK_STRING:
         case TOK_TYPEID:
				if(root->symbol != TOK_STRUCT || child_num != 0)
					declare_identifier(child, child->children[0], root->symnode);
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
            symbol* symnode;
            int i;
            for(i = symbol_stack.size() - 1; i >= 0; i--){
               if(symbol_stack[i]->find(child->lexinfo) !=
                  symbol_stack[i]->end()){
                  symnode = 
                     symbol_stack[i]->find(child->lexinfo)->second;
                  break;
               }
            }
            if(i < 0)
               undeclared_error(child,
                  "variable not found in symbol_stack");
            else{
               child->attributes = symnode->attributes;
               child->symnode = symnode;
            }
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
				gen_symtable_rec(child);
				if(!child->children[0]->attributes[ATTR_lval] ||
					!is_compatible(child->children[0], child->children[1]))
					typecheck_error(child, "operands not matching types");
				if(child->children[0]->attributes[ATTR_bool])
					child->attributes[ATTR_bool] = true;
				else if(child->children[0]->attributes[ATTR_char])
					child->attributes[ATTR_char] = true;
				else if(child->children[0]->attributes[ATTR_int])
					child->attributes[ATTR_int] = true;
				else if(child->children[0]->attributes[ATTR_string])
					child->attributes[ATTR_string] = true;
				else if(child->children[0]->attributes[ATTR_struct]){
					child->attributes[ATTR_struct] = true;
					child->struct_name = child->children[0]->struct_name;
				}
				child->attributes[ATTR_vreg] = true;
            break;
         
         case TOK_EQ:
         case TOK_NE:
            gen_symtable_rec(child);
            if(!is_compatible(child->children[0], child->children[1]))
               typecheck_error(child, "operands not matching types");
            child->attributes[ATTR_bool] = true;
            child->attributes[ATTR_vreg] = true;
            break;
            break;
         case TOK_GE:
         case TOK_GT:
         case TOK_LE:
         case TOK_LT:
            gen_symtable_rec(child);
            if(!is_compatible_primitive(child->children[0],
               child->children[1]))
               typecheck_error(child,
                  "operands not matching primitives");
            child->attributes[ATTR_bool] = true;
            child->attributes[ATTR_vreg] = true;
            break;
         
         case '+':
         case '-':
         case '*':
         case '/':
         case '%':
            gen_symtable_rec(child);
            if(!(child->children[0]->attributes[ATTR_int]
               && child->children[1]->attributes[ATTR_int]))
               typecheck_error(child, "non-int operand");
            child->attributes[ATTR_int] = true;
            child->attributes[ATTR_vreg] = true;
            break;
            break;

         case TOK_POS:
         case TOK_NEG:
            gen_symtable_rec(child);
            if(!child->children[0]->attributes[ATTR_int])
               typecheck_error(child, "non-int operand");
            child->attributes[ATTR_int] = true;
            child->attributes[ATTR_vreg] = true;
            break;
         case '!':
            gen_symtable_rec(child);
            if(!child->children[0]->attributes[ATTR_bool])
               typecheck_error(child, "non-bool operand");
            child->attributes[ATTR_bool] = true;
            child->attributes[ATTR_vreg] = true;
            break;
         case TOK_ORD:
            gen_symtable_rec(child);
            if(!child->children[0]->attributes[ATTR_char])
               typecheck_error(child, "non-char operand");
            child->attributes[ATTR_int] = true;
            child->attributes[ATTR_vreg] = true;
            break;
         case TOK_CHR:
            gen_symtable_rec(child);
            if(!child->children[0]->attributes[ATTR_int])
               typecheck_error(child, "non-int operand");
            child->attributes[ATTR_char] = true;
            child->attributes[ATTR_vreg] = true;
            break;
         
         case TOK_INTCON:
            child->attributes[ATTR_int] = true;
            child->attributes[ATTR_const] = true;
            break;
         case TOK_CHARCON:
            child->attributes[ATTR_char] = true;
            child->attributes[ATTR_const] = true;
            break;
         case TOK_STRINGCON:
            child->attributes[ATTR_string] = true;
            child->attributes[ATTR_const] = true;
            break;
         case TOK_FALSE:
         case TOK_TRUE:
            child->attributes[ATTR_bool] = true;
            child->attributes[ATTR_const] = true;
            break;
         case TOK_NULL:
            child->attributes[ATTR_null] = true;
            child->attributes[ATTR_const] = true;
            break;
            
         case ';':
            //ignore
            break;
         
         default:
            cerr << "ERROR: rec_gen_symtable() did not handle " << 
               get_yytname(child->symbol) << endl;
            gen_symtable_rec(child);
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
