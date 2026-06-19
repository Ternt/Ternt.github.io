// 2026-06-15

static qud_Node *qud_traverseTreeDepth(qud_Node **parent)
{
}

static qud_Node *qud_allocNode(Arena *arena, qud_Tree *tree, qud_Node **parent)
{
  qud_Node *node = tree->free;
  if(node == null)
  {
    node = push_array(arena, qud_Node, 1);
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

static void qud_releaseNode(qud_Tree *tree, qud_Node **parent, qud_Node **node)
{
  DLLRemove((*parent)->first, (*parent)->last, *node);
  tree->node_count -= 1;
}


static qud_Tree *qud_initTree(Arena *arena)
{
  qud_Tree *tree = push_array(arena, qud_Tree, 1);
  tree->root = push_array(arena, qud_Node, 1);
  tree->node_count += 1;
  return tree;
}

