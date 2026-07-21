#include <stddef.h>

#include <algorithm>
#include <flat_set>
#include <iostream>
#include <random>
#include <set>
#include <type_traits>
#include <unordered_set>
#include <vector>

#include "absl/container/btree_set.h"
#include "counters/counters.hpp"
#include "fast_B-trees/include/dynamic_search.hpp"
#include "fast_B-trees/include/static_search.hpp"
#include "op_templates.hpp"

namespace pt {
template <class set_t, class T, class counter_t>
void pred(counter_t& counter, uint64_t n) {
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n, queries / 2, queries / 2, q_vec);
  print_dt<T>();
  auto e = set.end();
  int64_t checksum = 0;
  counter.clear();
  for (auto v : q_vec) {
    checksum += with<set_t, T>::pred(set, v) != e;
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: predecessor" << std::endl;
  counter.output_counters(0, queries);
  std::cout << "Checksum: " << checksum << std::endl;
}

template <class set_t, class T, class counter_t>
void contains(counter_t& counter, uint64_t n) {
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n, queries / 2, queries / 2, q_vec);
  print_dt<T>();

  int64_t checksum = 0;
  counter.clear();
  for (auto v : q_vec) {
    checksum += with<set_t, T>::contains(set, v);
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: contains" << std::endl;
  counter.output_counters(0, queries);
  std::cout << "Checksum: " << checksum << std::endl;
}

template <class set_t, class T, class counter_t>
void access(counter_t& counter, uint64_t n) {
  const constexpr size_t queries = q_count;
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n, queries, 0, q_vec);
  print_dt<T>();
  T checksum{};
  counter.clear();
  for (auto v : q_vec) {
    checksum += *with<set_t, T>::pred(set, v);
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: access" << std::endl;
  counter.output_counters(0, queries);
  std::cout << "Checksum: " << checksum << std::endl;
}

template <class set_t, class T, class counter_t>
void sum(counter_t& counter, uint64_t n) {
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n, 0, 0, q_vec);
  print_dt<T>();
  T checksum{};
  counter.clear();
  for (auto v : set) {
    checksum += v;
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: sum" << std::endl;
  counter.output_counters(0, n);
  std::cout << "Checksum: " << checksum << std::endl;
}

template <class set_t, class T, class counter_t>
void insert(counter_t& counter, uint64_t n) {
  const size_t queries = std::is_same<set_t, std::flat_set<T>>::value
                             ? 1000
                             : std::min(q_count, n);
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n - queries / 2, 0, queries, q_vec);
  print_dt<T>();
  counter.clear();
  int64_t checksum = 0;
  for (auto v : q_vec) {
    checksum += with<set_t, T>::insert(set, v);
    ;
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: insert" << std::endl;
  counter.output_counters(0, queries);
  std::cout << "Checksum: " << checksum << std::endl;
}

template <class set_t, class T, class counter_t>
void remove(counter_t& counter, uint64_t n) {
  const size_t queries = std::is_same<set_t, std::flat_set<T>>::value
                             ? 1000
                             : std::min(q_count, n);
  std::vector<T> q_vec;
  with<set_t, T>::print_ds();
  set_t set = with<set_t, T>::build(n + queries / 2, queries, 0, q_vec);
  print_dt<T>();
  int64_t checksum = 0;
  counter.clear();
  for (auto v : q_vec) {
    checksum += with<set_t, T>::remove(set, v);
  }
  counter.accumulate();
  std::cout << "Size: " << n << "\n"
            << "Operation: remove" << std::endl;
  counter.output_counters(0, queries);
  std::cout << "Checksum: " << checksum << std::endl;
}

}  // namespace pt

int main() {
  count::Counters<false, 1, count::Counter::instructions,
                  count::Counter::branches, count::Counter::branch_miss,
                  count::Counter::L1D_miss, count::Counter::IPC>
      counter;
  std::array<uint64_t, 6> sizes = {1000,    10000,    100000,
                                   1000000, 10000000, 100000000};
  for (auto n : sizes) {
#ifdef TEST_STD_SET_INT
    pt::pred<std::set<int64_t>, int64_t>(counter, n);
    pt::contains<std::set<int64_t>, int64_t>(counter, n);
    pt::access<std::set<int64_t>, int64_t>(counter, n);
    pt::sum<std::set<int64_t>, int64_t>(counter, n);
    pt::insert<std::set<int64_t>, int64_t>(counter, n);
    pt::remove<std::set<int64_t>, int64_t>(counter, n);
#endif
#ifdef TEST_STD_SET_DOUBLE
    pt::pred<std::set<double>, double>(counter, n);
    pt::contains<std::set<double>, double>(counter, n);
    pt::access<std::set<double>, double>(counter, n);
    pt::sum<std::set<double>, double>(counter, n);
    pt::insert<std::set<double>, double>(counter, n);
    pt::remove<std::set<double>, double>(counter, n);
#endif
#ifdef TEST_STD_SET_DEC
    pt::pred<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter, n);
    pt::contains<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                       n);
    pt::access<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                     n);
    pt::sum<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter, n);
    pt::insert<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                     n);
    pt::remove<std::set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                     n);
#endif

#ifdef TEST_BT_DYNAMIC_SET_INT
    pt::pred<bt::dynamic_set<int64_t>, int64_t>(counter, n);
    pt::contains<bt::dynamic_set<int64_t>, int64_t>(counter, n);
    pt::access<bt::dynamic_set<int64_t>, int64_t>(counter, n);
    pt::sum<bt::dynamic_set<int64_t>, int64_t>(counter, n);
    pt::insert<bt::dynamic_set<int64_t>, int64_t>(counter, n);
    pt::remove<bt::dynamic_set<int64_t>, int64_t>(counter, n);
#endif
#ifdef TEST_BT_DYNAMIC_SET_DOUBLE
    pt::pred<bt::dynamic_set<double>, double>(counter, n);
    pt::contains<bt::dynamic_set<double>, double>(counter, n);
    pt::access<bt::dynamic_set<double>, double>(counter, n);
    pt::sum<bt::dynamic_set<double>, double>(counter, n);
    pt::insert<bt::dynamic_set<double>, double>(counter, n);
    pt::remove<bt::dynamic_set<double>, double>(counter, n);
#endif
#ifdef TEST_BT_DYNAMIC_SET_DEC
    pt::pred<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::contains<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::access<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::sum<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::insert<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::remove<bt::dynamic_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
#endif

#ifdef TEST_ABSL_BTREE_SET_INT
    pt::pred<absl::btree_set<int64_t>, int64_t>(counter, n);
    pt::contains<absl::btree_set<int64_t>, int64_t>(counter, n);
    pt::access<absl::btree_set<int64_t>, int64_t>(counter, n);
    pt::sum<absl::btree_set<int64_t>, int64_t>(counter, n);
    pt::insert<absl::btree_set<int64_t>, int64_t>(counter, n);
    pt::remove<absl::btree_set<int64_t>, int64_t>(counter, n);
#endif
#ifdef TEST_ABSL_BTREE_SET_DOUBLE
    pt::pred<absl::btree_set<double>, double>(counter, n);
    pt::contains<absl::btree_set<double>, double>(counter, n);
    pt::access<absl::btree_set<double>, double>(counter, n);
    pt::sum<absl::btree_set<double>, double>(counter, n);
    pt::insert<absl::btree_set<double>, double>(counter, n);
    pt::remove<absl::btree_set<double>, double>(counter, n);
#endif
#ifdef TEST_ABSL_BTREE_SET_DEC
    pt::pred<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::contains<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::access<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::sum<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::insert<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::remove<absl::btree_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
#endif

#ifdef TEST_STD_FLAT_SET_INT
    pt::pred<std::flat_set<int64_t>, int64_t>(counter, n);
    pt::contains<std::flat_set<int64_t>, int64_t>(counter, n);
    pt::access<std::flat_set<int64_t>, int64_t>(counter, n);
    pt::sum<std::flat_set<int64_t>, int64_t>(counter, n);
    pt::insert<std::flat_set<int64_t>, int64_t>(counter, n);
    pt::remove<std::flat_set<int64_t>, int64_t>(counter, n);
#endif
#ifdef TEST_STD_FLAT_SET_DOUBLE
    pt::pred<std::flat_set<double>, double>(counter, n);
    pt::contains<std::flat_set<double>, double>(counter, n);
    pt::access<std::flat_set<double>, double>(counter, n);
    pt::sum<std::flat_set<double>, double>(counter, n);
    pt::insert<std::flat_set<double>, double>(counter, n);
    pt::remove<std::flat_set<double>, double>(counter, n);
#endif
#ifdef TEST_STD_FLAT_SET_DEC
    pt::pred<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                        n);
    pt::contains<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::access<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::sum<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                       n);
    pt::insert<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::remove<std::flat_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
#endif

#ifdef TEST_BT_STATIC_SET_INT
    pt::pred<bt::static_set<int64_t>, int64_t>(counter, n);
    pt::contains<bt::static_set<int64_t>, int64_t>(counter, n);
    pt::access<bt::static_set<int64_t>, int64_t>(counter, n);
    pt::sum<bt::static_set<int64_t>, int64_t>(counter, n);
#endif
#ifdef TEST_BT_STATIC_SET_DOUBLE
    pt::pred<bt::static_set<double>, double>(counter, n);
    pt::contains<bt::static_set<double>, double>(counter, n);
    pt::access<bt::static_set<double>, double>(counter, n);
    pt::sum<bt::static_set<double>, double>(counter, n);
#endif
#ifdef TEST_BT_STATIC_SET_DEC
    pt::pred<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::contains<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::access<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(
        counter, n);
    pt::sum<bt::static_set<pt::Decimal<int64_t>>, pt::Decimal<int64_t>>(counter,
                                                                        n);
#endif
  }
  return 0;
}