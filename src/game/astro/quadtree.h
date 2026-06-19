// 2026-06-15
//
// Implementation of the Quad-Tree data structure.
// Used to easily query spatial information.

#ifndef QUADTREE_H
#define QUADTREE_H

/////////////////////////////
//- Quad-Tree Type Definitions

typedef struct qud_Node qud_Node;
struct qud_Node {
  qud_Node *parent;
  qud_Node *first;
  qud_Node *last;
  qud_Node *next;
  qud_Node *prev;
};

typedef struct qud_Tree {
  qud_Node *root;
  qud_Node *free;
  u32 node_count;
} qud_Tree;

/////////////////////////////
//- Quad-Tree Functions

static qud_Node *qud_traverseTreeDepth(qud_Node **parent);
static qud_Node *qud_allocNode(Arena *arena, qud_Tree *tree, qud_Node **parent);
static void      qud_releaseNode(qud_Tree *tree, qud_Node **parent, qud_Node **node);
static qud_Tree *qud_initTree(Arena *arena);

#endif // QUADTREE_H
