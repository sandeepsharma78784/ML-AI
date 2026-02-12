# C++ STL Reference (Concise)

This reference summarizes commonly-used C++ Standard Library (STL) classes, their primary methods, usage notes, and short examples. It is not an exhaustive API listing (the full reference is large), but covers the containers, adaptors, utilities, iterators, and frequently used algorithms you’ll encounter daily.

> File generated: `c:\ml-framework\cppExamples\STL_REFERENCE.md`

## Table of contents

- Containers (sequence)
  - `std::vector`
  - `std::deque`
  - `std::list`, `std::forward_list`
  - `std::array`
  - `std::string`
- Associative containers
  - `std::set`, `std::multiset`
  - `std::map`, `std::multimap`
  - `std::unordered_set`, `std::unordered_map`
- Container adaptors
  - `std::stack`, `std::queue`, `std::priority_queue`
- Utilities
  - `std::pair`, `std::tuple`
  - `std::optional`, `std::variant`, `std::any`
  - `std::bitset`, `std::array`
- Iterators & iterator utilities
- Algorithms (selection)
- Examples: common idioms

---

## Containers (sequence)

### std::vector<T>
Purpose: Dynamic contiguous array. Most-used sequence container.
Key methods:
- Constructors: `vector()`, `vector(size_t)`, `vector(initlist)`
- Element access: `operator[]`, `at`, `front`, `back`, `data`
- Capacity: `size`, `empty`, `reserve`, `capacity`, `shrink_to_fit`
- Modifiers: `push_back`, `pop_back`, `insert`, `erase`, `clear`, `resize`
- Iterators: `begin`, `end`, `rbegin`, `rend`

Use when you need random-access and amortized O(1) push_back.

Example:
```cpp
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<int> v = {1,2,3};
v.push_back(4);
std::sort(v.begin(), v.end());
for (int x : v) std::cout << x << ' ';
// Output: 1 2 3 4
```

### std::deque<T>
Purpose: Double-ended queue, efficient push/pop at both ends.
Key methods: similar to `vector` plus `push_front` / `pop_front`.
Use when you need frequent front inserts/removals.

### std::list<T> and std::forward_list<T>
Purpose: Doubly-linked (`list`) or singly-linked (`forward_list`) lists.
Key operations: splice, insert/erase in O(1) given iterator, `push_front`.
Use when you must frequently insert/erase in middle and have iterators to positions.

### std::array<T,N>
Purpose: Fixed-size array (stack-allocated semantics), supports `std::get` and tuple-like ops.
Key methods: `size`, `fill`, `begin`/`end`, `operator[]`.

### std::string
Purpose: Dynamic contiguous character sequence specialized for text.
Key methods: `size`, `empty`, `operator[]`, `at`, `substr`, `append`, `find`, `replace`, `c_str`.

Example:
```cpp
#include <string>
#include <iostream>
std::string s = "hello";
s += " world";
std::cout << s.substr(0,5); // hello
```

---

## Associative containers

These maintain order (by key) for sets/maps and provide logarithmic lookup.

### std::set<T>, std::multiset<T>
Key methods: `insert`, `erase`, `find`, `count`, `lower_bound`, `upper_bound`, `begin`/`end`.
`set` contains unique keys; `multiset` allows duplicates.

Example:
```cpp
#include <set>
std::set<int> s = {3,1,4};
s.insert(2);
if (s.count(3)) { /* found */ }
```

### std::map<Key, T>, std::multimap<Key, T>
Associative map from keys to values. `operator[]` inserts default value when not present.
Key methods: `operator[]`, `at`, `find`, `insert`, `erase`.

Example:
```cpp
#include <map>
std::map<std::string,int> m;
m["apple"] = 3;
auto it = m.find("apple");
if (it != m.end()) // use it->second
```

### Unordered variants: std::unordered_set, std::unordered_map
Hash-table-based containers with average O(1) lookup. Methods mirror ordered versions (`insert`, `find`, `erase`). Use when ordering by key is not required and performance matters.

---

## Container adaptors

These are thin wrappers that provide restricted interfaces on top of underlying containers.

### std::stack<T>
LIFO adaptor. Key methods: `push`, `pop`, `top`, `empty`, `size`.

### std::queue<T>
FIFO adaptor. Key methods: `push`, `pop`, `front`, `back`.

### std::priority_queue<T>
Max-heap by default. Key methods: `push`, `pop`, `top`, `emplace`, `size`.

Example (priority_queue):
```cpp
#include <queue>
#include <vector>
std::priority_queue<int> pq;
 pq.push(5); pq.push(1); pq.push(3);
 while (!pq.empty()) { std::cout<< pq.top() << ' '; pq.pop(); }
 // 5 3 1
```

---

## Utilities

### std::pair and std::make_pair
Small struct with `first` and `second`. Useful for returning two values or for map entries.

Example:
```cpp
#include <utility>
auto p = std::make_pair(1, "one");
int a = p.first;
```

### std::tuple
Holds fixed-size heterogeneous values. `std::get<idx>` and `std::tie` help unpack.

### std::optional<T>
Represents optional value (present or not). Methods: `has_value()` / `operator bool`, `value()`, `value_or()`.

Example:
```cpp
#include <optional>
std::optional<int> find_first_even(const std::vector<int>& v){
  for(int x:v) if (x%2==0) return x;
  return std::nullopt;
}
```

### std::variant, std::any
`variant` is a type-safe union of set types with `std::get` and `std::visit`.
`any` holds any copyable type (type-erased); use with caution.

### std::bitset<N>
Fixed-size compile-time bitset. Methods: `set`, `reset`, `flip`, `test`, `to_string`.

---

## Iterators & iterator utilities

- `begin(container)`, `end(container)` give iterators; use `cbegin`/`cend` for const.
- Iterator categories: input, output, forward, bidirectional, random-access.
- Use `std::iterator_traits` for generic code.
- Range-based for uses `begin`/`end`.

Example: generic sum
```cpp
template<class Range>
auto sum(const Range &r){
    using T = typename std::decay<decltype(*std::begin(r))>::type;
    T s{};
    for (auto &x : r) s += x;
    return s;
}
```

---

## Algorithms (selection)

Algorithms operate on iterator ranges. They are independent of container type when iterators are compatible.

Common algorithms and what they do:
- `std::sort(begin,end)` — sort random-access ranges (vector/array/deque)
- `std::stable_sort` — stable O(n log n)
- `std::partial_sort`, `std::nth_element`
- `std::binary_search`, `std::lower_bound`, `std::upper_bound` — work on sorted ranges
- `std::find`, `std::find_if` — linear search
- `std::count`, `std::count_if` — count occurrences
- `std::accumulate` — fold (from `<numeric>`)
- `std::transform` — apply function to range and write to output
- `std::copy`, `std::move`, `std::copy_if`
- `std::remove_if` + container `erase` (erase-remove idiom)
- Set algorithms: `std::set_union`, `std::set_intersection` (work on sorted ranges)

Example: erase-remove idiom
```cpp
#include <algorithm>
#include <vector>
std::vector<int> v = {1,2,3,4,5};
v.erase(std::remove_if(v.begin(), v.end(), [](int x){return x%2==0;}), v.end());
// v now {1,3,5}
```

Example: sorting and binary search
```cpp
#include <vector>
#include <algorithm>
std::vector<int> v = {3,1,4,2};
std::sort(v.begin(), v.end());
if (std::binary_search(v.begin(), v.end(), 3)) { /* found */ }
```

---

## Common idioms and notes

- Use `reserve` on `std::vector` if you can estimate size to avoid reallocations.
- Prefer `std::vector` over raw arrays for safety and convenience.
- For unique keys with lookup, prefer `std::unordered_map` for average O(1) performance; use `std::map` if you need ordered traversal or guaranteed logarithmic worst-case.
- Avoid `std::any` unless necessary; prefer `std::variant` or polymorphism.
- Use `const` and `&` in function parameters to avoid copies: `void f(const std::vector<int>& v)`.

---

## Short example: mixing containers and algorithms

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main(){
    std::vector<int> a = {5,2,9,1,5,6};
    // remove duplicates
    std::sort(a.begin(), a.end());
    a.erase(std::unique(a.begin(), a.end()), a.end());
    for (int x : a) std::cout << x << ' ';
}
```

---

## Where to go next / Further expansion
- Add per-class full method lists and complexity guarantees.
- Add more algorithm examples with time/space notes.
- Add C++20 ranges and views (e.g. `std::ranges::sort`, `views::filter`) — large topic.

---

If you'd like, I can:
- expand this into a fully exhaustive per-class method reference (long task)
- add C++17 / C++20 features (ranges, concepts)
- produce runnable example files for each container in the same folder

