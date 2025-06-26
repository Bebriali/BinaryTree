#ifndef TREE_PRINTER_H
#define TREE_PRINTER_H

void TreePrint(Tree_t* tree, const char* filename);
void PrintNode(Node_t* node, FILE* tree_dump, int rank);

#endif
