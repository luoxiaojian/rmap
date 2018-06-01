template <typename _Key, typename _Tp, typename _Compare = std::less<_Key>>
class rmap {
 public:
  typedef _Key key_type;
  typedef _Tp mapped_type;
  typedef std::pair<const _Key, _Tp> value_type;
  typedef _Compare key_compare;

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
  _Rep_type _M_t;

 public:
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

  rmap() : _M_t() {}

  rmap(const rmap& __x) : _M_t(__x._M_t) {}

  rmap& operator=(const rmap& __x) {
    _M_t = __x._M_t;
    return *this;
  }

  iterator begin() { return _M_t.begin(); }

  const_iterator begin() const { return _M_t.begin(); }

  iterator end() { return _M_t.end(); }

  const_iterator end() const { return _M_t.end(); }

  reverse_iterator rbegin() { return _M_t.rbegin(); }

  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }

  reverse_iterator rend() { return _M_t.rend(); }

  const_reverse_iterator rend() const { return _M_t.rend(); }

  bool empty() const { return _M_t.empty(); }

  size_type size() const { return _M_t.size(); }

  size_type max_size() const { return _M_t.max_size(); }

  mapped_type& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first)) {
      __i = insert(__i, value_type(__k, mapped_type()));
    }
    return (*__i).second;
  }

  std::pair<iterator, bool> insert(const value_type& __x) {
    return _M_t._M_insert_unique(__x);
  }

  /*
  iterator insert(iterator __position, const value_type& __x) {
    return _M_t._M_insert_unique(__position, __x);
  }

  template <typename _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_t._M_insert_unique(__first, __last);
  }
  */

  // void erase(iterator __position) { _M_t.erase(__position); }

  size_type erase(const key_type& __x) { return _M_t.erase(__x); }

  // void erase(iterator __first, iterator __last) { _M_t.erase(__first,
  // __last); }

  void swap(rmap& __x) { _M_t.swap(__x._M_t); }

  void clear() { _M_t.clear(); }

  key_compare key_comp() const { return _M_t.key_comp(); }

  value_compare value_comp() const { return value_compare(_M_t.key_comp()); }

  iterator find(const key_type& __x) { return _M_t.find(__x); }

  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }

  iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }

  /*
  const_iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x);
  }
  */

  /*
  iterator upper_bound(const key_type& __x) { return _M_t.upper_bound(__x); }

  const_iterator upper_bound(const key_type& __x) const {
    return _M_t.upper_bound(__x);
  }
  */

  /*
  std::pair<iterator, iterator> equal_range(const key_type& __x) {
    return _M_t.equal_range(__x);
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& __x) const {
    return _M_t.equal_range(__x);
  }
  */

  template <typename _K1, typename _T1, typename _C1>
  friend bool operator==(const rmap<_K1, _T1, _C1>&,
                         const rmap<_K1, _T1, _C1>&);

  template <typename _K1, typename _T1, typename _C1>
  friend bool operator<(const rmap<_K1, _T1, _C1>&, const rmap<_K1, _T1, _C1>&);
};

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator==(const rmap<_Key, _Tp, _Compare>& __x,
                       const rmap<_Key, _Tp, _Compare>& __y) {
  return __x._M_t == __y._M_t;
}

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator<(const rmap<_Key, _Tp, _Compare>& __x,
                      const rmap<_Key, _Tp, _Compare>& __y) {
  return __x._M_t < __y._M_t;
}

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator!=(const rmap<_Key, _Tp, _Compare>& __x,
                       const rmap<_Key, _Tp, _Compare>& __y) {
  return !(__x == __y);
}

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator>(const rmap<_Key, _Tp, _Compare>& __x,
                      const rmap<_Key, _Tp, _Compare>& __y) {
  return __y < __x;
}

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator<=(const rmap<_Key, _Tp, _Compare>& __x,
                       const rmap<_Key, _Tp, _Compare>& __y) {
  return !(__y < __x);
}

template <typename _Key, typename _Tp, typename _Compare>
inline bool operator>=(const rmap<_Key, _Tp, _Compare>& __x,
                       const rmap<_Key, _Tp, _Compare>& __y) {
  return !(__x < __y);
}

template <typename _Key, typename _Tp, typename _Compare>
inline void swap(rmap<_Key, _Tp, _Compare>& __x,
                 rmap<_Key, _Tp, _Compare>& __y) {
  __x.swap(__y);
}
