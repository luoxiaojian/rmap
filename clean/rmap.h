#ifndef RMAP_H_
#define RMAP_H_

#include "allocator.h"
#include "rb_tree.h"

template <typename _Pair>
struct S1st {
  typename _Pair::first_type& operator()(_Pair& __x) const { return __x.first; }
  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

template <typename _Key, typename _Tp, typename _Compare = std::less<_Key> >
class rmap {
 public:
  typedef _Key key_type;
  typedef _Tp mapped_type;
  typedef std::pair<const _Key, _Tp> value_type;
  typedef _Compare key_compare;

 public:
  class value_compare
      : public std::binary_function<value_type, value_type, bool> {
    friend class rmap<_Key, _Tp, _Compare>;

   protected:
    _Compare comp;

    value_compare(_Compare __c) : comp(__c) {}

   public:
    bool operator()(const value_type& __x, const value_type& __y) const {
      return comp(__x.first, __y.first);
    }
  };

 private:
  typedef _Rb_tree<key_type, value_type, S1st<value_type>, key_compare>
      _Rep_type;
  typedef Allocator<_Rb_tree_node<value_type> > _Alloc_type;

  _Rep_type _M_t;
  _Alloc_type _M_alloc;

 public:
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;

  rmap() : _M_t(_M_alloc) {}

  rmap(const rmap& __x) : _M_t(__x._M_t) {}

  rmap& operator=(const rmap& __x) {
    _M_t = __x._M_t;
    return *this;
  }

  iterator begin() { return _M_t.begin(); }

  const_iterator begin() const { return _M_t.begin(); }

  iterator end() { return _M_t.end(); }

  const_iterator end() const { return _M_t.end(); }

  bool empty() const { return _M_t.empty(); }

  size_type size() const { return _M_t.size(); }

  size_type max_size() const { return _M_t.max_size(); }

  mapped_type& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first))
      __i = insert(__i, value_type(__k, mapped_type()));
    return (*__i).second;
  }

  std::pair<iterator, bool> insert(const value_type& __x) {
    return _M_t._M_insert_unique(__x);
  }

  iterator insert(iterator __position, const value_type& __x) {
    return _M_t._M_insert_unique_(__position, __x);
  }

  size_type erase(const key_type& __x) { return _M_t.erase(__x); }

  void swap(rmap& __x) { _M_t.swap(__x._M_t); }

  void clear() { _M_t.clear(); }

  key_compare key_comp() const { return _M_t.key_comp(); }

  value_compare value_comp() const { return value_compare(_M_t.key_comp()); }

  iterator find(const key_type& __x) { return _M_t.find(__x); }

  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }

  iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }

  const_iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x);
  }
};

template <typename _Key, typename _Tp, typename _Compare>
inline void swap(rmap<_Key, _Tp, _Compare>& __x,
                 rmap<_Key, _Tp, _Compare>& __y) {
  __x.swap(__y);
}

#endif /* RMAP_H_ */
