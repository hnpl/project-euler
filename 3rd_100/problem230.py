# Problem: https://projecteuler.net/problem=230

"""
    . Quite similar to problem 220.
    . Build a "Fibonacci tree" such that,
        . Let T(k) indicate the tree representing the k^th Fibonacci word.
        . T(1) and T(2) are leaf nodes.
        . The left node of T(k) is T(k-2), and the right node of T(k) is T(k-1).
        . Keep the length of T(k) in T(k).n_characters, where T(k).n_characters = T(k-2).n_characters + T(k-1).n_characters.
            This mean, T(k).n_characters is the length of all leaves of the entire (sub)tree T(k).
    . Let n be the position of the digit we want to find.
    . Find the first tree T(k) such that T(k).n_characters >= n.
    . We traverse the tree as follows,
        . Let the current node be T(k).
        . Let n_remaining be the amount of characters left to traverse before we reach the desired position.
        . While n_remaining > 0:
            . If the current node is a leaf node, the desired digit is node.word[n_remaining-1] and break the loop.
            . Otherwise, we have to choose between the left node or the right node.
              If left_node.n_characters > n_remaining, let the current node be left_node.
              Otherwise, let the current node be right_node, subtract the number of characters of the left node from n_remaining.
              Either way, go back to the beginning of the while loop.
"""

class Node:
    def __init__(self):
        self.left = None
        self.right = None
        self.word = None
        self.n_characters = None
        self.index = None # 1-indexed

    def init_from_data(self, left_node, right_node, word, n_characters, index):
        self.left = left_node
        self.right = right_node
        self.word = word
        self.n_characters = n_characters
        self.index = index # 1-indexed
        return self

    def is_leaf(self):
        return (not self.left and not self.right)

    def describe(self):
        return "Node {}: (left = {}, right = {}, n_characters = {})".format(self.index, self.left.index, self.right.index, self.n_characters)

def build_fibonacci_tree(depth, A, B):
    fibonacci_tree = [None] * (depth+1)
    fibonacci_tree[1] = Node().init_from_data(left_node = None,
                                              right_node = None,
                                              word = A,
                                              n_characters = len(A),
                                              index = 1)
    fibonacci_tree[2] = Node().init_from_data(left_node = None,
                                              right_node = None,
                                              word = B,
                                              n_characters = len(B),
                                              index = 2)
    for i in range(3, depth+1):
        left_node = fibonacci_tree[i-2]
        right_node = fibonacci_tree[i-1]
        fibonacci_tree[i] = Node().init_from_data(left_node = left_node,
                                                  right_node = right_node,
                                                  word = None,
                                                  n_characters = left_node.n_characters + right_node.n_characters,
                                                  index = i)
    return fibonacci_tree

def D(n, fibonacci_tree_roots):
    ans = "-"

    n_remaining = n
    for fibonacci_tree_root in fibonacci_tree_roots[3:]:
        if fibonacci_tree_root.n_characters > n:
            current_node = fibonacci_tree_root
            break
    
    while n_remaining > 0:
        print("->", n_remaining)
        left_node = current_node.left
        right_node = current_node.right
        if current_node.is_leaf():
            ans = current_node.word[n_remaining-1]
            break
        elif left_node.is_leaf():
            if n_remaining > left_node.n_characters:
                n_remaining -= left_node.n_characters
                current_node = right_node
            else:
                ans = left_node.word[n_remaining-1]
                break
        elif n_remaining > left_node.n_characters:
            n_remaining -= left_node.n_characters
            current_node = right_node
        else:
            current_node = left_node
    return ans

if __name__ == "__main__":
    ans = 0

    N = 17

    A = "14159265358979323846264338327950288419716939937510"+\
        "58209749445923078164062862089986280348253421170679"
    B = "82148086513282306647093844609550582231725359408128"+\
        "48111745028410270193852110555964462294895493038196"

    fibonacci_tree_roots = build_fibonacci_tree(depth = 74, A = A, B = B)

    for n in range(N+1):
        print((127 + 19*n) * (7**n))
        ans += (10**n) * int(D((127 + 19*n) * (7**n), fibonacci_tree_roots))

    print(ans)