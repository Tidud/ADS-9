// Copyright 2022 NNTU-CS
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <locale>
#include "tree.h"

long long calculateSafeFactorial(int value) {
  if (value < 0) return 0;
  if (value == 0) return 1;

  long long current_product = 1;
  for (int i = 1; i <= value; ++i) {
    if (current_product > 0 && i > (9223372036854775807LL / current_product)) {
      return -1;
    }
    current_product *= i;
  }
  return current_product;
}

PMTree::PMTree(std::vector<char> in) {
  root = new Node('\0');
  std::vector<char> sorted_elements = in;
  std::sort(sorted_elements.begin(), sorted_elements.end());
  buildTree(root, sorted_elements);
}

PMTree::~PMTree() { deleteTree(root); }

void PMTree::buildTree(Node* parent_node,
                       std::vector<char> available_elements) {
  if (available_elements.empty()) {
    return;
  }

  std::sort(available_elements.begin(), available_elements.end());

  for (size_t i = 0; i < available_elements.size(); ++i) {
    char chosen_char = available_elements[i];
    Node* new_child_node = new Node(chosen_char);
    parent_node->children.push_back(new_child_node);

    std::vector<char> remaining_for_next_level;
    for (size_t j = 0; j < available_elements.size(); ++j) {
      if (i != j) {
        remaining_for_next_level.push_back(available_elements[j]);
      }
    }
    buildTree(new_child_node, remaining_for_next_level);
  }
}

void PMTree::deleteTree(Node* node_to_delete) {
  if (node_to_delete == nullptr) {
    return;
  }
  for (Node* child_ptr : node_to_delete->children) {
    deleteTree(child_ptr);
  }
  delete node_to_delete;
}

static void recursivePermutationCollector(
    PMTree::Node* current_tree_node,
    std::vector<char>& current_permutation_path,
    std::vector<std::vector<char>>& all_permutations_output,
    size_t expected_length) {
  if (current_tree_node->data != '\0') {
    current_permutation_path.push_back(current_tree_node->data);
  }

  if (current_tree_node->children.empty()) {
    if (current_permutation_path.size() == expected_length) {
      all_permutations_output.push_back(current_permutation_path);
    }
  } else {
    for (PMTree::Node* child_of_node : current_tree_node->children) {
      recursivePermutationCollector(child_of_node, current_permutation_path,
                                    all_permutations_output, expected_length);
    }
  }

  if (current_tree_node->data != '\0') {
    current_permutation_path.pop_back();
  }
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> collected_permutations;
  std::vector<char> temporary_permutation_elements;

  size_t initial_elements_count = tree.getRoot()->children.size();

  long long estimated_total_perms = calculateSafeFactorial(
      static_cast<int>(initial_elements_count));  // Преобразование size_t в int
  if (estimated_total_perms > 0) {
    collected_permutations.reserve(static_cast<size_t>(estimated_total_perms));
  }

  recursivePermutationCollector(tree.getRoot(), temporary_permutation_elements,
                                collected_permutations, initial_elements_count);
  return collected_permutations;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  std::vector<std::vector<char>> all_generated_perms = getAllPerms(tree);

  if (num <= 0 || static_cast<size_t>(num) >
                      all_generated_perms.size()) {  // Сравнение int с size_t
    return {};
  }

  return all_generated_perms[static_cast<size_t>(
      num - 1)];  // Преобразование int в size_t для индекса
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  if (num <= 0) {
    return {};
  }

  std::vector<char> resulting_permutation;
  PMTree::Node* current_traversal_node = tree.getRoot();
  long long zero_based_query_index = num - 1;

  std::vector<char> available_elements_pool;
  for (PMTree::Node* child_option : current_traversal_node->children) {
    available_elements_pool.push_back(child_option->data);
  }
  size_t total_original_elements_size =
      available_elements_pool.size();  // Используем size_t

  long long maximum_possible_permutations =
      calculateSafeFactorial(static_cast<int>(
          total_original_elements_size));  // Преобразуем size_t в int
  if (maximum_possible_permutations == -1 ||
      zero_based_query_index >= maximum_possible_permutations) {
    return {};
  }

  for (size_t i = total_original_elements_size; i >= 1;
       --i) {  // Цикл по size_t
    if (available_elements_pool.empty()) {
      return {};
    }

    long long permutations_in_sub_branch = calculateSafeFactorial(
        static_cast<int>(i - 1));  // Преобразуем size_t в int
    if (permutations_in_sub_branch == -1) {
      return {};
    }

    int selected_element_index =
        static_cast<int>(zero_based_query_index / permutations_in_sub_branch);

    if (static_cast<size_t>(selected_element_index) >=
        available_elements_pool.size()) {
      return {};
    }

    char chosen_element_value = available_elements_pool[static_cast<size_t>(
        selected_element_index)];  // Преобразуем int в size_t
    resulting_permutation.push_back(chosen_element_value);

    zero_based_query_index %= permutations_in_sub_branch;

    available_elements_pool.erase(
        available_elements_pool.begin() +
        static_cast<size_t>(
            selected_element_index));  // Преобразуем int в size_t
  }

  if (resulting_permutation.size() == total_original_elements_size) {
    return resulting_permutation;
  } else {
    return {};
  }
}
