#include "scoreboard.h"
#include <cassert>

int main() {
  Scoreboard sb;
  sb.get_results();
  sb.save();
  return 0;
}
