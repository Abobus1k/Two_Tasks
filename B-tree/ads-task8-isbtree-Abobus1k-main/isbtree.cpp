#include <fstream>
#include <iostream>
class Node {
public:
    int* keys;
    int t;
    Node** children;
    int n;
    bool leaf;
    int index = 0;
    int* childrenIndexes;
    int keys_num;
    int level = 0;

    Node(int _t, bool _leaf) {
        t = _t;
        leaf = _leaf;
        keys = new int[2 * t - 1];
        children = new Node * [2 * t];
        childrenIndexes = new int[2 * t];
        n = 0;
        keys_num = 0;
    }

    ~Node() {
        delete[] keys;
        delete[] children;
        delete[] childrenIndexes;
    }
};

void quickSort(Node** arr, int l, int r) {
    if (l >= r) {
        return;
    }

    int pivot = arr[l + (r - l) / 2]->index;
    int i = l - 1;
    int j = r + 1;

    while (true) {
        do {
            i++;
        } while (arr[i]->index < pivot);

        do {
            j--;
        } while (arr[j]->index > pivot);

        if (i >= j) {
            break;
        }

        Node* temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    quickSort(arr, l, j);
    quickSort(arr, j + 1, r);
}

Node* binarySearch(Node** nodes, int len, int target) {
    int left = 0;
    int right = len - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nodes[mid]->index == target) {
            return nodes[mid];
        }
        if (nodes[mid]->index < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr;
}
int numOfLeaves = 0;
int buildTree(Node* node, Node** nodes, int len, int l, int r,
    int level, Node **& leaves) {
    Node* child;
    if (node->keys[0] < l || node->keys[node->n - 1] > r) {
        return -5;
    }
    for (int i = 1; i < node->n; ++i) {
        if (node->keys[i - 1] > node->keys[i]) {
            return -6;
        }
    }
    if (node->leaf) {
        leaves[numOfLeaves++]=node;
        return level;
    }
    for (int i = 0; i < node->keys_num; ++i) {
        child = binarySearch(nodes, len, node->childrenIndexes[i]);
        if (child == nullptr) {
            return -7;
        }
        child->level = level + 1;
        node->children[i] = child;
    }
    for (int i = 0; i < node->keys_num; ++i) {
        int cur_left;
        int cur_right;
        if (i - 1 >= 0) {
            cur_left = node->keys[i - 1];
        }
        else {
            cur_left = l;
        }
        if (i < node->n) {
            cur_right = node->keys[i];
        }
        else {
            cur_right = r;
        }
        int tmp = buildTree(node->children[i], nodes, len, std::max(cur_left, l),
            std::min(cur_right, r), level+1,leaves);
        if (tmp < 0)
        {
            return -1;
        }
    }

    return 0;
}
bool isInvalidNode(Node* node, int t, int root, int len) {
    if (node->index == root) {
        if (node->n < 1 || node->n > 2 * t - 1 ||
            (!node->leaf && (node->keys_num < 2 || node->keys_num > 2 * t))) {
            return true;
        }
    }
    else {
        if (node->n < t - 1 || node->n > 2 * t - 1 ||
            (!node->leaf && node->n != node->keys_num - 1)) {
            return true;
        }
    }
    return false;
}
bool isEqualLevel(Node** leaves)
{
    int lvl = leaves[0]->level;
    for (int i = 0; i < numOfLeaves; i++)
    {
        if (leaves[i]->level!=lvl) {
            return false;
        }
    }
    return true;
}
int main(int argc, char* argv[]) {
    std::ifstream in(argv[1]);
    int N;
    int t;
    int root;
    in >> N >> t >> root;
    Node** leaves = new Node * [N];
    char ch;
    bool check = false;
    Node** nodes = new Node * [N];
    std::string word;
    int max = -1000001;
    int min = 1000001;
    bool leaf = false;
    for (int i = 0; i < N; i++) {
        nodes[i] = new Node(t, leaf);
        nodes[i]->t = t;
        in >> word;
        if (word == "leaf:") {
            nodes[i]->leaf = true;
            in >> ch;
            in >> ch;
            in >> nodes[i]->index >> ch >> nodes[i]->n >> ch;

            for (int j = 0; j < nodes[i]->n; j++) {
                in >> nodes[i]->keys[j];
                if (nodes[i]->keys[j] < min) {
                    min = nodes[i]->keys[j];
                }
                else if (nodes[i]->keys[j] > max) {
                    max = nodes[i]->keys[j];
                }
            }

        }
        else if (word == "branch:") {
            nodes[i]->leaf = false;
            int node;
            in >> ch;
            in >> ch;
            in >> nodes[i]->index >> ch >> nodes[i]->n >> ch;
            for (int j = 0; j < nodes[i]->n; j++) {
                in >> nodes[i]->keys[j];
                if (nodes[i]->keys[j] < min) {
                    min = nodes[i]->keys[j];
                }
                else if (nodes[i]->keys[j] > max) {
                    max = nodes[i]->keys[j];
                }
            }
            in >> ch >> ch >> nodes[i]->keys_num >> ch;
            for (int j = 0; j < nodes[i]->keys_num; j++) {
                in >> nodes[i]->childrenIndexes[j];
            }
        }

        if (isInvalidNode(nodes[i], t, root, N)) {
            std::cout << "no";
            check = true;
            break;
        }

        in >> ch;
    }
    if (!check) {
        quickSort(nodes, 0, N - 1);
        int result =
            buildTree(binarySearch(nodes, N, root), nodes, N, min, max,0,leaves);

        if (result < 0 || !isEqualLevel(leaves)) {
            std::cout << "no";
        }
        else {
            std::cout << "yes";
        }
    }
    else {
        for (int i = 0; i < N; i++) {
            delete nodes[i];
        }
        delete[] nodes;
        delete[] leaves;
        in.close();
        return 0;
    }
    for (int i = 0; i < N; i++) {
        delete nodes[i];
    }
    delete[] nodes;
    delete[] leaves;
    in.close();
    return 0;
}
