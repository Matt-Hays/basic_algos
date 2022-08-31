/**
 * @file BBSTSelect.cpp
 * @author Matthew Hays
 * @brief Takes in a command representing all nodes within the tree, inserts the nodes into a
 * Balanced Binary Search Tree, takes in a command representing the selection order and
 * returns the node located at the given selection order position.
 * @date 2022-06-1
 *
 * The time compelxity of the overall program is O(n)
 * and is dominated by the time necessary to read the nodes
 * from the command line and insert them into the tree.
 *
 * After initial insertion has completed, all further actions within the
 * tree occur in O(log(n)) time.
 *
 */

#include <iostream>
#include <cstddef>
#include <sstream>
#include <vector>

using namespace std;

/**
 * @brief Node structure.
 *
 * Contains additional features for height: to easily calculate the balance factor,
 * and the number of left and right children for each node: to assist with selection.
 *
 */
class Node
{
public:
    int value;
    Node *left;
    Node *right;
    int height;
    int numLeftChildren, numRightChildren;
};

/**
 * @brief Return the height of a given node.
 *
 * @param node A node.
 * @return int The height of the node.
 */
int height(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return node->height;
}

/**
 * @brief Creates a new node and initializes its
 * parameter. Public visibility for simplicity.
 *
 * @param value
 * @return Node*
 */
Node *newNode(int value)
{
    Node *node = new Node();
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->numLeftChildren = 0;
    node->numRightChildren = 0;
    return (node);
}

/**
 * @brief Right rotation.
 * 1. Store inital pointers.
 * 2. Connect the pivot as the left child for the new parent.
 * 3. Connect the new parent's left child as the pivot's right child.
 * 4. Update the number of children for each node involved in the rotation.
 * 5. Update the height of the nodes involved for further balance tests.
 *
 * @param pivotNode The unbalanced node about which the rotation will occur.
 * @return Node* The new parent node AFTER the rotation. To be connected back into the tree.
 */
Node *rightRotate(Node *pivotNode)
{
    Node *newPivot = pivotNode->left;
    Node *newPivotRightChild = newPivot->right;

    // Perform rotation
    newPivot->right = pivotNode;
    pivotNode->left = newPivotRightChild;

    // Store necessary details of intial config.
    int beginPivotLeftChildren = pivotNode->numLeftChildren;
    int beginNewPivotRightChildren = newPivot->numRightChildren;
    // Redetermine old pivot nodes new num of children.
    pivotNode->numLeftChildren = beginNewPivotRightChildren;
    newPivot->numRightChildren = pivotNode->numLeftChildren + pivotNode->numRightChildren + 1;

    // Recalculate heights
    pivotNode->height = max(height(pivotNode->left), height(pivotNode->right)) + 1;
    newPivot->height = max(height(newPivot->left), height(newPivot->right)) + 1;

    // Return new root
    return newPivot;
}

/**
 * @brief Left rotation.
 * 1. Store inital pointers.
 * 2. Connect the pivot as the right child for the new parent.
 * 3. Connect the new parent's right child as the pivot's left child.
 * 4. Update the number of children for each node involved in the rotation.
 * 5. Update the height of the nodes involved for further balance tests.
 *
 * @param pivotNode The unbalanced node about which the rotation will occur.
 * @return Node* The new parent node AFTER the rotation. To be connected back into the tree.
 */
Node *leftRotate(Node *pivotNode)
{
    Node *newPivot = pivotNode->right;
    Node *leftChildOfNewPivot = newPivot->left;

    // Perform rotation
    newPivot->left = pivotNode;
    pivotNode->right = leftChildOfNewPivot;

    // Store necessary details of intial config.
    int beginPivotRightChildren = pivotNode->numRightChildren;
    int beginNewPivotLeftChildren = newPivot->numLeftChildren;
    // Redetermine old pivot nodes new num of children.
    pivotNode->numRightChildren = beginNewPivotLeftChildren;
    newPivot->numLeftChildren = pivotNode->numLeftChildren + pivotNode->numRightChildren + 1;

    // Redetermine the node heights
    pivotNode->height = max(height(pivotNode->left), height(pivotNode->right)) + 1;
    newPivot->height = max(height(newPivot->left), height(newPivot->right)) + 1;

    // Return new root to connect to parent
    return newPivot;
}

/**
 * @brief Get the balance factor of a given node.
 *
 * @param node The node to calculate the balance factor.
 * @return int The balance factor.
 */
int getBalance(Node *node)
{
    if (node == NULL)
    {
        return 0;
    }
    return height(node->left) - height(node->right);
}

/**
 * @brief Inserts a new node into the Balanced Binary Search Tree.
 * 1. If no root: Insert the new node as the root.
 * 2. Determine if the value of the new node is greater than or less than that
 * of the currently in place node. Move down the tree accordingly.
 * 3. Insert the node as a leaf node (where the are no nodes currently).
 * 4. Determine the balance factor for the BBST after the insertion.
 * 4. Perform rotations upon unbalanced nodes as needed.
 *
 * @param node Initially, the root of the BBST. Recursion will update this
 *             parameter throughout the function's life cycle.
 * @param value The value of the new node.
 * @return Node* Each recursion will return the current node it is on.
 * Ultimately, the root node will be returned.
 */
Node *insert(Node *node, int value)
{
    // If no root supplied, add the node as the root.
    // OR
    // Add the node as a leaf node.
    if (node == NULL)
    {
        return (newNode(value));
    }
    // Is the node gt or lt the current node?
    // Insert the node as a leaf.
    // Build the number of children each node has as
    // derived from insertions.
    if (value < node->value)
    {
        node->numLeftChildren += 1;
        node->left = insert(node->left, value);
    }
    else if (value > node->value)
    {
        node->numRightChildren += 1;
        node->right = insert(node->right, value);
    }
    // No duplicates
    else
    {
        return node;
    }

    // Calculate the height of the node.
    node->height = max(height(node->left), height(node->right)) + 1;

    // Check the balance of the node.
    int balance = getBalance(node);

    // Left Left Rotation
    if (balance > 1 && value < node->left->value)
    {
        return rightRotate(node);
    }

    // Right Right Rotation
    if (balance < -1 && value > node->right->value)
    {
        return leftRotate(node);
    }

    // Left Right Rotation
    if (balance > 1 && value > node->left->value)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Rotation
    if (balance < -1 && value < node->right->value)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/**
 * @brief Takes a user requested selection statistic,
 * i.e., 12 representing the 12th lowest item in the BBST,
 * and returns a reference to the Node at that location.
 *
 * @param position The position of the desired node.
 * @param root The root node of the BBST.
 * @return Node* The desired node.
 */
Node *select(int position, Node *root)
{
    if (root != NULL)
    {
        Node *currNode = root;

        while (true)
        {
            // What is the current node's position?
            int myPosition = currNode->numLeftChildren + 1;
            // Is the desired position gt, lt, or equal to the current node's position?
            if (position > myPosition)
            {
                // Update the requested position to be equiv. to the desired position
                // considering only the right subtree portions of the BBST.
                position -= myPosition;
                // Move right
                currNode = currNode->right;
            }
            else if (position < myPosition)
            {
                // Move left - no need to update position
                currNode = currNode->left;
            }
            // Match condition
            else
            {
                return currNode;
            }
        }
    }
    // No BBST exists
    return NULL;
}

/**
 * @brief Program driver. Takes input from the command line.
 * 1st line = all space seperated node values to be inserted into the BBST.
 * 2nd line = a single integer representing the ordering statistic to be used
 *            in the select function.
 *
 * @return int A success status code of 0.
 */
int main()
{
    vector<int> inputNodes;
    string inputString, selectStatistic;

    // Create our root node for reference.
    Node *root = NULL;

    // Get the 1st line of input from the command line.
    getline(cin, inputString);

    stringstream ss(inputString);
    string node;
    // Create a vector from our space-delimited input string
    while (getline(ss, node, ' '))
    {
        inputNodes.push_back(stoi(node));
    }

    // Get the 2nd line of input from the command line.
    getline(cin, selectStatistic);

    // Create our BBST.
    for (int i = 0; i < inputNodes.size(); i++)
    {
        root = insert(root, inputNodes.at(i));
    }

    // Find and print the desired node by the given ordering statistic. i.e., 5th lowest value.
    cout << select(stoi(selectStatistic), root)->value << endl;

    return 0;
}
