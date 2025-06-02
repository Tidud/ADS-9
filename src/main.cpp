// Copyright 2022 NNTU-CS
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include "tree.h"

int64_t calculateSafeFactorial(int value);

int main() {
  const int MIN_N = 1;
  const int MAX_N = 9;

  std::random_device rd;
  std::mt19937 gen(rd());

  for (int n = MIN_N; n <= MAX_N; ++n) {
    std::vector<char> alphabet(n);
    for (int i = 0; i < n; ++i) {
      alphabet[i] = static_cast<char>('a' + i);
    }

    PMTree tree(alphabet);

    auto start_getAllPerms = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<char>> all_perms = getAllPerms(tree);
    auto end_getAllPerms = std::chrono::high_resolution_clock::now();
    long long duration_getAllPerms =
        std::chrono::duration_cast<std::chrono::microseconds>(end_getAllPerms -
                                                              start_getAllPerms)
            .count();

    long long total_perms_count = calculateSafeFactorial(n);
    if (total_perms_count == -1) {
      std::cout << n << "," << duration_getAllPerms << ",-1,-1\n";
      continue;
    }

    std::uniform_int_distribution<long long> dist(1, total_perms_count);

    const int NUM_SAMPLES = 100;
    long long total_duration_getPerm1 = 0;
    long long total_duration_getPerm2 = 0;

    for (int i = 0; i < NUM_SAMPLES; ++i) {
      long long random_perm_num = dist(gen);

      auto start_getPerm1 = std::chrono::high_resolution_clock::now();
      std::vector<char> perm1_result =
          getPerm1(tree, static_cast<int>(random_perm_num));
      auto end_getPerm1 = std::chrono::high_resolution_clock::now();
      total_duration_getPerm1 +=
          std::chrono::duration_cast<std::chrono::microseconds>(end_getPerm1 -
                                                                start_getPerm1)
              .count();

      auto start_getPerm2 = std::chrono::high_resolution_clock::now();
      std::vector<char> perm2_result =
          getPerm2(tree, static_cast<int>(random_perm_num));
      auto end_getPerm2 = std::chrono::high_resolution_clock::now();
      total_duration_getPerm2 +=
          std::chrono::duration_cast<std::chrono::microseconds>(end_getPerm2 -
                                                                start_getPerm2)
              .count();
    }

    double avg_duration_getPerm1 =
        static_cast<double>(total_duration_getPerm1) / NUM_SAMPLES;
    double avg_duration_getPerm2 =
        static_cast<double>(total_duration_getPerm2) / NUM_SAMPLES;

    std::cout << n << " " << duration_getAllPerms << " "
              << avg_duration_getPerm1 << " " << avg_duration_getPerm2 << "\n";
  }

  return 0;
}
