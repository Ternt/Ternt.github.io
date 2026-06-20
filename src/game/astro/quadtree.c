// 2026-06-15

static Qd_Node *Qd_TraverseTreeDepth(Qd_Node **parent)
{
}

static Qd_Node *Qd_AllocNode(Arena *arena, Qd_Tree *tree, Qd_Node **parent)
{
  Qd_Node *node = tree->free;
  if(node == null)
  {
    node = PushArray(arena, Qd_Node, 1);
  }
  else
  {
    StackPop(tree->free);
  }
  MemoryZeroStruct(node);
  DLLPushBack((*parent)->first, (*parent)->last, node);
  tree->node_count += 1;
  return node;
}

static void Qd_ReleaseNode(Qd_Tree *tree, Qd_Node **parent, Qd_Node **node)
{
  DLLRemove((*parent)->first, (*parent)->last, *node);
  tree->node_count -= 1;
}

static Qd_Tree Qd_MakeTree(Arena *arena, Vector2 bounds)
{
  Qd_Tree tree = zero_struct;
  tree.root = PushArray(arena, Qd_Node, 1);
  tree.node_count += 1;
  tree.bounds = bounds;
  return tree;
}

