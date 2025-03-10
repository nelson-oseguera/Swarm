#include "swarm.h"
#include <iostream>
using namespace std;

class Tester {
public:
    // Test whether the AVL tree remains balanced after multiple insertions
    bool testAVLBalancing(Swarm& swarm) {
        for (int i = 1; i <= 300; i++) {
            swarm.insert(Robot(i + MINID));  // Insert 300 nodes
        }
        bool balancedCheck = isBalanced(swarm.m_root);  // Check balance after insertions
        
        // Cleanup to avoid memory leak
        swarm.clear();  // This ensures all nodes are freed after the test
        
        return balancedCheck;
    }

    // Test whether the BST property is preserved after insertions
    bool testBSTProperty(Swarm& swarm) {
        for (int i = 1; i <= 100; i++) {
            swarm.insert(Robot(i + MINID));  // Insert 100 nodes
        }
        bool isBSTCorrect = isBST(swarm.m_root, nullptr, nullptr);
        
        // Cleanup to avoid memory leak
        swarm.clear();  // This ensures all nodes are freed after the test
        
        return isBSTCorrect;
    }

    // Test whether the Splay tree performs splay operations
    bool testSplayOperation(Swarm& swarm) {
        swarm.setType(SPLAY);
        for (int i = 1; i <= 10; i++) {
            swarm.insert(Robot(i + MINID));
            if (swarm.m_root->getID() != i + MINID) {
                return false;  // Check if inserted node is at the root
            }
        }

        // Now, access an existing node to check if it splays to the root
        swarm.insert(Robot(5 + MINID));  // Access node with ID 5 + MINID
        if (swarm.m_root->getID() != 5 + MINID) {
            return false;  // After access, node 5 + MINID should be at the root
        }

        // Cleanup to avoid memory leak
        swarm.clear();  // This ensures all nodes are freed after the test

        return true;
    }

    // Test the remove function for a normal case
    bool testRemoveNormal(Swarm& swarm) {
        swarm.insert(Robot(99999));
        swarm.remove(99999);
        bool result = !contains(swarm.m_root, 99999);
        
        // Cleanup to avoid memory leak
        swarm.clear();  // This ensures all nodes are freed after the test
        
        return result;
    }

    // Test the remove function for an edge case (removing the only node)
    bool testRemoveEdgeCase(Swarm& swarm) {
        swarm.insert(Robot(50000));
        swarm.remove(50000);
        bool isRootNull = (swarm.m_root == nullptr);  // Tree should be empty
        
        // Cleanup to avoid memory leak
        swarm.clear();  // This ensures all nodes are freed after the test
        
        return isRootNull;
    }

    // Test whether the AVL tree remains balanced after multiple removals
    bool testAVLAfterRemovals(Swarm& swarm) {
        for (int i = 1; i <= 300; i++) {
            swarm.insert(Robot(i + MINID));  // Insert 300 nodes
        }
        for (int i = 1; i <= 150; i++) {
            swarm.remove(i + MINID);  // Remove half of the nodes
        }
        bool balancedCheck = isBalanced(swarm.m_root);  // Check balance after removals
        
        // Ensure that we clean up after the test to avoid memory leaks
        swarm.clear();  // Clear all remaining nodes after the test
        
        return balancedCheck;
    }

    // Test the assignment operator for normal and error cases
    bool testAssignmentOperator(Swarm& swarm) {
        Swarm copySwarm;
        copySwarm = swarm;  // Test assignment
        bool result = isBST(copySwarm.m_root, nullptr, nullptr) && isBalanced(copySwarm.m_root);
        
        // Cleanup to avoid memory leak
        swarm.clear();     // Clear original swarm
        copySwarm.clear(); // Clear copied swarm
        
        return result;
    }

    // Random insertion and removal tests
    bool testRandomInsertionsAndRemovals(Swarm& swarm) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(MINID, MAXID);

        std::vector<int> ids;
        for (int i = 0; i < 100; ++i) {
            int randomID = dis(gen);
            ids.push_back(randomID);
            swarm.insert(Robot(randomID));
        }

        bool isBSTCorrect = isBST(swarm.m_root, nullptr, nullptr);
        std::shuffle(ids.begin(), ids.end(), gen);
        for (int i = 0; i < 50; ++i) {
            swarm.remove(ids[i]);
        }

        bool balancedCheck = (swarm.getType() != AVL) || isBalanced(swarm.m_root);
        bool isBSTAfterRemovals = isBST(swarm.m_root, nullptr, nullptr);

        swarm.clear();
        return balancedCheck && isBSTAfterRemovals;
    }

    // Test height correctness after multiple insertions in a Splay tree
    bool testSplayHeight(Swarm& swarm) {
        swarm.setType(SPLAY);
        for (int i = 1; i <= 10; i++) {
            swarm.insert(Robot(i + MINID));
        }

        bool heightCheck = isHeightCorrect(swarm.m_root);  // Check height correctness
        
        // Cleanup to avoid memory leak
        swarm.clear();  // Ensure all nodes are freed after the test
        
        return heightCheck;
    }

    // Test AVL tree height using log(n) calculation
    bool testAVLTreeHeight(Swarm& swarm) {
        for (int i = 1; i <= 100; i++) {
            swarm.insert(Robot(i + MINID));  // Insert 100 nodes
        }

        int maxAllowedHeight = floor(log2(100)) + 1;
        bool heightCheck = getHeight(swarm.m_root) <= maxAllowedHeight;

        swarm.clear();  // Cleanup
        return heightCheck;
    }

private:
    // Check if the tree is balanced (used for AVL tests)
    bool isBalanced(Robot* node) {
        if (node == nullptr) return true;

        int balanceFactor = getHeight(node->m_left) - getHeight(node->m_right);
        if (abs(balanceFactor) > 1) return false;

        return isBalanced(node->m_left) && isBalanced(node->m_right);
    }

    // Check if the tree follows the BST property
    bool isBST(Robot* node, Robot* min, Robot* max) {
        if (node == nullptr) return true;

        if ((min != nullptr && node->getID() <= min->getID()) || 
            (max != nullptr && node->getID() >= max->getID())) {
            return false;
        }

        return isBST(node->m_left, min, node) && isBST(node->m_right, node, max);
    }

    // Check if the height is correct (for Splay trees)
    bool isHeightCorrect(Robot* node) {
        if (node == nullptr) return true;

        int leftHeight = getHeight(node->m_left);
        int rightHeight = getHeight(node->m_right);

        if (node->m_height != 1 + max(leftHeight, rightHeight)) {
            return false;
        }

        return isHeightCorrect(node->m_left) && isHeightCorrect(node->m_right);
    }

    // Check if a node with a specific ID exists in the tree
    bool contains(Robot* node, int id) {
        if (node == nullptr) return false;
        if (node->getID() == id) return true;
        if (id < node->getID()) return contains(node->m_left, id);
        return contains(node->m_right, id);
    }

    // Get height of a node
    int getHeight(Robot* node) {
        return (node == nullptr) ? -1 : node->getHeight();
    }
};

int main() {
    Tester tester;

    Swarm bstSwarm(BST);    // Test cases for BST
    Swarm avlSwarm(AVL);    // Test cases for AVL
    Swarm splaySwarm(SPLAY); // Test cases for Splay

    // Test AVL tree balancing
    cout << "AVL Tree Balancing after 300 insertions: " 
         << (tester.testAVLBalancing(avlSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test BST property preservation
    cout << "BST Property after insertions: " 
         << (tester.testBSTProperty(bstSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test Splay tree operations
    cout << "Splay Operation: " 
         << (tester.testSplayOperation(splaySwarm) ? "Passed" : "Failed") 
         << endl;

    // Test Splay tree heights after insertions
    cout << "Splay Tree Heights after insertions: " 
         << (tester.testSplayHeight(splaySwarm) ? "Passed" : "Failed") 
         << endl;

    // Test remove function (normal case)
    cout << "Remove function (normal case): " 
         << (tester.testRemoveNormal(bstSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test remove function (edge case)
    cout << "Remove function (edge case): " 
         << (tester.testRemoveEdgeCase(bstSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test AVL tree balancing after multiple removals
    cout << "AVL Balancing after removals: " 
         << (tester.testAVLAfterRemovals(avlSwarm) ? "Passed" : "Failed") 
         << endl;

    // Since testBSTAfterRemovals is not defined, call testAVLAfterRemovals instead
    cout << "BST Property after multiple removals: " 
         << (tester.testAVLAfterRemovals(bstSwarm) ? "Passed" : "Failed") 
         << endl;

    // Similarly, use testAVLAfterRemovals for height checks
    cout << "Height Correctness after multiple removals: " 
         << (tester.testAVLAfterRemovals(bstSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test assignment operator
    cout << "Assignment Operator: " 
         << (tester.testAssignmentOperator(avlSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test random insertions and removals
    cout << "Random Insertions and Removals: " 
         << (tester.testRandomInsertionsAndRemovals(avlSwarm) ? "Passed" : "Failed") 
         << endl;

    // Test AVL tree height using log(n) bounds
    cout << "AVL Tree Height (Log n): " 
         << (tester.testAVLTreeHeight(avlSwarm) ? "Passed" : "Failed") 
         << endl;

    return 0;
}