#include "abseil-cpp/absl/container/btree_set.h"
#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <set>
#include <vector>

#include "fast_B-trees/include/dynamic_search.hpp"
#include "fast_B-trees/include/static_search.hpp"

void BM_std_set_pred(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  auto e = set.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.lower_bound(v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_set_pred)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_set_contains(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.contains(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_set_contains)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_set_access(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries) {
      q_vec.push_back(val);
    }
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *set.lower_bound(v);
    }
  }
  state.SetLabel(std::to_string(-checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_set_access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_set_sum(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);

  std::set<int64_t> set;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : set) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * set.size());
}
BENCHMARK(BM_std_set_sum)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_set_insert(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.erase(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_set_insert)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_set_remove(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.erase(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_set_remove)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_access(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries) {
      q_vec.push_back(val);
    }
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *set.predecessor(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_set_access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_sum(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);

  bt::dynamic_set<int64_t> set;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : set) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * set.size());
}
BENCHMARK(BM_bt_set_sum)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_pred(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  auto e = set.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.predecessor(v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_set_pred)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_contains(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.contains(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_set_contains)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_insert(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.remove(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_set_insert)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_set_remove(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.remove(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_set_remove)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_access(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;

  absl::btree_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries) {
      q_vec.push_back(val);
    }
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *set.lower_bound(v);
    }
  }
  state.SetLabel(std::to_string(-checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_absl_set_access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_sum(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);

  absl::btree_set<int64_t> set;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : set) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * set.size());
}
BENCHMARK(BM_absl_set_sum)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_pred(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  absl::btree_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  auto e = set.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.lower_bound(v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_absl_set_pred)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_contains(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  absl::btree_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += set.contains(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_absl_set_contains)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_insert(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  absl::btree_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.erase(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_absl_set_insert)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_absl_set_remove(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  absl::btree_set<int64_t> set;
  std::vector<int64_t> q_vec;

  while (set.size() < n - queries / 2) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    q_vec.push_back(val);
  }

  for (auto _ : state) {
    for (auto v : q_vec) {
      set.erase(v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      set.insert(v);
    }
    state.ResumeTiming();
  }
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_absl_set_remove)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_vec_access(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> vec;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries) {
      q_vec.push_back(val);
    }
  }

  std::sort(vec.begin(), vec.end());
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *std::lower_bound(vec.begin(), vec.end(), v);
    }
  }
  state.SetLabel(std::to_string(-checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_vec_access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_vec_sum(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
  }

  std::sort(vec.begin(), vec.end());
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : vec) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * vec.size());
}
BENCHMARK(BM_std_vec_sum)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_vec_pred(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  std::sort(vec.begin(), vec.end());
  auto e = vec.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += std::lower_bound(vec.begin(), vec.end(), v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_vec_pred)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_std_vec_contains(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = -dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  std::sort(vec.begin(), vec.end());
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += std::binary_search(vec.begin(), vec.end(), v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_std_vec_contains)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_flatset_access(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> vec;
  std::vector<int64_t> q_vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries) {
      q_vec.push_back(val);
    }
  }

  bt::static_set s_set(vec);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *s_set.predecessor(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_flatset_access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_flatset_sum(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);

  bt::dynamic_set<int64_t> set;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
  }

  bt::static_set s_set(vec);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : s_set) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * s_set.size());
}
BENCHMARK(BM_bt_flatset_sum)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_flatset_pred(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  bt::static_set s_set(vec);
  auto e = s_set.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += s_set.predecessor(v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_flatset_pred)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

void BM_bt_flatset_contains(benchmark::State& state) {
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> dist(0, 100000000);
  size_t n = state.range(0);
  const constexpr size_t queries = 1000;
  std::set<int64_t> set;
  std::vector<int64_t> q_vec;
  std::vector<int64_t> vec;

  while (set.size() < n) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    set.insert(val);
    vec.push_back(val);
    if (q_vec.size() < queries / 2) {
      q_vec.push_back(val);
    }
  }

  while (q_vec.size() < queries) {
    int64_t val = dist(gen);
    if (set.contains(val)) {
      continue;
    }
    q_vec.push_back(val);
  }

  std::shuffle(q_vec.begin(), q_vec.end(), gen);
  bt::static_set s_set(vec);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += s_set.contains(v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
BENCHMARK(BM_bt_flatset_contains)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

BENCHMARK_MAIN();