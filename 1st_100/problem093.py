# Question: https://projecteuler.net/problem=93

# To avoid placing parentheses, we can use a full binary tree structure with 4 leaves containing the numbers, and the other 3 nodes containing the arithmetic operators.
# e.g.
#     +
#    / \
#   3   5
# this means 3 + 5
# Knowing the tree structure, we can convert it to RPN for evaluating the expression.

# We use this paper's algorithm to enumerate the full binary trees with 4 leaves: Darwin Meets Graph Theory on a Strange Planet: Counting Full n-ary Trees with Labeled Leafs (http://ajmonline.org/2010/darwin.pdf).
# Since there are 4 leaves, there are (2*4-3)!! = 15 such binary trees.

import itertools

# ---------------------------------------------------------------------------------------------------------------------
# Based on this implementation: https://stackoverflow.com/a/14901512

# A very simple representation for Nodes. Leaves are anything which is not a Node.
class Node(object):
    def __init__(self, left, right, op = None):
        self.left = left
        self.right = right
        self.op = op
    def __repr__(self):
        return '({} {} {})'.format(self.left, self.op, self.right)

# Given a tree and a label, yields every possible augmentation of the tree by
# adding a new node with the label as a child "above" some existing Node or Leaf.
def add_leaf(tree, leaf):
    yield Node(leaf, tree)
    if isinstance(tree, Node):
        for left in add_leaf(tree.left, leaf):
            yield Node(left, tree.right)
        for right in add_leaf(tree.right, leaf):
            yield Node(tree.left, right)

def get_inner_nodes(tree):
    inner_nodes = []
    if isinstance(tree, Node):
        inner_nodes.extend(get_inner_nodes(tree.left))
        inner_nodes.extend(get_inner_nodes(tree.right))
        inner_nodes.extend([tree])
    return inner_nodes

def assign_op(tree, ops):
    inner_nodes = get_inner_nodes(tree)
    for op, inner_node in zip(ops, inner_nodes):
        inner_node.op = op

# Given a list of labels, yield each rooted, unordered full binary tree with
# the specified labels.
def enum_unordered(leaves, ops = None):
    if len(leaves) == 1:
        yield leaves[0]
    else:
        for tree in enum_unordered(leaves[1:]):
            for new_tree in add_leaf(tree, leaves[0]):
                if ops:
                    assign_op(new_tree, ops)
                yield new_tree

def eval_tree(tree):
    if not isinstance(tree, Node):
        return tree
    left_val = eval_tree(tree.left)
    right_val = eval_tree(tree.right)
    if not left_val or not right_val:
        return None
    if tree.op == '+':
        return left_val + right_val
    elif tree.op == '-':
        return left_val - right_val
    elif tree.op == '*':
        return left_val * right_val
    elif tree.op == '/':
        if not right_val == 0:
            return left_val / right_val
        return None
    print(tree)
    assert(False)
    return None
# ---------------------------------------------------------------------------------------------------------------------

best_seq_length = 0
permutation = None
for leaf_set in itertools.combinations([1,2,3,4,5,6,7,8,9], 4):
    result_set = set()
    for leaves in itertools.permutations(leaf_set, 4):
        for ops in itertools.product(['+', '-', '*', '/'], repeat = 3):
            for tree in enum_unordered(leaves, ops):
                result_set.add(eval_tree(tree))
    i = 1
    while True:
        if not i in result_set:
            break
        i = i + 1
    if i-1 > best_seq_length:
        best_seq_length = i-1
        permutation = list(leaf_set)
print(best_seq_length)
print(permutation)


