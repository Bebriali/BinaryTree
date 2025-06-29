#ifndef TREE_READER_H
#define TREE_READER_H

Tree_t* TreeRead(const char* filename, FILE* log);
Node_t* ReadNode(FILE* dump);

#endif
