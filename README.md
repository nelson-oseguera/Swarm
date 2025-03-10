# Swarm

This project is a C++ simulation of a swarm of autonomous robots, utilizing Binary Search Trees (BST), AVL Trees, and Splay Trees for efficient management. The system dynamically manages robot data (IDs, types, states), supporting insertion, removal, and type conversion while maintaining memory safety and tree integrity. A robust testing suite is included for validation.

Core Features

1. Swarm Management Using Trees (BST, AVL, Splay)
Robots are stored in dynamic tree structures for efficient lookup and management.
Tree types supported:
BST (Basic Binary Search Tree)
AVL (Self-balancing BST)
Splay Tree (Self-adjusting BST with splaying on access)

2. Object-Oriented Class Design (Swarm, Robot)
Robot Class:
Represents each robot in the swarm.

Attributes:

m_id: Unique robot ID.
m_type: Enum for robot type (BIRD, DRONE, REPTILE, SUB, QUADRUPED).
m_state: Enum for robot state (ALIVE, DEAD).
m_left, m_right: Left/right child in the tree.
m_height: For AVL balancing.
Methods:

Getters/Setters for ID, type, state, height, left, right.
Swarm Class:
Manages a collection of robots within a tree structure.

Attributes:

m_root: Root node of the tree.
m_type: Tree type (NONE, BST, AVL, SPLAY).
Key Methods:

insert(Robot): Adds a robot to the tree.
remove(int id): Removes a robot by ID.
clear(): Deletes all robots (memory safe).
setType(TREETYPE): Changes tree type and rebuilds if necessary.
getType(): Returns current tree type.
dumpTree(): Prints tree contents (in-order traversal).
Internal Logic:

Insert/Remove follow BST rules.
AVL: Balancing performed after each insertion/removal.
Splay: Splay operation brings recently accessed nodes to the root.
Rebuild as AVL when switching types.
Deep Copy (Clone) for safe assignments.
Proper destructors and cleanup for preventing memory leaks.

3. Tree Operations & Algorithms
AVL Balancing:

Height calculation and balance factor checking.
Rotations (Left, Right, Left-Right, Right-Left).
Splay Operation:

Zig-Zig, Zig-Zag, Zag-Zig, Zag-Zag for bringing nodes to root.
In-order Traversal for tree dumping.

4. Memory Management
Destructor and clear() ensure all dynamically allocated memory is properly freed.
Deep copy using overloaded operator= and clone() function.
Rebuild logic ensures tree integrity when switching types.

5. Testing Framework (Tester Class)
Includes comprehensive tests for:

Test Name	Purpose
testAVLBalancing	AVL remains balanced after 300 insertions
testBSTProperty	BST property maintained after 100 insertions
testSplayOperation	Splay tree properly moves nodes to root
testRemoveNormal	Standard removal operation
testRemoveEdgeCase	Remove only node in tree
testAVLAfterRemovals	AVL balanced after 150 removals from 300 nodes
testAssignmentOperator	Correct deep copy via assignment
testRandomInsertionsAndRemovals	Mixed random insertions/removals, checks BST/AVL
testSplayHeight	Splay tree maintains correct height
testAVLTreeHeight	AVL tree height within log(n) bounds

Skills & Concepts Demonstrated
Advanced Data Structures: BST, AVL, Splay Trees.
Tree balancing algorithms.
Memory management: Dynamic allocation, deep copy, destructors.
Object-Oriented Programming (OOP).
Testing and validation of tree properties.
Randomization for testing.
