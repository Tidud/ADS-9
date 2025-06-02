// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

class PMTree {
 public:
  struct Node {
    char data;
    std::vector<Node*> children;

    Node(char val = '\0') : data(val) {}
  };

  PMTree(std::vector<char> in);
  ~PMTree();

  Node* getRoot() const { return root; }

 private:
  Node* root;
  void buildTree(Node* parent_node, std::vector<char> available_elements);

  void deleteTree(Node* node);
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
