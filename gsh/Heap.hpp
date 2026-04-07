#pragma once
#include "Functional.hpp"
#include "TypeDef.hpp"
#include "Util.hpp"
#include "Vec.hpp"
#include "internal/UtilMacro.hpp"
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
namespace gsh {
template<class T, class Comp = Less, class Alloc = std::allocator<T>> class Heap {
  Vec<T, Alloc> data;
  [[no_unique_address]] Comp comp_func;
public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = u32;
  using difference_type = i32;
  using compare_type = Comp;
  using allocator_type = Alloc;
  constexpr Heap() noexcept {}
  constexpr explicit Heap(const Comp& comp, const Alloc& alloc = Alloc()) : data(alloc), comp_func(comp) {}
  constexpr explicit Heap(const Alloc& alloc) : data(alloc) {}
  template<class InputIterator> constexpr Heap(InputIterator first, InputIterator last, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : data(first, last, alloc), comp_func(comp) { make_heap(); }
  template<class InputIterator> Heap(InputIterator first, InputIterator last, const Alloc& alloc) : data(first, last, alloc) { make_heap(); }
  constexpr Heap(const Heap& x) = default;
  constexpr Heap(Heap&& y) noexcept = default;
  constexpr Heap(const Heap& x, const Alloc& alloc) : data(x.data, alloc), comp_func(x.comp_func) {}
  constexpr Heap(Heap&& y, const Alloc& alloc) : data(std::move(y.data), alloc), comp_func(y.comp_func) {}
  constexpr Heap(std::initializer_list<value_type> init, const Comp& comp = Comp(), const Alloc& alloc = Alloc()) : data(init, alloc), comp_func(comp) { make_heap(); }
  constexpr Heap(std::initializer_list<value_type> init, const Alloc& alloc) : data(init, alloc) { make_heap(); }
  constexpr Heap& operator=(const Heap&) = default;
  constexpr Heap& operator=(Heap&&) noexcept(std::is_nothrow_move_assignable_v<Comp>) = default;
private:
  constexpr static bool nothrow_op = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T> && std::is_nothrow_invocable_v<Comp, const T&, const T&>;
  GSH_INTERNAL_INLINE constexpr u32 max_child_idx(const u32 ch, const u32 n) const noexcept(std::is_nothrow_invocable_v<Comp, const T&, const T&>) {
    u32 m = ch;
    switch(n - ch) {
    case 4: {
      const u32 a = ch + static_cast<bool>(std::invoke(comp_func, data[ch], data[ch + 1]));
      const u32 b = ch + 2 + static_cast<bool>(std::invoke(comp_func, data[ch + 2], data[ch + 3]));
      m = a + static_cast<bool>(std::invoke(comp_func, data[a], data[b])) * (b - a);
      break;
    }
    case 3:
      m += static_cast<bool>(std::invoke(comp_func, data[ch], data[ch + 1]));
      m += static_cast<bool>(std::invoke(comp_func, data[m], data[ch + 2])) * (ch + 2 - m);
      break;
    case 2:
      m += static_cast<bool>(std::invoke(comp_func, data[ch], data[ch + 1]));
      break;
    case 1:
      break;
    default: Unreachable();
    }
    return m;
  }
  GSH_INTERNAL_INLINE constexpr void fix_last_internal(const u32 idx, const u32 n) noexcept(nothrow_op) {
    const u32 ch = idx * 4 + 1;
    const u32 m = max_child_idx(ch, n);
    if(std::invoke(comp_func, data[idx], data[m])) {
      T tmp = std::move(data[idx]);
      data[idx] = std::move(data[m]);
      data[m] = std::move(tmp);
    }
  }
  GSH_INTERNAL_INLINE constexpr void push_up(u32 idx) noexcept(nothrow_op) {
    T tmp = std::move(data[idx]);
    u32 cur = idx;
    while(cur != 0) {
      u32 p = (cur - 1) / 4;
      if(!std::invoke(comp_func, data[p], tmp)) break;
      data[cur] = std::move(data[p]);
      cur = p;
    }
    data[cur] = std::move(tmp);
  }
  GSH_INTERNAL_INLINE constexpr void push_down(u32 idx) noexcept(nothrow_op) {
    const u32 n = data.size();
    if(n <= 1) [[unlikely]]
      return;
    u32 cur = idx;
    T tmp = std::move(data[cur]);
    const u32 lim = (n - 2) / 4;
    if(n >= 5) {
      const u32 full_lim = (n - 5) / 4;
      while(cur <= full_lim) {
        const u32 ch = cur * 4 + 1;
        const u32 a = ch + static_cast<bool>(std::invoke(comp_func, data[ch], data[ch + 1]));
        const u32 b = ch + 2 + static_cast<bool>(std::invoke(comp_func, data[ch + 2], data[ch + 3]));
        const u32 m = a + static_cast<bool>(std::invoke(comp_func, data[a], data[b])) * (b - a);
        if(!std::invoke(comp_func, tmp, data[m])) {
          data[cur] = std::move(tmp);
          return;
        }
        data[cur] = std::move(data[m]);
        cur = m;
      }
    }
    if(cur <= lim) {
      const u32 ch = cur * 4 + 1;
      const u32 m = max_child_idx(ch, n);
      if(std::invoke(comp_func, tmp, data[m])) {
        data[cur] = std::move(data[m]);
        cur = m;
      }
    }
    data[cur] = std::move(tmp);
  }
  constexpr void make_heap() noexcept(nothrow_op) {
    const u32 n = data.size();
    if(n <= 1) [[unlikely]]
      return;
    if(n <= 5) [[unlikely]] {
      fix_last_internal(0, n);
      return;
    }
    const u32 lim = (n - 2) / 4;
    const u32 upper_lim = (lim - 1) / 4;
    for(u32 i = lim + 1; i-- > upper_lim + 1;) fix_last_internal(i, n);
    for(u32 i = upper_lim + 1; i--;) push_down(i);
  }
public:
  constexpr void clear() noexcept { data.clear(); }
  template<std::forward_iterator Iter, std::sentinel_for<Iter> Sent> constexpr void assign(Iter first, Sent last) {
    data.assign(std::forward<Iter>(first), std::forward<Sent>(last));
    make_heap();
  }
  template<class F> constexpr void assign(F&& f) {
    data.clear();
    auto push = [&](auto&& x) { data.push_back(std::forward<decltype(x)>(x)); };
    std::invoke(f, push);
    make_heap();
  }
  constexpr const_reference top() const noexcept { return data[0]; }
  [[nodiscard]] constexpr bool empty() const noexcept { return data.empty(); }
  constexpr u32 size() const noexcept { return data.size(); }
  constexpr u32 capacity() const noexcept { return data.capacity(); }
  constexpr void reserve(u32 n) { data.reserve(n); }
  constexpr auto& container() noexcept { return data; }
  constexpr const auto& container() const noexcept { return data; }
  constexpr void push(const T& x) {
    data.push_back(x);
    push_up(data.size() - 1);
  }
  constexpr void push(T&& x) {
    data.push_back(std::move(x));
    push_up(data.size() - 1);
  }
  template<class... Args> constexpr void emplace(Args&&... args) {
    data.emplace_back(std::forward<Args>(args)...);
    push_up(data.size() - 1);
  }
  constexpr void pop() noexcept(nothrow_op) {
    data[0] = std::move(data.back());
    data.pop_back();
    if(!data.empty()) push_down(0);
  }
  constexpr void replace(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
    data[0] = x;
    push_down(0);
  }
  constexpr void replace(T&& x) noexcept(nothrow_op) {
    data[0] = std::move(x);
    push_down(0);
  }
  constexpr void pushpop(const T& x) noexcept(nothrow_op && std::is_nothrow_copy_assignable_v<T>) {
    if(std::invoke(comp_func, x, data[0])) {
      data[0] = x;
      push_down(0);
    }
  }
  constexpr void pushpop(T&& x) noexcept(nothrow_op) {
    if(std::invoke(comp_func, x, data[0])) {
      data[0] = std::move(x);
      push_down(0);
    }
  }
};
}
