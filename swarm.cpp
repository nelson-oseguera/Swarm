#include "swarm.h"
#include <iostream>
using namespace std;

// Constructor - Default tree type is NONE
Swarm::Swarm() : m_root(nullptr), m_type(NONE) {}

// Constructor - Initialize with specific tree type
Swarm::Swarm(TREETYPE type) : m_root(nullptr), m_type(type) {}

// Destructor - Clear the tree
Swarm::~Swarm() {
    clear();
}

// Clear function - Deallocates all memory in the tree
void Swarm::clear() {
    while (m_root != nullptr) {
        remove(m_root->getID());  // Remove root until tree is empty
    }
}

// Insert function - Insert a robot into the tree
void Swarm::insert(const Robot& robot) {
    Robot* newRobot = new Robot(robot);  // Allocate new robot node

    if (m_root == nullptr) {
        m_root = newRobot;  // Insert into empty tree
    } else {
        m_root = insertHelper(m_root, newRobot);  // Recursive insertion
    }

    if (m_type == AVL) {
        m_root = balanceTree(m_root);  // Balance the AVL tree after insertion
    } else if (m_type == SPLAY) {
        m_root = splay(m_root, newRobot->getID());  // Splay the new node to the root
        // Fix: Ensure the newRobot is properly deallocated if not used
        if (m_root != newRobot) {
            delete newRobot;  // Prevent memory leak if splay didn't use newRobot
        }
    }
}

// Helper function for insertion - Standard BST insertion, balance only if AVL
Robot* Swarm::insertHelper(Robot* root, Robot* newRobot) {
    if (root == nullptr) {
        return newRobot;  // Insert position found
    }

    // Insert to left subtree
    if (newRobot->getID() < root->getID()) {
        root->m_left = insertHelper(root->m_left, newRobot);
    }
    // Insert to right subtree
    else if (newRobot->getID() > root->getID()) {
        root->m_right = insertHelper(root->m_right, newRobot);
    }

    // Update height of current node
    root->m_height = 1 + max(getHeight(root->m_left), getHeight(root->m_right));

    // Balance the tree only if it's AVL
    if (m_type == AVL) {
        root = balanceTree(root);
    }

    return root;
}

// Remove function - Remove a node with given ID
void Swarm::remove(int id) {
    if (m_root == nullptr) {
        return;  // Tree is empty
    }
    m_root = removeHelper(m_root, id);  // Recursive removal

    // Balance if the tree is AVL
    if (m_type == AVL) {
        m_root = balanceTree(m_root);
    } else if (m_type == SPLAY && m_root != nullptr) {
        m_root = splay(m_root, id);  // Splay the last accessed node to the root
    }
}

// Helper function for removal - Standard BST removal
Robot* Swarm::removeHelper(Robot* root, int id) {
    if (root == nullptr) {
        return nullptr;  // Node not found
    }

    if (id < root->getID()) {
        root->m_left = removeHelper(root->getLeft(), id);
    } else if (id > root->getID()) {
        root->m_right = removeHelper(root->getRight(), id);
    } else {
        // Node to be removed found
        if (root->m_left == nullptr) {
            Robot* rightNode = root->m_right;
            delete root;
            return rightNode;
        } else if (root->m_right == nullptr) {
            Robot* leftNode = root->m_left;
            delete root;
            return leftNode;
        } else {
            // Two children case - Find the inorder successor
            Robot* minLargerNode = getMinNode(root->m_right);
            root->m_id = minLargerNode->getID();
            root->m_right = removeHelper(root->m_right, minLargerNode->getID());
        }
    }
    // Update height after deletion
    root->m_height = 1 + max(getHeight(root->m_left), getHeight(root->m_right));

    // Balance the tree if it's an AVL tree
    if (m_type == AVL) {
        root = balanceTree(root);
    }

    return root;
}

// Splay function - Moves the node with the given ID to the root
Robot* Swarm::splay(Robot* root, int id) {
    if (root == nullptr || root->getID() == id) {
        return root;  // The root is already the node we want to splay
    }

    // Left subtree case
    if (id < root->getID()) {
        if (root->m_left == nullptr) {
            return root;  // Node not in the tree, return root
        }

        // Zig-Zig (Left Left) case
        if (id < root->m_left->getID()) {
            root->m_left->m_left = splay(root->m_left->m_left, id);
            root = rightRotate(root);  // First rotation
        }
        // Zig-Zag (Left Right) case
        else if (id > root->m_left->getID()) {
            root->m_left->m_right = splay(root->m_left->m_right, id);
            if (root->m_left->m_right != nullptr) {
                root->m_left = leftRotate(root->m_left);  // Left rotation
            }
        }
        return (root->m_left == nullptr) ? root : rightRotate(root);  // Second rotation
    }

    // Right subtree case
    else {
        if (root->m_right == nullptr) {
            return root;  // Node not in the tree, return root
        }

        // Zag-Zig (Right Left) case
        if (id < root->m_right->getID()) {
            root->m_right->m_left = splay(root->m_right->m_left, id);
            if (root->m_right->m_left != nullptr) {
                root->m_right = rightRotate(root->m_right);  // Right rotation
            }
        }
        // Zag-Zag (Right Right) case
        else if (id > root->m_right->getID()) {
            root->m_right->m_right = splay(root->m_right->m_right, id);
            root = leftRotate(root);  // First left rotation
        }
        return (root->m_right == nullptr) ? root : leftRotate(root);  // Second rotation
    }
}

// Get the current tree type (BST, AVL, SPLAY, NONE)
TREETYPE Swarm::getType() const {
    return m_type;
}

// Set tree type and rebuild the tree as AVL if necessary
void Swarm::setType(TREETYPE type) {
    if (m_type == type) {
        return;  // No change in type
    }

    if (type == NONE) {
        clear();  // Clear the tree if changing to NONE
    } 
    else if (type == AVL && m_type != AVL) {
        // Rebuild the tree as AVL
        Robot* tempRoot = m_root; // Temporarily store the current root
        m_root = nullptr; // Clear the current tree
        
        // Rebuild the tree recursively
        rebuildAVL(tempRoot);  // Rebuild the tree in AVL mode
    }

    // Set the new tree type
    m_type = type;
}

// Helper to rebuild the tree as AVL recursively
void Swarm::rebuildAVL(Robot* node) {
    if (node == nullptr) {
        return;
    }

    // Recursively rebuild the left subtree
    rebuildAVL(node->m_left);

    // Insert the current node into the new AVL tree
    Robot tempRobot(node->getID(), node->getType(), node->getState());
    m_type = AVL; // Ensure we are using AVL logic for balancing here
    insert(tempRobot);

    // Recursively rebuild the right subtree
    rebuildAVL(node->m_right);
}

// Helper function to find the minimum node (leftmost node)
Robot* Swarm::getMinNode(Robot* root) const {
    while (root->m_left != nullptr) {
        root = root->m_left;
    }
    return root;
}

// Clone function for deep copy (recursive)
Robot* Swarm::clone(Robot* root) const {
    if (root == nullptr) {
        return nullptr;
    }

    Robot* newRoot = new Robot(root->getID(), root->getType(), root->getState());
    newRoot->m_left = clone(root->m_left);
    newRoot->m_right = clone(root->m_right);
    newRoot->m_height = root->m_height;
    return newRoot;
}

// Assignment operator for Swarm class
const Swarm& Swarm::operator=(const Swarm& rhs) {
    // Check for self-assignment
    if (this != &rhs) {
        // Clear the existing tree
        clear();

        // Deep copy the tree from the rhs (right-hand side)
        m_type = rhs.m_type;
        m_root = clone(rhs.m_root);  // Recursively clone the tree
    }
    return *this;  // Return the current object
}

// Balance the AVL tree if necessary
Robot* Swarm::balanceTree(Robot* node) {
    if (node == nullptr) return node;

    int balanceFactor = getBalance(node);

    // Left heavy case
    if (balanceFactor > 1) {
        if (getBalance(node->m_left) < 0) {
            node->m_left = leftRotate(node->m_left);  // Left-right case
        }
        return rightRotate(node);  // Left-left case
    }

    // Right heavy case
    if (balanceFactor < -1) {
        if (getBalance(node->m_right) > 0) {
            node->m_right = rightRotate(node->m_right);  // Right-left case
        }
        return leftRotate(node);  // Right-right case
    }

    return node;
}

// Left rotation helper function
Robot* Swarm::leftRotate(Robot* x) {
    Robot* y = x->m_right;
    Robot* T2 = y->m_left;

    y->m_left = x;
    x->m_right = T2;

    // Update heights
    x->m_height = 1 + max(getHeight(x->m_left), getHeight(x->m_right));
    y->m_height = 1 + max(getHeight(y->m_left), getHeight(y->m_right));

    return y;
}

// Right rotation helper function
Robot* Swarm::rightRotate(Robot* y) {
    Robot* x = y->m_left;
    Robot* T2 = x->m_right;

    x->m_right = y;
    y->m_left = T2;

    // Update heights
    y->m_height = 1 + max(getHeight(y->m_left), getHeight(y->m_right));
    x->m_height = 1 + max(getHeight(x->m_left), getHeight(x->m_right));

    return x;
}

// Get height of a node
int Swarm::getHeight(Robot* node) const {
    return (node == nullptr) ? -1 : node->getHeight();
}

// Get balance factor of a node
int Swarm::getBalance(Robot* node) const {
    return (node == nullptr) ? 0 : getHeight(node->m_left) - getHeight(node->m_right);
}

// Dump function for in-order traversal
void Swarm::dumpTree() const {
    dump(m_root);
    cout << endl;
}

void Swarm::dump(Robot* aBot) const {
    if (aBot != nullptr) {
        cout << "(";
        dump(aBot->m_left);  // Visit left child
        cout << aBot->m_id << ":" << aBot->m_height;  // Visit node itself
        dump(aBot->m_right);  // Visit right child
        cout << ")";
    }
}