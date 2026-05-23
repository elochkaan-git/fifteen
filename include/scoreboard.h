#pragma once
#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "structs.h"

class Scoreboard
{
public:
  Scoreboard() {
    for (auto &p : results) {
      p.name = "";
      p.rows = 0;
      p.cols = 0;
      p.steps = 0;
      p.time = 0;
      p.score = 0;
    }
    if (!std::filesystem::exists("score.csv")) {
      std::ofstream out("score.csv");
      out.close();
    } else if (std::filesystem::is_empty("score.csv")) {
    } else {
      int counter = 0;
      std::ifstream in("score.csv");
      std::vector<std::string> words;
      words.reserve(6);
      std::string line, word;
      while (counter < 16 && getline(in, line)) {
        words.clear();
        std::stringstream s(line);
        while (getline(s, word, ';'))
          words.push_back(word);
        if (words.size() < 6 || words.size() > 6) continue;
        GameResult p = {
          words[0],
          std::stoi(words[1]),
          std::stoi(words[2]),
          std::stoi(words[3]),
          std::stoi(words[4]),
          std::stoi(words[5])
        };
        results[counter++] = p;
      }
    }
    std::sort(results.begin(), results.end(), [](const GameResult& a, const GameResult& b) {
      if (a.name.empty() != b.name.empty())
        return !a.name.empty();
      return a.score > b.score;
    });
  }
  void print() {
    char buf[8];
    for (auto &p : results) {
      snprintf(buf, sizeof(buf), "%dx%d", p.rows, p.cols);
      printf("%20s %s %4d %4d %7d\n", p.name.c_str(), buf, p.steps, p.time, p.score);
    }
  }
  void save() const {
    std::ofstream out("score.csv");
    for (auto &p : results) {
      out << p.name << ';'
          << p.rows << ';'
          << p.cols << ';'
          << p.steps << ';'
          << p.time << ';'
          << p.score << '\n';
    }
    out.close();
  }
  void add(const GameResult& p) {
    if (p.score >= results[results.size() - 1].score) {
      results[results.size() - 1] = p;
    }
    for (int i = results.size() - 2; i >= 0; --i) {
      if (results[i].score <= results[i + 1].score)
        std::swap(results[i], results[i + 1]);
      else
        break;
    }
  }
  std::array<GameResult, 16>& get_results() { return results; }

private:
  std::array<GameResult, 16> results;
};