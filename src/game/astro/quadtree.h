// 2026-06-15
//
// Implementation of the Quad-Tree data structure.
// Used to easily query spatial information.

#ifndef QUADTREE_H
#define QUADTREE_H

/////////////////////////////
//- Quad-Tree Type Definitions

typedef struct Qd_Node Qd_Node;
struct Qd_Node {
  Qd_Node *parent;
  Qd_Node *first;
  Qd_Node *last;
  Qd_Node *next;
  Qd_Node *prev;
};

typedef struct Qd_Tree {
  Qd_Node *root;
  Qd_Node *free;
  u32 node_count;
  Vector2 bounds;
} Qd_Tree;

/////////////////////////////
//- Quad-Tree Functions

static Qd_Node *Qd_TraverseTreeDepth(Qd_Node **parent);
static Qd_Node *Qd_AllocNode(Arena *arena, Qd_Tree *tree, Qd_Node **parent);
static void     Qd_ReleaseNode(Qd_Tree *tree, Qd_Node **parent, Qd_Node **node);
static Qd_Tree  Qd_MakeTree(Arena *arena, Vector2 bounds);

#endif // QUADTREE_H
