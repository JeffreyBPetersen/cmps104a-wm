// Jeffrey Petersen | jebpeter@ucsc.edu

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

extern ofstream sym_output;

enum{ ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
      ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
      ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param,
      ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr,
      ATTR_bitset_size,
};

using attr_bitset = bitset<ATTR_bitset_size>;

struct symbol;
using symbol_table = unordered_map<const string*, symbol*>;
using symbol_entry = pair<const string*, symbol*>;

struct symbol{
   attr_bitset attributes;
   symbol_table* fields;
   //ident is the identifier for the symbol
   string ident;
   //field_of is used when ATTR_field
   //struct_name is used when ATTR_struct
   string field_of, struct_name;
   size_t filenr, linenr, offset;
   size_t blocknr;
   vector<symbol*>* parameters;
};

void gen_symtable(struct astree* root);

#endif
