// Jeffrey Petersen | jebpeter@ucsc.edu

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "stringset.h"
#include "lyutils.h"
#include "symtable.h"

astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo) {
   astree* tree = new astree();
   tree->symbol = symbol;
   tree->filenr = filenr;
   tree->linenr = linenr;
   tree->offset = offset;
   tree->lexinfo = intern_stringset (lexinfo);
   //asg4 additions
   tree->attributes = 0;
   tree->blocknr = 0;
   tree->symnode = nullptr;
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           tree, tree->filenr, tree->linenr, tree->offset,
           get_yytname (tree->symbol), tree->lexinfo->c_str());
   return tree;
}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child, child->lexinfo->c_str());
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt1sym (astree* root, astree* child, int symbol) {
   root = adopt1 (root, child);
   root->symbol = symbol;
   return root;
}

astree* adopt2sym (astree* root, astree* left, astree* right,
   int symbol) {
   adopt1 (root, left);
   adopt1 (root, right);
   root->symbol = symbol;
   return root;
}

astree* update_sym (astree* root, int symbol) {
   root->symbol = symbol;
   return root;
}

astree* new_function (astree* identdecl, astree* paramlist,
   astree* block){
   astree* func = new_astree(TOK_FUNCTION, identdecl->filenr,
      identdecl->linenr, identdecl->offset, "");
   adopt1(func, identdecl);
   adopt1(func, paramlist);
   if(block->lexinfo->compare(";") == 0)
      func->symbol = TOK_PROTOTYPE;
   else
      adopt1(func, block);
   return func;
}

static void dump_node (FILE* outfile, astree* node) {
   fprintf (outfile, "%p->{%s(%d) %ld:%ld.%03ld \"%s\" [",
            node, get_yytname (node->symbol), node->symbol,
            node->filenr, node->linenr, node->offset,
            node->lexinfo->c_str());
   bool need_space = false;
   for (size_t child = 0; child < node->children.size();
        ++child) {
      if (need_space) fprintf (outfile, " ");
      need_space = true;
      fprintf (outfile, "%p", node->children.at(child));
   }
   fprintf (outfile, "]}");
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   fprintf (outfile, "%*s%s ", depth * 3, "",
            root->lexinfo->c_str());
   dump_node (outfile, root);
   fprintf (outfile, "\n");
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_astree_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}

static void output_ast_node (ostream& out, astree* node){
   string name = get_yytname(node->symbol);
   if(name.substr(0,4).compare("TOK_") == 0)
      name = name.substr(4);
   out << name << " \"" << *node->lexinfo << "\" (" << node->filenr << 
   "." << node->linenr << "." << node->offset << ") {" <<
   node->blocknr << "}";
   if(node->attributes[ATTR_void])
      out << " void";
   if(node->attributes[ATTR_bool])
      out << " bool";
   if(node->attributes[ATTR_char])
      out << " char";
   if(node->attributes[ATTR_int])
      out << " int";
   if(node->attributes[ATTR_null])
      out << " null";
   if(node->attributes[ATTR_string])
      out << " string";
   if(node->attributes[ATTR_struct])
      out << " struct \"" << "PLACEHOLDER_for_struct_name" << "\"";
   if(node->attributes[ATTR_array])
      out << " array";
   if(node->attributes[ATTR_function])
      out << " function";
   if(node->attributes[ATTR_variable])
      out << " variable";
   if(node->attributes[ATTR_field])
      out << " field";
   if(node->attributes[ATTR_typeid])
      out << " typeid";
   if(node->attributes[ATTR_param])
      out << " param";
   if(node->attributes[ATTR_lval])
      out << " lval";
   if(node->attributes[ATTR_const])
      out << " const";
   if(node->attributes[ATTR_vreg])
      out << " vreg";
   if(node->attributes[ATTR_vaddr])
      out << " vaddr";
   if(node->symbol == TOK_IDENT)
      out << " (" << "PLACEHOLDER_for_declaration_coordinates" << ")";
}

static void output_ast_rec (ostream& out, astree* root, int depth){
   if(root == NULL)
      return;
   for(int i = 0; i < depth; i++)
      out << "|  ";
   output_ast_node(out, root);
   out << "\n";
   for(size_t child = 0; child < root->children.size(); ++child){
      output_ast_rec(out, root->children[child], depth + 1);
   }
}

void output_ast (ostream& out, astree* root){
   output_ast_rec(out, root, 0);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}

void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}
