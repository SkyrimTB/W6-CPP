// From http://www.cprogramming.com/tutorial/lesson18.html

#include <memory>

struct node {
  int key_value;
  std::unique_ptr<node> left;
  std::unique_ptr<node> right;

  node(int key) : key_value(key), left(nullptr), right(nullptr) {}
};

class btree {
public:
  btree() = default;
  ~btree() = default;

  // Copy constructor
  btree(const btree &other) : root(copyTree(other.root)) {}

  // Copy assignment operator
  btree &operator=(const btree &other) {
    if (this != &other) {
      root = copyTree(other.root);
    }
    return *this;
  }

  void insert(int key) { insert(key, root); }

  node *search(int key) { return search(key, root.get()); }

  void destroy_tree() { root.reset(); }

private:
  std::unique_ptr<node> root;

  void insert(int key, std::unique_ptr<node> &leaf) {
    if (!leaf) {
      leaf = std::make_unique<node>(key);
    } else if (key < leaf->key_value) {
      insert(key, leaf->left);
    } else {
      insert(key, leaf->right);
    }
  }

  node *search(int key, node *leaf) {
    if (!leaf)
      return nullptr;
    if (key == leaf->key_value)
      return leaf;
    if (key < leaf->key_value)
      return search(key, leaf->left.get());
    else
      return search(key, leaf->right.get());
  }

  std::unique_ptr<node> copyTree(const std::unique_ptr<node> &source) {
    if (!source)
      return nullptr;
    auto newNode = std::make_unique<node>(source->key_value);
    newNode->left = copyTree(source->left);
    newNode->right = copyTree(source->right);
    return newNode;
  }
};

// Improvement：
// 1. Use std::unique_ptr to manage memory, so you dont need for manual deletion
// of memory, which prevents memory leaks.
// 2. Use std::make_unique to create a new node, which is more exception safe
// than new.
// 3. Implements a copy constructor and copy assignment operator so that the
// class can be deep copied correctly, which prevented issues with shared or
// dangling pointers
// 4. Utilizes default constructor and destructor, which is more idiomatic in
// modern C++. Overall, this version takes advantage of C++'s RAII principle,
// which makes the code more exception safe and easier to read.
