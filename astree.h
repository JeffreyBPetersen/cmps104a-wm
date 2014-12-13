// Jeffrey Petersen | jebpeter@ucsc.edu

#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "auxlib.h"
#include "symtable.h"

struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   const string* lexinfo;    // pointer to lexical information
	//field_of is used when ATTR_field
	//struct_name is used when ATTR_struct
	string field_of, struct_name;
   vector<astree*> children; // children of this n-way node
   //asg4 additions
   attr_bitset attributes;
   size_t blocknr;
   struct symbol* symnode;
};


astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo);
astree* adopt1 (astree* root, astree* child);
astree* adopt2 (astree* root, astree* left, astree* right);
astree* adopt1sym (astree* root, astree* child, int symbol);
astree* adopt2sym (astree* root, astree* left, astree* right,
   int symbol);
astree* update_sym (astree* root, int symbol);
astree* new_function (astree* identdecl, astree* paramlist,
   astree* block);
void dump_astree (FILE* outfile, astree* root);
void output_ast (ostream& out, astree* root);
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);
void free_ast (astree* tree);
void free_ast2 (astree* tree1, astree* tree2);

#endif
