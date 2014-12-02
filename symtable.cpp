// Jeffrey Petersen | jebpeter@ucsc.edu

#include <fstream>
#include <string>

#include "astree.h"
#include "symtable.h"

ofstream sym_output;
vector<symbol_table*> symbol_stack;
symbol_table type_name_table;
int next_block;

string reserved_words[] = {"void", "bool", "char", "int", "string"};

void gen_symtable_rec(astree* node){
	sym_output << "PLACEHOLDER\n";
}

void gen_symtable(astree* root){
	for(int i = 0; i < 5; i++)
		type_name_table.insert({&reserved_words[i],nullptr});
	for(size_t child = 0; child < root->children.size(); ++child){
      gen_symtable_rec(root->children[child]);
   }
}
