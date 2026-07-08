#include <stddef.h>

#include <algorithm>
#include <flat_set>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include "absl/container/btree_set.h"
#include "fast_B-trees/include/dynamic_search.hpp"
#include "fast_B-trees/include/static_search.hpp"

namespace pt {

template <class T>
struct Decimal {
  T whole;
  T partial;

  Decimal(int64_t w, int64_t p) : whole(w), partial(p) {}

  Decimal() : whole(0), partial(0) {}

  static Decimal max_val() {
    return {std::numeric_limits<T>::max(), std::numeric_limits<T>::max()};
  }

  bool operator<(const Decimal& rhs) const {
    if (whole == rhs.whole) {
      return partial < rhs.partial;
    }
    return whole < rhs.whole;
  }

  bool operator>(const Decimal& rhs) const { return rhs < *this; }

  bool operator==(const Decimal& rhs) const {
    return whole == rhs.whole && partial == rhs.partial;
  }

  bool operator<=(const Decimal& rhs) const {
    if (whole == rhs.whole) {
      return partial <= rhs.partial;
    }
    return whole < rhs.whole;
  }

  Decimal operator+(const Decimal& rhs) {
    return {whole + rhs.whole, partial + rhs.partial};
  }

  Decimal& operator+=(const Decimal& rhs) {
    whole += rhs.whole;
    partial += rhs.partial;
    return *this;
  }

  bool operator>=(const Decimal& rhs) const {
    if (whole == rhs.whole) {
      return partial >= rhs.partial;
    }
    return whole > rhs.whole;
  }

  Decimal operator-() const { return {-whole, -partial}; }

  operator int64_t() const { return whole; }
};

class random_er {
 public:
  std::mt19937_64 gen;
  std::uniform_int_distribution<int64_t> i_dist;
  std::uniform_real_distribution<double> r_dist;

  random_er() : gen(), i_dist(), r_dist(0, std::numeric_limits<double>::max()) {}

  template <class T>
  T get();
};

template <>
inline int64_t random_er::get<int64_t>() {
  return i_dist(gen);
}

template <>
inline double random_er::get<double>() {
  return r_dist(gen);
}

template <>
inline Decimal<int64_t> random_er::get<Decimal<int64_t>>() {
  return {i_dist(gen), i_dist(gen)};
}

template <class set_t, class T>
struct with;

template <class T>
struct with<std::set<T>, T> {
  static inline std::set<T> build(size_t size, size_t hit_count,
                                  size_t miss_count, std::vector<T>& vec) {
    random_er rand;
    std::set<T> set;
    while (set.size() < size) {
      T val = -rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      set.insert(val);
      if (vec.size() < hit_count) {
        vec.push_back(val);
      }
    }
    size_t target = hit_count + miss_count;
    while (vec.size() < target) {
      T val = -rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      vec.push_back(val);
    }
    std::shuffle(vec.begin(), vec.end(), rand.gen);
    return set;
  }

  static inline auto pred(const std::set<T>& set, const T& q) {
    return set.lower_bound(q);
  }

  static inline bool contains(const std::set<T>& set, const T& q) {
    return set.contains(q);
  }

  static inline bool insert(std::set<T>& set, const T& q) {
    return set.insert(q).second;
  }

  static inline bool remove(std::set<T>& set, const T& q) {
    return set.erase(q);
  }

  static inline void print_ds() { std::cout << "Type: std::set\n"; }
};

template <class T>
struct with<bt::dynamic_set<T>, T> {
  static inline bt::dynamic_set<T> build(size_t size, size_t hit_count,
                                         size_t miss_count,
                                         std::vector<T>& vec) {
    random_er rand;
    bt::dynamic_set<T> set;
    while (set.size() < size) {
      T val = rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      set.insert(val);
      if (vec.size() < hit_count) {
        vec.push_back(val);
      }
    }
    size_t target = hit_count + miss_count;
    while (vec.size() < target) {
      T val = rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      vec.push_back(val);
    }
    std::shuffle(vec.begin(), vec.end(), rand.gen);
    return set;
  }

  static inline auto pred(const bt::dynamic_set<T>& set, const T& q) {
    return set.predecessor(q);
  }

  static inline bool contains(const bt::dynamic_set<T>& set, const T& q) {
    return set.contains(q);
  }

  static inline bool insert(bt::dynamic_set<T>& set, const T& q) {
    return set.insert(q);
  }

  static inline bool remove(bt::dynamic_set<T>& set, const T& q) {
    return set.remove(q);
  }

  static inline void print_ds() { std::cout << "Type: bt::dynamic_set\n"; }
};

template <class T>
struct with<absl::btree_set<T>, T> {
  static inline absl::btree_set<T> build(size_t size, size_t hit_count,
                                         size_t miss_count,
                                         std::vector<T>& vec) {
    random_er rand;
    absl::btree_set<T> set;
    while (set.size() < size) {
      T val = -rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      set.insert(val);
      if (vec.size() < hit_count) {
        vec.push_back(val);
      }
    }
    size_t target = hit_count + miss_count;
    while (vec.size() < target) {
      T val = -rand.get<T>();
      if (set.contains(val)) {
        continue;
      }
      vec.push_back(val);
    }
    std::shuffle(vec.begin(), vec.end(), rand.gen);
    return set;
  }

  static inline auto pred(const absl::btree_set<T>& set, const T& q) {
    return set.lower_bound(q);
  }

  static inline bool contains(const absl::btree_set<T>& set, const T& q) {
    return set.contains(q);
  }

  static inline bool insert(absl::btree_set<T>& set, const T& q) {
    return set.insert(q).second;
  }

  static inline bool remove(absl::btree_set<T>& set, const T& q) {
    return set.erase(q);
  }

  static inline void print_ds() { std::cout << "Type: absl::btree_set\n"; }
};

template <class T>
struct with<std::flat_set<T>, T> {
  static inline std::flat_set<T> build(size_t size, size_t hit_count,
                                       size_t miss_count, std::vector<T>& vec) {
    random_er rand;
    std::flat_set<T> set;
    std::unordered_set<T> o_set;
    while (o_set.size() < size) {
      T val = -rand.get<T>();
      if (o_set.contains(val)) {
        continue;
      }
      o_set.insert(val);
      if (vec.size() < hit_count) {
        vec.push_back(val);
      }
    }
    size_t target = hit_count + miss_count;
    while (vec.size() < target) {
      T val = -rand.get<T>();
      if (o_set.contains(val)) {
        continue;
      }
      vec.push_back(val);
    }
    std::shuffle(vec.begin(), vec.end(), rand.gen);
    set.insert(o_set.begin(), o_set.end());
    return set;
  }

  static inline auto pred(const std::flat_set<T>& set, const T& q) {
    return set.lower_bound(q);
  }

  static inline bool contains(const std::flat_set<T>& set, const T& q) {
    return set.contains(q);
  }

  static inline bool insert(std::flat_set<T>& set, const T& q) {
    return set.insert(q).second;
  }

  static inline bool remove(std::flat_set<T>& set, const T& q) {
    return set.erase(q);
  }

  static inline void print_ds() { std::cout << "Type: std::flat_set\n"; }
};

template <class T>
struct with<bt::static_set<T>, T> {
  static inline bt::static_set<T> build(size_t size, size_t hit_count,
                                        size_t miss_count,
                                        std::vector<T>& vec) {
    random_er rand;
    std::unordered_set<T> o_set;
    while (o_set.size() < size) {
      T val = rand.get<T>();
      if (o_set.contains(val)) {
        continue;
      }
      o_set.insert(val);
      if (vec.size() < hit_count) {
        vec.push_back(val);
      }
    }
    size_t target = hit_count + miss_count;
    while (vec.size() < target) {
      T val = rand.get<T>();
      if (o_set.contains(val)) {
        continue;
      }
      vec.push_back(val);
    }
    std::shuffle(vec.begin(), vec.end(), rand.gen);
    std::vector<T> s_vec(o_set.begin(), o_set.end());
    bt::static_set<T> set(s_vec);
    return set;
  }

  static inline auto pred(const bt::static_set<T>& set, const T& q) {
    return set.predecessor(q);
  }

  static inline bool contains(const bt::static_set<T>& set, const T& q) {
    return set.contains(q);
  }

  static inline void print_ds() { std::cout << "Type: bt::static_set\n"; }
};

template <class T>
void print_dt();

template <>
void print_dt<int64_t>() {
  std::cout << "Data type: int64_t\n";
}

template <>
void print_dt<double>() {
  std::cout << "Data type: double\n";
}

template <>
void print_dt<Decimal<int64_t>>() {
  std::cout << "Data type: Decimal<int64_t>\n";
}
}  // namespace pt

template <>
struct std::hash<pt::Decimal<int64_t>> {
  size_t operator()(const pt::Decimal<int64_t>& dec) const {
    return std::hash<int64_t>{}(dec.whole ^ dec.partial);
  }
};
