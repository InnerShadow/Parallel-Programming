
#include "Tree.hpp"

Tree::Node::Node(int data, Node* left = nullptr, Node* right = nullptr) {
    this->data = data;
    this->left = left;
    this->right = right;
}

Tree::Tree(int n) {
    root = buildTree(0, n);

    FillTree();
}

Tree::Node* Tree::buildTree(int left, int right) {
    if (left > right) {
        return nullptr;
    }

    int mid = left + (right - left) / 2;
    Node* node = new Node(0);

    node->left = buildTree(left, mid - 1);
    if (node->left) {
        node->left->father = node;
    }
    
    node->right = buildTree(mid + 1, right);
    if (node->right) {
        node->right->father = node;
    }
    
    return node;
}

void Tree::FillTree() {
    if (!root) {
        return;
    }
    std::queue<Node*> q;
    q.push(root);

    int i = 0;

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        node->data = i++;

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

void Tree::bfs() {
    if (!root) {
        return ;
    }
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        std::cout << node->data << " ";

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

void Tree::RootToAll(int argc, char** argv) {
    if (!root) {
        return;
    }

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string message("Good morning world!");

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* node = q.front();
        q.pop();

        if (node->data == rank && node->right && node->left) {
            MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, node->left->data, 0, MPI_COMM_WORLD);
            MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, node->right->data, 0, MPI_COMM_WORLD);
        }
        if (node->data == rank && node->father) {
            char buffer[100];
            MPI_Recv(buffer, 100, MPI_CHAR, node->father->data, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Received message: " << buffer << ", procese's rank = " << rank << ", get message from " << node->father->data << std::endl;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    if (rank == 0) {
        print();
    }

    MPI_Finalize();
}

void Tree::remove(Node* root) {
    if (root) {
        remove(root->left);
        remove(root->right);
        delete root;
        root = NULL;
    }
}

Tree::~Tree() {
    this->remove(root);
    root = nullptr;
}

void Tree::print_tree_level(Node* top, int level) {
    if (top) {
        print_tree_level(top->left, level + 1);
        for (int i = 0; i < level; i++)
            std::cout << "   ";
        std::cout << top->data << "\n";
        print_tree_level(top->right, level + 1);
    }
}

void Tree::print() {
    print_tree_level(this->root, 1);
}
