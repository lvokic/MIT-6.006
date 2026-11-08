#include <algorithm>

struct Node {
    Node* left;
    Node* right;
    Node* parent;
    int   val_;
    int   height_;
    
    Node() = default;
    Node(int value, int height = 1) {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        val_ = value;
        height_ = height;
    }
};

class AVLTree {
private:
    Node* root{nullptr};

private:
    int height(Node* node) {
        if (!node) return 0;
        return std::max(height(node->left), height(node->right)) + 1;
    }

    int storedHeight(const Node* node) { return node ? node->height_ : 0; }

    int balanceFactor(const Node* node) { return storedHeight(node->left) - storedHeight(node->right); }

    void updateHeight(Node* node) {
        while (node) {
            int new_height = 1 + std::max(
                storedHeight(node->left),
                storedHeight(node->right)
            );
            if (new_height == node->height_)
                break;
            node->height_ = new_height;
            node = node->parent;
        }
    }

    Node* successor(Node* node) {
        Node* right_child = node->right;
        if (!right_child) {
            while (node->parent) {
                Node* parent = node->parent;
                if (parent->left == node)
                    return parent;
                node = node->parent;
            }
            return node->parent;
        } else {
            while (right_child->left)
                right_child = right_child->left;
            return right_child;
        }
    }

    Node* predecessor(Node* node) {
        Node* left = node->left;
        if (!left) {
            while (node->parent) {
                Node* parent = node->parent;
                if (parent->right == node)
                    return parent;
                node = node->parent;
            }
            return node->parent;
        } else {
            while (left->right)
                left = left->right;
            return left;
        }
    }

    Node* leftRotate(Node* node) {
        Node* right = node->right;
        if (!right) return nullptr;

        node->right = right->left;
        if (node->right)
            node->right->parent = node;
        right->parent = node->parent;
        if (node == root) 
            root = right;
        else {
            if (node->parent->left == node)
                node->parent->left = right;
            else node->parent->right = right;
        }
        node->parent = right;
        right->left = node;

        node->height_ = 1 + std::max(storedHeight(node->left), storedHeight(node->right));
        right->height_ = 1 + std::max(storedHeight(right->left), storedHeight(right->right));
        return right;
    }

    Node* rightRotate(Node* node) {
        Node* left = node->left;
        if (!left) return nullptr;

        node->left = left->right;
        if (node->left)
            node->left->parent = node;
        left->parent = node->parent;
        if (node == root) 
            root = left;
        else {
            if (node->parent->left == node)
                node->parent->left = left;
            else node->parent->right = left;
        }
        node->parent = left;
        left->right = node;

        node->height_ = 1 + std::max(storedHeight(node->left), storedHeight(node->right));
        left->height_ = 1 + std::max(storedHeight(left->left), storedHeight(left->right));
        return left;
    }

    void release_node(Node* node) {
        if (node->parent) {
            if (node == node->parent->left)
                node->parent->left = nullptr;
            else node->parent->right = nullptr;
            node->parent = nullptr;
        } else {
            root = nullptr;
        }
        node->left = nullptr;
        node->right = nullptr;
        delete node;
    }

    void replace_node(Node* node, Node* replacement) {
        if (node == root)
            root = replacement;
        replacement->left = node->left;
        replacement->right = node->right;
        replacement->parent = node->parent;
        replacement->height_ = node->height_;
        if (replacement->left)
            replacement->left->parent = replacement;
        if (replacement->right)
            replacement->right->parent = replacement;
        if (replacement->parent) {
            if (node->parent->left == node)
                replacement->parent->left = replacement;
            else
                replacement->parent->right = replacement;
        }

        node->left = nullptr;
        node->right = nullptr;
        node->parent = nullptr;
        delete node;
    }

public:
    AVLTree() = default;

    void insert(int val) {
        if (!root) {
            root = new Node(val);
            return;
        }
        Node* prev = nullptr;
        Node* node = root;
        while (node) {
            prev = node;
            if (val <= node->val_)
                node = node->left;
            else
                node = node->right;
        }
        Node* new_node = new Node(val);
        new_node->parent = prev;
        if (val <= prev->val_)
            prev->left = new_node;
        else
            prev->right = new_node;
        
        inser_fix(new_node);
    }

    void inser_fix(Node* new_node) {
        Node *node = new_node->parent;
        while (node) {
            node->height_ = 1 + std::max(storedHeight(node->left), storedHeight(node->right));
            int balance = balanceFactor(node);
            if (balance > 1) {
                int left_balance = balanceFactor(node->left);
                if (left_balance == -1) node->left = leftRotate(node->left);
                node = rightRotate(node);
            } else if (balance < -1) {
                int right_balance = balanceFactor(node->right);
                if (right_balance == 1) node->right = rightRotate(node->right);
                node = leftRotate(node);
            } 
            node = node->parent;
        }        
    }

    void erase(int val) {
        Node* node = root;
        while (node) {
            if (val < node->val_)
                node = node->left;
            else if (val > node->val_)
                node = node->right;
            else
                break;
        }
        if (node == nullptr) return;
        if (!node->left && !node->right) {
            Node* parent = node->parent;
            release_node(node);
            erase_fix(parent);
        } else if (node->left) {
            Node* predecessor_node = predecessor(node);
            Node* parent = predecessor_node->parent;
            Node* fix_start = (parent == node) ? predecessor_node : parent;
            if (parent) {
                if (predecessor_node->left) {
                    if (parent->left == predecessor_node) {
                        parent->left = predecessor_node->left;
                    } else {
                        parent->right = predecessor_node->left;
                    }
                    predecessor_node->left->parent = parent;
                } else {
                    if (parent->left == predecessor_node) 
                        parent->left = nullptr;
                    else
                        parent->right = nullptr;
                }
            }

            replace_node(node, predecessor_node);
            erase_fix(fix_start);
        } else {
            Node* successor_node = successor(node);
            Node* parent = successor_node->parent;
            Node* fix_start = (parent == node) ? successor_node : parent;
            if (parent) {
                if (successor_node->right) {
                    if (parent->left == successor_node)
                        parent->left = successor_node->right;
                    else
                        parent->right = successor_node->right;
                    successor_node->right->parent = parent;
                } else {
                    if (parent->left == successor_node) 
                        parent->left = nullptr;
                    else
                        parent->right = nullptr;
                }
            }

            replace_node(node, successor_node);
            erase_fix(fix_start);
        }
        
    }

    void erase_fix(Node* node) {
        while (node) {
            node->height_ = 1 + std::max(storedHeight(node->left), storedHeight(node->right));
            int balance = balanceFactor(node);
            if (balance > 1) {
                int left_balance = balanceFactor(node->left);
                if (left_balance < 0) node->left = leftRotate(node->left);
                node = rightRotate(node);
            } else if (balance < -1) {
                int right_balance = balanceFactor(node->right);
                if (right_balance > 0) node->right = rightRotate(node->right);
                node = leftRotate(node);
            } 
            node = node->parent;
        }        
    }

};
