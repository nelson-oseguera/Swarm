#ifndef SWARM_H
#define SWARM_H

#include <iostream>
using namespace std;

class Grader;  // This class is for grading purposes, no need to do anything
class Tester;  // This is your tester class, you add your test functions in this class

class Swarm;

// Enum declarations
enum STATE {ALIVE, DEAD};   // Possible states for a robot
enum ROBOTTYPE {BIRD, DRONE, REPTILE, SUB, QUADRUPED};
enum TREETYPE {NONE, BST, AVL, SPLAY};

// Constants
const int MINID = 10000;    // Min robot ID
const int MAXID = 99999;    // Max robot ID

#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE DRONE
#define DEFAULT_STATE ALIVE

// Robot class definition
class Robot {
public:
    friend class Swarm;
    friend class Grader;
    friend class Tester;

    Robot(int id, ROBOTTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        : m_id(id), m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }

    Robot() {
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }

    int getID() const { return m_id; }
    STATE getState() const { return m_state; }
    string getStateStr() const {
        string text = "";
        switch (m_state) {
        case DEAD: text = "DEAD"; break;
        case ALIVE: text = "ALIVE"; break;
        default: text = "UNKNOWN"; break;
        }
        return text;
    }
    
    ROBOTTYPE getType() const { return m_type; }
    string getTypeStr() const {
        string text = "";
        switch (m_type) {
        case BIRD: text = "BIRD"; break;
        case DRONE: text = "DRONE"; break;
        case REPTILE: text = "REPTILE"; break;
        case SUB: text = "SUB"; break;
        case QUADRUPED: text = "QUADRUPED"; break;
        default: text = "UNKNOWN"; break;
        }
        return text;
    }
    
    int getHeight() const { return m_height; }
    Robot* getLeft() const { return m_left; }
    Robot* getRight() const { return m_right; }

    void setID(const int id) { m_id = id; }
    void setState(STATE state) { m_state = state; }
    void setType(ROBOTTYPE type) { m_type = type; }
    void setHeight(int height) { m_height = height; }
    void setLeft(Robot* left) { m_left = left; }
    void setRight(Robot* right) { m_right = right; }

private:
    int m_id;           // Unique ID for the robot
    ROBOTTYPE m_type;   // Type of robot
    STATE m_state;      // State of the robot (alive/dead)
    Robot* m_left;      // Pointer to the left child in the BST
    Robot* m_right;     // Pointer to the right child in the BST
    int m_height;       // Height of the node (for AVL balancing)
};

// Swarm class definition
class Swarm {
public:
    friend class Grader;
    friend class Tester;

    // Constructors, Destructor
    Swarm();
    Swarm(TREETYPE type);
    ~Swarm();  // Destructor

    // Overloaded assignment operator
    const Swarm& operator=(const Swarm& rhs);

    // Basic tree operations
    void insert(const Robot& robot);
    void remove(int id);
    void clear();
    TREETYPE getType() const;
    void setType(TREETYPE type);

    // Tree printing (in-order traversal)
    void dumpTree() const;

private:
    Robot* m_root;      // Root of the BST
    TREETYPE m_type;    // Type of tree (NONE, BST, AVL, SPLAY)

    // Private helper functions
    Robot* insertHelper(Robot* root, Robot* newRobot);   // Helper for insertion
    Robot* removeHelper(Robot* root, int id);            // Helper for removal
    Robot* getMinNode(Robot* root) const;                // Helper to find min node
    Robot* clone(Robot* root) const;                     // Deep copy helper

    // AVL tree balancing helpers
    Robot* balanceTree(Robot* node);                     // Balance the tree (for AVL)
    int getHeight(Robot* node) const;                    // Get node height
    int getBalance(Robot* node) const;                   // Get balance factor
    Robot* leftRotate(Robot* x);                         // Left rotation
    Robot* rightRotate(Robot* y);                        // Right rotation

    // Splay tree helper
    Robot* splay(Robot* root, int id);                   // Splay a node to the root

    // Helper for dumping tree (in-order traversal)
    void dump(Robot* aBot) const;

    // Memory management helpers
    void clearHelper(Robot* node);  // Recursively deallocate memory

    // Helper to rebuild the tree as AVL without STL
    void rebuildAVL(Robot* node);
};

#endif