#include <benchmark/benchmark.h>

#include <algorithm>
#include <flat_set>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include "absl/container/btree_set.h"
#include "fast_B-trees/include/dynamic_search.hpp"
#include "fast_B-trees/include/static_search.hpp"
#include "op_templates.hpp"

#define NUMS ->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000)->Arg(10000000)->Arg(100000000)

template <class set_t, class T>
void BM_pred(benchmark::State& state) {
  size_t n = state.range(0);
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n, queries / 2, queries / 2, q_vec);

  auto e = set.end();
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += pt::with<set_t, T>::pred(set, v) != e;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
template<class set_t, class T>
void BM_contains(benchmark::State& state) {
  size_t n = state.range(0);
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n, queries / 2, queries / 2, q_vec);
  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += pt::with<set_t, T>::contains(set, v);
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}

template<class set_t, class T>
void BM_access(benchmark::State& state) {
  size_t n = state.range(0);
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n, queries, 0, q_vec);

  T checksum{};
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += *pt::with<set_t, T>::pred(set, v);
    }
  }
  state.SetLabel(std::to_string(-checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}

template<class set_t, class T>
void BM_sum(benchmark::State& state) {
  size_t n = state.range(0);
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n, 0, 0, q_vec);

  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : set) {
      checksum += v;
    }
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * set.size());
}

template<class set_t, class T>
void BM_insert(benchmark::State& state) {
  size_t n = state.range(0);
  const size_t queries = std::min(q_count, n);
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n - queries / 2, 0, queries, q_vec);

  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += pt::with<set_t, T>::insert(set, v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      pt::with<set_t, T>::remove(set, v);
    }
    state.ResumeTiming();
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}


template<class set_t, class T>
void BM_remove(benchmark::State& state) {
  size_t n = state.range(0);
  const size_t queries = std::min(q_count, n);
  std::vector<T> q_vec;
  set_t set = pt::with<set_t, T>::build(n + queries / 2, queries, 0, q_vec);

  int64_t checksum = 0;
  for (auto _ : state) {
    for (auto v : q_vec) {
      checksum += pt::with<set_t, T>::remove(set, v);
    }
    state.PauseTiming();
    for (auto v : q_vec) {
      pt::with<set_t, T>::insert(set, v);
    }
    state.ResumeTiming();
  }
  state.SetLabel(std::to_string(checksum));
  state.SetItemsProcessed(state.iterations() * queries);
}
#ifdef TEST_STD_SET_INT
BENCHMARK(BM_pred<std::set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_sum<std::set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_access<std::set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_contains<std::set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_remove<std::set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_insert<std::set<int64_t>, int64_t>)NUMS;
#endif
#ifdef TEST_STD_SET_DOUBLE
BENCHMARK(BM_pred<std::set<double>, double>)NUMS;
BENCHMARK(BM_sum<std::set<double>, double>)NUMS;
BENCHMARK(BM_access<std::set<double>, double>)NUMS;
BENCHMARK(BM_contains<std::set<double>, double>)NUMS;
BENCHMARK(BM_remove<std::set<double>, double>)NUMS;
BENCHMARK(BM_insert<std::set<double>, double>)NUMS;
#endif
#ifdef TEST_STD_SET_DEC
BENCHMARK(BM_pred<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_sum<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_access<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_contains<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_remove<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_insert<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
#endif

#ifdef TEST_STD_FLAT_SET_INT
BENCHMARK(BM_pred<std::flat_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_sum<std::flat_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_access<std::flat_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_contains<std::flat_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_remove<std::flat_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_insert<std::flat_set<int64_t>, int64_t>)NUMS;
#endif
#ifdef TEST_STD_FLAT_SET_DOUBLE
BENCHMARK(BM_pred<std::flat_set<double>, double>)NUMS;
BENCHMARK(BM_sum<std::flat_set<double>, double>)NUMS;
BENCHMARK(BM_access<std::flat_set<double>, double>)NUMS;
BENCHMARK(BM_contains<std::flat_set<double>, double>)NUMS;
BENCHMARK(BM_remove<std::flat_set<double>, double>)NUMS;
BENCHMARK(BM_insert<std::flat_set<double>, double>)NUMS;
#endif
#ifdef TEST_STD_FLAT_SET_DEC
BENCHMARK(BM_pred<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_sum<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_access<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_contains<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_remove<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_insert<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
#endif

#ifdef TEST_ABSL_BTREE_SET_INT
BENCHMARK(BM_pred<absl::btree_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_sum<absl::btree_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_access<absl::btree_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_contains<absl::btree_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_remove<absl::btree_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_insert<absl::btree_set<int64_t>, int64_t>)NUMS;
#endif
#ifdef TEST_ABSL_BTREE_SET_DOUBLE
BENCHMARK(BM_pred<absl::btree_set<double>, double>)NUMS;
BENCHMARK(BM_sum<absl::btree_set<double>, double>)NUMS;
BENCHMARK(BM_access<absl::btree_set<double>, double>)NUMS;
BENCHMARK(BM_contains<absl::btree_set<double>, double>)NUMS;
BENCHMARK(BM_remove<absl::btree_set<double>, double>)NUMS;
BENCHMARK(BM_insert<absl::btree_set<double>, double>)NUMS;
#endif
#ifdef TEST_ABSL_BTREE_SET_DEC
BENCHMARK(BM_pred<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_sum<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_access<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_contains<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_remove<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_insert<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
#endif

#ifdef TEST_BT_DYNAMIC_SET_INT
BENCHMARK(BM_pred<bt::dynamic_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_sum<bt::dynamic_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_access<bt::dynamic_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_contains<bt::dynamic_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_remove<bt::dynamic_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_insert<bt::dynamic_set<int64_t>, int64_t>)NUMS;
#endif
#ifdef TEST_BT_DYNAMIC_SET_DOUBLE
BENCHMARK(BM_pred<bt::dynamic_set<double>, double>)NUMS;
BENCHMARK(BM_sum<bt::dynamic_set<double>, double>)NUMS;
BENCHMARK(BM_access<bt::dynamic_set<double>, double>)NUMS;
BENCHMARK(BM_contains<bt::dynamic_set<double>, double>)NUMS;
BENCHMARK(BM_remove<bt::dynamic_set<double>, double>)NUMS;
BENCHMARK(BM_insert<bt::dynamic_set<double>, double>)NUMS;
#endif
#ifdef TEST_BT_DYNAMIC_SET_DEC
BENCHMARK(BM_pred<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_sum<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_access<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_contains<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_remove<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_insert<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
#endif

#ifdef TEST_BT_STATIC_SET_INT
BENCHMARK(BM_pred<bt::static_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_sum<bt::static_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_access<bt::static_set<int64_t>, int64_t>)NUMS;
BENCHMARK(BM_contains<bt::static_set<int64_t>, int64_t>)NUMS;
#endif
#ifdef TEST_BT_STATIC_SET_DOUBLE
BENCHMARK(BM_pred<bt::static_set<double>, double>)NUMS;
BENCHMARK(BM_sum<bt::static_set<double>, double>)NUMS;
BENCHMARK(BM_access<bt::static_set<double>, double>)NUMS;
BENCHMARK(BM_contains<bt::static_set<double>, double>)NUMS;
#endif
#ifdef TEST_BT_STATIC_SET_DEC
BENCHMARK(BM_pred<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_sum<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_access<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
BENCHMARK(BM_contains<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>)NUMS;
#endif

BENCHMARK_MAIN();