#ifndef RB_TREE_H_
#define RB_TREE_H_

#include <iterator>
#include <limits>
#include <utility>

#include "allocator.h"

// #define USE_OFFSET
#define DULL (std::numeric_limits<ptrdiff_t>::max())

enum _Rb_tree_color { _S_red = false, _S_black = true };

template <typename _Val>
struct _Rb_tree_node {
  typedef _Rb_tree_node<_Val>* _Link_type;
  typedef const _Rb_tree_node<_Val>* _Const_Link_type;

  _Rb_tree_color _M_color;
  ptrdiff_t _M_parent;
  ptrdiff_t _M_left;
  ptrdiff_t _M_right;

  _Val _M_value_field;

  static _Link_type _S_minimum(_Link_type __x) {
    while (__x->_M_left != DULL) __x += __x->_M_left;
    return __x;
  }

  static _Const_Link_type _S_minimum(_Const_Link_type __x) {
    while (__x->_M_left != DULL) __x += __x->_M_left;
    return __x;
  }

  static _Link_type _S_maximum(_Link_type __x) {
    while (__x->_M_right != DULL) __x += __x->_M_right;
    return __x;
  }

  static _Const_Link_type _S_maximum(_Const_Link_type __x) {
    while (__x->_M_right != DULL) __x += __x->_M_right;
    return __x;
  }

  _Val* _M_valptr() {
    return &_M_value_field;
    // return std::__addressof(_M_value_field);
  }

  const _Val* _M_valptr() const {
    return &_M_value_field;
    // return std::__addressof(_M_value_field);
  }

  _Link_type parent() { return _M_parent == DULL ? NULL : this + _M_parent; }

  _Const_Link_type parent() const {
    return _M_parent == DULL ? NULL : this + _M_parent;
  }

  _Link_type left() { return _M_left == DULL ? NULL : this + _M_left; }

  _Const_Link_type left() const {
    return _M_left == DULL ? NULL : this + _M_left;
  }

  _Link_type right() { return _M_right == DULL ? NULL : this + _M_right; }

  _Const_Link_type right() const {
    return _M_right == DULL ? NULL : this + _M_right;
  }

  void SetLeft(_Const_Link_type ptr) { _M_left = ptr ? ptr - this : DULL; }

  void SetRight(_Const_Link_type ptr) { _M_right = ptr ? ptr - this : DULL; }

  void SetParent(_Const_Link_type ptr) { _M_parent = ptr ? ptr - this : DULL; }
};

template <typename _Val>
void _Rb_tree_insert_and_rebalance(const bool __insert_left,
                                   _Rb_tree_node<_Val>* __x,
                                   _Rb_tree_node<_Val>* __p,
                                   _Rb_tree_node<_Val>& __header) throw();

template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_rebalance_for_erase(
    _Rb_tree_node<_Val>* const __z, _Rb_tree_node<_Val>& __header) throw();

template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_increment(_Rb_tree_node<_Val>* __x) throw() {
  if (__x->_M_right != DULL) {
    __x = __x->right();
    while (__x->_M_left != DULL) __x = __x->left();
  } else {
    _Rb_tree_node<_Val>* __y = __x->parent();
    while (__x == __y->right()) {
      __x = __y;
      __y = __y->parent();
    }
    if (__x->right() != __y) __x = __y;
  }
  return __x;
}

template <typename _Val>
const _Rb_tree_node<_Val>* _Rb_tree_increment(
    const _Rb_tree_node<_Val>* __cx) throw() {
  _Rb_tree_node<_Val>* __x = const_cast<_Rb_tree_node<_Val>*>(__cx);
  if (__x->_M_right != DULL) {
    __x = __x->right();
    while (__x->_M_left != DULL) __x = __x->left();
  } else {
    _Rb_tree_node<_Val>* __y = __x->parent();
    while (__x == __y->right()) {
      __x = __y;
      __y = __y->parent();
    }
    if (__x->right() != __y) __x = __y;
  }
  return __x;
}

template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_decrement(_Rb_tree_node<_Val>* __x) throw() {
  if (__x->_M_color == _S_red && __x->parent()->parent() == __x) {
    __x = __x->right();
  } else if (__x->_M_left != DULL) {
    _Rb_tree_node<_Val>* __y = __x->left();
    while (__y->_M_right != DULL) __y = __y->right();
    __x = __y;
  } else {
    _Rb_tree_node<_Val>* __y = __x->parent();
    while (__x == __y->left()) {
      __x = __y;
      __y = __y->parent();
    }
    __x = __y;
  }
  return __x;
}

template <typename _Val>
const _Rb_tree_node<_Val>* _Rb_tree_decrement(
    const _Rb_tree_node<_Val>* __cx) throw() {
  _Rb_tree_node<_Val>* __x = const_cast<_Rb_tree_node<_Val>*>(__cx);
  if (__x->_M_color == _S_red && __x->parent()->parent() == __x) {
    __x = __x->right();
  } else if (__x->_M_left != DULL) {
    _Rb_tree_node<_Val>* __y = __x->left();
    while (__y->_M_right != DULL) __y = __y->right();
    __x = __y;
  } else {
    _Rb_tree_node<_Val>* __y = __x->parent();
    while (__x == __y->left()) {
      __x = __y;
      __y = __y->parent();
    }
    __x = __y;
  }
  return __x;
}

template <typename _Tp>
struct _Rb_tree_iterator {
  typedef _Tp value_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_iterator<_Tp> _Self;
  typedef _Rb_tree_node<_Tp>* _Link_type;

  _Rb_tree_iterator() : _M_node() {}

  explicit _Rb_tree_iterator(_Link_type __x) : _M_node(__x) {}

  reference operator*() const { return _M_node->_M_value_field; }

  pointer operator->() const { return _M_node->_M_valptr(); }

  _Self& operator++() {
    _M_node = _Rb_tree_increment<_Tp>(_M_node);
    return *this;
  }

  _Self operator++(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment<_Tp>(_M_node);
    return __tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement<_Tp>(_M_node);
    return *this;
  }

  _Self operator--(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement<_Tp>(_M_node);
    return __tmp;
  }

  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }

  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }

  _Link_type _M_node;
};

template <typename _Tp>
struct _Rb_tree_const_iterator {
  typedef _Tp value_type;
  typedef const _Tp& reference;
  typedef const _Tp* pointer;

  typedef _Rb_tree_iterator<_Tp> iterator;

  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_const_iterator<_Tp> _Self;
  typedef const _Rb_tree_node<_Tp>* _Link_type;

  _Rb_tree_const_iterator() : _M_node() {}

  explicit _Rb_tree_const_iterator(_Link_type __x) : _M_node(__x) {}

  _Rb_tree_const_iterator(const iterator& __it) : _M_node(__it._M_node) {}

  iterator _M_const_cast() const {
    return iterator(static_cast<typename iterator::_Link_type>(
        const_cast<typename iterator::_Link_type>(_M_node)));
  }

  reference operator*() const { return _M_node->_M_value_field; }

  pointer operator->() const { return _M_node->_M_valptr(); }

  _Self& operator++() {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self operator++(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self operator--(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return __tmp;
  }

  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }

  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }

  _Link_type _M_node;
};

template <typename _Val>
inline bool operator==(const _Rb_tree_iterator<_Val>& __x,
                       const _Rb_tree_const_iterator<_Val>& __y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val>
inline bool operator!=(const _Rb_tree_iterator<_Val>& __x,
                       const _Rb_tree_const_iterator<_Val>& __y) {
  return __x._M_node != __y._M_node;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
class _Rb_tree {
 public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef _Rb_tree_node<_Val>* _Link_type;
  typedef const _Rb_tree_node<_Val>* _Const_Link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef _Rb_tree_impl<_Rb_tree_node<_Val> > allocator_type;

 private:
  _Rb_tree_impl<_Rb_tree_node<_Val> >& impl_;
  _Compare _M_key_compare;

 protected:
  _Link_type _M_get_node() { return impl_.GetNode(); }

  _Link_type _M_create_node(const value_type& __x) {
    _Link_type __tmp = _M_get_node();
    new (__tmp->_M_valptr()) value_type(__x);
    return __tmp;
  }

  _Link_type _M_clone_node(_Const_Link_type __x) {
    _Link_type __tmp = _M_create_node(__x->_M_value_field);
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = DULL;
    __tmp->_M_right = DULL;
    return __tmp;
  }

 protected:
  _Link_type _M_root() { return this->impl_[0].parent(); }

  _Const_Link_type _M_root() const { return this->impl_[0].parent(); }

  _Link_type _M_leftmost() { return this->impl_[0].left(); }

  _Const_Link_type _M_leftmost() const { return this->impl_[0].left(); }

  _Link_type _M_rightmost() { return this->impl_[0].right(); }

  _Const_Link_type _M_rightmost() const { return this->impl_[0].right(); }

  _Link_type _M_begin() { return this->impl_[0].parent(); }

  _Const_Link_type _M_begin() const { return this->impl_[0].parent(); }

  _Link_type _M_end() { return &(this->impl_[0]); }

  _Const_Link_type _M_end() const { return &(this->impl_[0]); }

  static const_reference _S_value(_Const_Link_type __x) {
    return __x->_M_value_field;
  }

  static const _Key& _S_key(_Const_Link_type __x) {
    return _KeyOfValue()(_S_value(__x));
  }

  static _Link_type _S_left(_Link_type __x) { return __x->left(); }

  static _Const_Link_type _S_left(_Const_Link_type __x) { return __x->left(); }

  static _Link_type _S_right(_Link_type __x) { return __x->right(); }

  static _Const_Link_type _S_right(_Const_Link_type __x) {
    return __x->right();
  }

  static _Link_type _S_minimum(_Link_type __x) {
    return _Rb_tree_node<_Val>::_S_minimum(__x);
  }

  static _Const_Link_type _S_minimum(_Const_Link_type __x) {
    return _Rb_tree_node<_Val>::_S_minimum(__x);
  }

  static _Link_type _S_maximum(_Link_type __x) {
    return _Rb_tree_node<_Val>::_S_maximum(__x);
  }

  static _Const_Link_type _S_maximum(_Const_Link_type __x) {
    return _Rb_tree_node<_Val>::_S_maximum(__x);
  }

 public:
  typedef _Rb_tree_iterator<value_type> iterator;
  typedef _Rb_tree_const_iterator<value_type> const_iterator;

 public:
  // Insert/erase.
  std::pair<iterator, bool> _M_insert_unique(const value_type& __x);

  iterator _M_insert_unique_(const_iterator __position, const value_type& __x);

 private:
#ifdef USE_OFFSET
  std::pair<ptrdiff_t, ptrdiff_t> _M_get_insert_unique_pos_offset(
      const key_type& __k);
  std::pair<ptrdiff_t, ptrdiff_t> _M_get_insert_hint_unique_pos_offset(
      const_iterator __pos, const key_type& __k);
  iterator _M_insert_offset_(ptrdiff_t __x, ptrdiff_t __y,
                             const value_type& __v);
#else
  std::pair<_Link_type, _Link_type> _M_get_insert_unique_pos(
      const key_type& __k);
  std::pair<_Link_type, _Link_type> _M_get_insert_hint_unique_pos(
      const_iterator __pos, const key_type& __k);
  iterator _M_insert_(_Link_type __x, _Link_type __y, const value_type& __v);
#endif

  _Link_type _M_copy(_Const_Link_type __x, _Link_type __p);

  iterator _M_lower_bound(_Link_type __x, _Link_type __y, const _Key& __k);

  const_iterator _M_lower_bound(_Const_Link_type __x, _Const_Link_type __y,
                                const _Key& __k) const;

  iterator _M_upper_bound(_Link_type __x, _Link_type __y, const _Key& __k);

  const_iterator _M_upper_bound(_Const_Link_type __x, _Const_Link_type __y,
                                const _Key& __k) const;

 public:
  _Rb_tree(allocator_type& impl) : impl_(impl) {
    _Link_type ptr = impl_.GetNode();
    ptr->_M_color = _S_red;
    ptr->SetParent(NULL);
    ptr->SetLeft(ptr);
    ptr->SetRight(ptr);
  }

  ~_Rb_tree() {}

  _Rb_tree& operator=(const _Rb_tree& __x);

  // Accessors.
  _Compare key_comp() const { return _M_key_compare; }

  iterator begin() { return iterator(this->impl_[0].left()); }

  const_iterator begin() const { return const_iterator(this->impl_[0].left()); }

  iterator end() { return iterator(&this->impl_[0]); }

  const_iterator end() const { return const_iterator(&this->impl_[0]); }

  bool empty() const { return impl_.Size() <= 1; }

  size_type size() const { return MAX(impl_.Size() - 1, 0); }

  size_type max_size() const { return impl_.Capacity(); }

  void swap(_Rb_tree& __t);

 public:
  void clear() {
    impl_.Reset();
    _Link_type ptr = impl_.GetNode();
    ptr->_M_color = _S_red;
    ptr->SetParent(NULL);
    ptr->SetLeft(ptr);
    ptr->SetRight(ptr);
  }

  // Set operations.
  iterator find(const key_type& __k);

  const_iterator find(const key_type& __k) const;

  size_type count(const key_type& __k) const;

  iterator lower_bound(const key_type& __k) {
    return _M_lower_bound(_M_begin(), _M_end(), __k);
  }

  const_iterator lower_bound(const key_type& __k) const {
    return _M_lower_bound(_M_begin(), _M_end(), __k);
  }

  iterator upper_bound(const key_type& __k) {
    return _M_upper_bound(_M_begin(), _M_end(), __k);
  }

  const_iterator upper_bound(const key_type& __k) const {
    return _M_upper_bound(_M_begin(), _M_end(), __k);
  }

  std::pair<iterator, iterator> equal_range(const key_type& __k);

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& __k) const;

  // Debugging.
  // bool __rb_verify() const;
};

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
inline void swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>& __x,
                 _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>& __y) {
  __x.swap(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>&
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::operator=(const _Rb_tree& __x) {
  if (this != &__x) {
    // Note that _Key may be a constant type.
    clear();
    _M_key_compare = __x._M_key_compare;
    this->impl_ = __x.impl_;
  }
  return *this;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_lower_bound(_Link_type __x,
                                                            _Link_type __y,
                                                            const _Key& __k) {
  while (__x != NULL) {
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  }
  return iterator(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_lower_bound(
    _Const_Link_type __x, _Const_Link_type __y, const _Key& __k) const {
  while (__x != NULL) {
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  }
  return const_iterator(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_upper_bound(_Link_type __x,
                                                            _Link_type __y,
                                                            const _Key& __k) {
  while (__x != 0)
    if (_M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  return iterator(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_upper_bound(
    _Const_Link_type __x, _Const_Link_type __y, const _Key& __k) const {
  while (__x != 0)
    if (_M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  return const_iterator(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator,
          typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::equal_range(const _Key& __k) {
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  while (__x != 0) {
    if (_M_key_compare(_S_key(__x), __k))
      __x = _S_right(__x);
    else if (_M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else {
      _Link_type __xu(__x), __yu(__y);
      __y = __x, __x = _S_left(__x);
      __xu = _S_right(__xu);
      return std::pair<iterator, iterator>(_M_lower_bound(__x, __y, __k),
                                           _M_upper_bound(__xu, __yu, __k));
    }
  }
  return std::pair<iterator, iterator>(iterator(__y), iterator(__y));
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::const_iterator,
          typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::const_iterator>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::equal_range(
    const _Key& __k) const {
  _Const_Link_type __x = _M_begin();
  _Const_Link_type __y = _M_end();
  while (__x != 0) {
    if (_M_key_compare(_S_key(__x), __k))
      __x = _S_right(__x);
    else if (_M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else {
      _Const_Link_type __xu(__x), __yu(__y);
      __y = __x, __x = _S_left(__x);
      __xu = _S_right(__xu);
      return std::pair<const_iterator, const_iterator>(
          _M_lower_bound(__x, __y, __k), _M_upper_bound(__xu, __yu, __k));
    }
  }
  return std::pair<const_iterator, const_iterator>(const_iterator(__y),
                                                   const_iterator(__y));
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
void _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::swap(
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>& __t) {
  /*
    if (_M_root() == 0) {
      if (__t._M_root() != 0) {
        _M_root() = __t._M_root();
        _M_leftmost() = __t._M_leftmost();
        _M_rightmost() = __t._M_rightmost();
        _M_root()->_M_parent = _M_end();

        __t._M_root() = 0;
        __t._M_leftmost() = __t._M_end();
        __t._M_rightmost() = __t._M_end();
      }
    } else if (__t._M_root() == 0) {
      __t._M_root() = _M_root();
      __t._M_leftmost() = _M_leftmost();
      __t._M_rightmost() = _M_rightmost();
      __t._M_root()->_M_parent = __t._M_end();

      _M_root() = 0;
      _M_leftmost() = _M_end();
      _M_rightmost() = _M_end();
    } else {
      std::swap(_M_root(), __t._M_root());
      std::swap(_M_leftmost(), __t._M_leftmost());
      std::swap(_M_rightmost(), __t._M_rightmost());

      _M_root()->_M_parent = _M_end();
      __t._M_root()->_M_parent = __t._M_end();
    }
    // No need to swap header's color as it does not change.
    std::swap(this->_M_key_compare, __t._M_key_compare);
  */
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator, bool>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_insert_unique(const _Val& __v) {
  typedef std::pair<iterator, bool> _Res;
#ifdef USE_OFFSET
  std::pair<ptrdiff_t, ptrdiff_t> __res =
      _M_get_insert_unique_pos_offset(_KeyOfValue()(__v));
  if (__res.second != -1) {
    return _Res(_M_insert_offset_(__res.first, __res.second, __v), true);
  }

  return _Res(iterator(this->impl_.Offset2Ptr(__res.first)), false);
#else
  std::pair<_Link_type, _Link_type> __res =
      _M_get_insert_unique_pos(_KeyOfValue(__v));
  if (__res.second != NULL) {
    return _Res(_M_insert_(__res.first, __res.second, __v), true);
  }
  return _Res(iterator(__res.first), false);
#endif
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_insert_unique_(
    const_iterator __position, const _Val& __v) {
#ifdef USE_OFFSET
  std::pair<ptrdiff_t, ptrdiff_t> __res =
      _M_get_insert_hint_unique_pos_offset(__position, _KeyOfValue()(__v));
  if (__res.second != -1) {
    return _M_insert_offset(__res.first, __res.second, __v);
  }
  return iterator(impl_.Offset2Ptr(__res.first));
#else
  std::pair<_Link_type, _Link_type> __res =
      _M_get_insert_hint_unique_pos(__position, _KeyOfValue()(__v));
  if (__res.second != NULL) {
    return _M_insert_(__res.first, __res.second, __v);
  }
  return iterator(__res.first);
#endif
}

#ifdef USE_OFFSET
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<ptrdiff_t, ptrdiff_t>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_get_insert_unique_pos_offset(
    const key_type& __k) {
  typedef std::pair<ptrdiff_t, ptrdiff_t> _Res;
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  bool __comp = true;
  while (__x != NULL) {
    __y = __x;
    __comp = _M_key_compare(__k, _S_key(__x));
    __x = __comp ? _S_left(__x) : _S_right(__x);
  }
  iterator __j = iterator(__y);
  if (__comp) {
    if (__j == begin()) {
      return _Res(impl_.Ptr2Offset(__x), impl_.Ptr2Offset(__y));
    } else {
      --__j;
    }
  }
  if (_M_key_compare(_S_key(__j._M_node), __k))
    return _Res(impl_.Ptr2Offset(__x), impl_.Ptr2Offset(__y));
  return _Res(impl_.Ptr2Offset(__j._M_node), -1);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<ptrdiff_t, ptrdiff_t> _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::
    _M_get_insert_hint_unique_pos_offset(const_iterator __position,
                                         const key_type& __k) {
  iterator __pos = __position._M_const_cast();
  typedef std::pair<ptrdiff_t, ptrdiff_t> _Res;
  if (__pos._M_node == _M_end()) {
    if (size() > 0 && _M_key_compare(_S_key(_M_rightmost()), __k)) {
      return _Res(-1, impl_.Ptr2Offset(_M_rightmost()));
    } else {
      return _M_get_insert_unique_pos_offset(__k);
    }
  } else if (_M_key_compare(__k, _S_key(__pos._M_node))) {
    iterator __before = __pos;
    if (__pos._M_node == _M_leftmost()) {
      return _Res(impl_.Ptr2Offset(_M_leftmost()),
                  impl_.Ptr2Offset(_M_leftmost()));
    } else if (_M_key_compare(_S_key((--__before)._M_node), __k)) {
      if (_S_right(__before._M_node) == 0) {
        return _Res(-1, impl_.Ptr2Offset(__before._M_node));
      } else {
        return _Res(impl_.Ptr2Offset(__pos._M_node),
                    impl_.Ptr2Offset(__pos._M_node));
      }
    } else {
      return _M_get_insert_unique_pos_offset(__k);
    }

  } else if (_M_key_compare(_S_key(__pos._M_node), __k)) {
    iterator __after = __pos;
    if (__pos._M_node == _M_rightmost()) {
      return _Res(-1, impl_.Ptr2Offset(_M_rightmost()));
    } else if (_M_key_compare(__k, _S_key((++__after)._M_node))) {
      if (_S_right(__pos._M_node) == 0) {
        return _Res(-1, impl_.Ptr2Offset(__pos._M_node));
      } else {
        return _Res(impl_.Ptr2Offset(__after._M_node),
                    impl_.Ptr2Offset(__after._M_node));
      }
    } else {
      return _M_get_insert_unique_pos_offset(__k);
    }

  } else {
    return _Res(impl_.Ptr2Offset(__pos._M_node), 0);
  }
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_insert_offset_(
    ptrdiff_t __x, ptrdiff_t __p, const _Val& __v) {
  bool __insert_left = (__x != -1 || impl_.Offset2Ptr(__p) == _M_end() ||
                        _M_key_compare(_KeyOfValue()(__v), _S_key(__p)));
  _Link_type __z = _M_create_node(__v);
  _Rb_tree_insert_and_rebalance<_Val>(__insert_left, __z, impl_.Offset2Ptr(__p),
                                      this->impl_[0]);
  return iterator(__z);
}
#else
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_Link_type,
          typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_Link_type>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_get_insert_unique_pos(
    const key_type& __k) {
  typedef std::pair<_Link_type, _Link_type> _Res;
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  bool __comp = true;
  while (__x != NULL) {
    __y = __x;
    __comp = _M_key_compare(__k, _S_key(__x));
    __x = __comp ? _S_left(__x) : _S_right(__x);
  }
  iterator __j = iterator(__y);
  if (__comp) {
    if (__j == begin()) {
      return _Res(__x, __y);
    } else {
      --__j;
    }
  }
  if (_M_key_compare(_S_key(__j._M_node), __k)) return _Res(__x, __y);
  return _Res(__j._M_node, NULL);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
std::pair<typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_Link_type,
          typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_Link_type>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_get_insert_hint_unique_pos(
    const_iterator __position, const key_type& __k) {
  iterator __pos = __position._M_const_cast();
  typedef std::pair<_Link_type, _Link_type> _Res;
  if (__pos._M_node == _M_end()) {
    if (size() > 0 && _M_key_compare(_S_key(_M_rightmost()), __k)) {
      return _Res(NULL, _M_rightmost());
    } else {
      return _M_get_insert_unique_pos(__k);
    }
  } else if (_M_key_compare(__k, _S_key(__pos._M_node))) {
    iterator __before = __pos;
    if (__pos._M_node == _M_leftmost()) {
      return _Res(_M_leftmost(), _M_leftmost());
    } else if (_M_key_compare(_S_key((--__before)._M_node), __k)) {
      if (_S_right(__before._M_node) == 0) {
        return _Res(NULL, __before._M_node);
      } else {
        return _Res(__pos._M_node, __pos._M_node);
      }
    } else {
      return _M_get_insert_unique_pos(__k);
    }
  } else if (_M_key_compare(_S_key(__pos._M_node), __k)) {
    iterator __after = __pos;
    if (__pos._M_node == _M_rightmost()) {
      return _Res(NULL, _M_rightmost());
    } else if (_M_key_compare(__k, _S_key((++__after)._M_node))) {
      if (_S_right(__pos._M_node) == 0) {
        return _Res(NULL, __pos._M_node);
      } else {
        return _Res(__after._M_node, __after._M_node);
      }
    } else {
      return _M_get_insert_unique_pos(__k);
    }
  } else {
    return _Res(__pos._M_node, NULL);
  }
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::_M_insert_(_Link_type __x,
                                                        _Link_type __p,
                                                        const _Val& __v) {
  bool __insert_left = (__x != NULL || __p == _M_end() ||
                        _M_key_compare(_KeyOfValue()(__v), _S_key(__p)));
  ptrdiff_t __p_offset = impl_.Ptr2Offset(__p);
  _Link_type __z = _M_create_node(__v);
  _Rb_tree_insert_and_rebalance<_Val>(
      __insert_left, __z, impl_.Offset2Ptr(__p_offset), this->impl_[0]);
  return iterator(__z);
}
#endif

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::find(const _Key& __k) {
  iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                    : __j;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::find(const _Key& __k) const {
  const_iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                    : __j;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::size_type
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::count(const _Key& __k) const {
  std::pair<const_iterator, const_iterator> __p = equal_range(__k);
  const size_type __n = std::distance(__p.first, __p.second);
  return __n;
}

/*
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
bool _Rb_tree<_Key, _Val, _KeyOfValue, _Compare>::__rb_verify() const {
  if (_M_impl._M_node_count == 0 || begin() == end())
    return _M_impl._M_node_count == 0 && begin() == end() &&
           this->alloc_.Header().left() == _M_end() &&
           this->alloc_.Header().right() == _M_end();

  unsigned int __len = _Rb_tree_black_count<_Val>(_M_leftmost(), _M_root());
  for (const_iterator __it = begin(); __it != end(); ++__it) {
    _Const_Link_type __x = static_cast<_Const_Link_type>(__it._M_node);
    _Const_Link_type __L = _S_left(__x);
    _Const_Link_type __R = _S_right(__x);

    if (__x->_M_color == _S_red)
      if ((__L && __L->_M_color == _S_red) || (__R && __R->_M_color == _S_red))
        return false;

    if (__L && _M_key_compare(_S_key(__x), _S_key(__L))) return false;
    if (__R && _M_key_compare(_S_key(__R), _S_key(__x))) return false;

    if (!__L && !__R && _Rb_tree_black_count<_Val>(__x, _M_root()) != __len)
      return false;
  }

  if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root())) return false;
  if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root())) return false;
  return true;
}
*/

template <typename _Val>
void _Rb_tree_rotate_left(_Rb_tree_node<_Val>* const __x,
                          _Rb_tree_node<_Val>& __header) {
  _Rb_tree_node<_Val>* const __y = __x->right();
  __x->SetRight(__y->left());
  if (__y->_M_left != DULL) {
    __y->left()->SetParent(__x);
  }
  __y->SetParent(__x->parent());

  if (__x == __header.parent()) {
    __header.SetParent(__y);
  } else if (__x == __x->parent()->left()) {
    __x->parent()->SetLeft(__y);
  } else {
    __x->parent()->SetRight(__y);
  }
  __y->SetLeft(__x);
  __x->SetParent(__y);
}

template <typename _Val>
void _Rb_tree_rotate_right(_Rb_tree_node<_Val>* const __x,
                           _Rb_tree_node<_Val>& __header) {
  _Rb_tree_node<_Val>* const __y = __x->left();
  __x->SetLeft(__y->right());
  if (__y->_M_right != DULL) {
    __y->right()->SetParent(__x);
  }
  __y->SetParent(__x->parent());

  if (__x == __header.parent()) {
    __header.SetParent(__y);
  } else if (__x == __x->parent()->right()) {
    __x->parent()->SetRight(__y);
  } else {
    __x->parent()->SetLeft(__y);
  }
  __y->SetRight(__x);
  __x->SetParent(__y);
}

template <typename _Val>
void _Rb_tree_insert_and_rebalance(const bool __insert_left,
                                   _Rb_tree_node<_Val>* __x,
                                   _Rb_tree_node<_Val>* __p,
                                   _Rb_tree_node<_Val>& __header) throw() {
  __x->SetParent(__p);
  __x->SetLeft(NULL);
  __x->SetRight(NULL);
  __x->_M_color = _S_red;

  if (__insert_left) {
    __p->SetLeft(__x);
    if (__p == &__header) {
      __header.SetParent(__x);
      __header.SetRight(__x);
    } else if (__p == __header.left()) {
      __header.SetLeft(__x);
    }
  } else {
    __p->SetRight(__x);
    if (__p == __header.right()) {
      __header.SetRight(__x);
    }
  }

  while (__x != __header.parent() && __x->parent()->_M_color == _S_red) {
    _Rb_tree_node<_Val>* const __xpp = __x->parent()->parent();
    if (__x->parent() == __xpp->left()) {
      _Rb_tree_node<_Val>* const __y = __xpp->right();
      if (__y && __y->_M_color == _S_red) {
        __x->parent()->_M_color = _S_black;
        __y->_M_color = _S_black;
        __xpp->_M_color = _S_red;
        __x = __xpp;
      } else {
        if (__x == __x->parent()->right()) {
          __x = __x->parent();
          _Rb_tree_rotate_left<_Val>(__x, __header);
        }
        __x->parent()->_M_color = _S_black;
        __xpp->_M_color = _S_red;
        _Rb_tree_rotate_right<_Val>(__xpp, __header);
      }
    } else {
      _Rb_tree_node<_Val>* const __y = __xpp->left();
      if (__y && __y->_M_color == _S_red) {
        __x->parent()->_M_color = _S_black;
        __y->_M_color = _S_black;
        __xpp->_M_color = _S_red;
        __x = __xpp;
      } else {
        if (__x == __x->parent()->left()) {
          __x = __x->parent();
          _Rb_tree_rotate_right<_Val>(__x, __header);
        }
        __x->parent()->_M_color = _S_black;
        __xpp->_M_color = _S_red;
        _Rb_tree_rotate_left<_Val>(__xpp, __header);
      }
    }
  }
  __header.parent()->_M_color = _S_black;
}

/*
template <typename _Val>
_Rb_tree_node<_Val>* _Rb_tree_rebalance_for_erase(
    _Rb_tree_node<_Val>* const __z, _Rb_tree_node<_Val>& __header) throw() {
  _Rb_tree_node<_Val>* __y = __z;
  _Rb_tree_node<_Val>* __x = NULL;
  _Rb_tree_node<_Val>* __x_parent = NULL;

  if (__y->_M_left == DULL) {
    __x = __y->right();
  } else if (__y->_M_right == DULL) {
    __x = __y->left();
  } else {
    __y = __y->right();
    while (__y->_M_left != DULL) {
      __y = __y->left();
    }
    __x = __y->right();
  }

  if (__y != __z) {
    __z->left()->SetParent(__y);
    __y->SetLeft(__z->left());
    if (__y != __z->right()) {
      __x_parent = __y->parent();
      if (__x) {
        __x->SetParent(__y->parent());
      }
      __y->parent()->SetLeft(__x);
      __y->SetRight(__z->right());
      __z->right()->SetParent(__y);
    } else {
      __x_parent = __y;
    }

    if (__header.parent() == __z) {
      __header.SetParent(__y);
    } else if (__z->parent()->left() == __z) {
      __z->parent()->SetLeft(__y);
    } else {
      __z->parent()->SetRight(__y);
    }
    __y->SetParent(__z->parent());
    std::swap(__y->_M_color, __z->_M_color);
    __y = __z;
  } else {
    __x_parent = __y->parent();
    if (__x) {
      __x->SetParent(__y->parent());
      if (__header.parent() == __z) {
        __header.SetParent(__x);
      } else if (__z->parent()->left() == __z) {
        __z->parent()->SetLeft(__x);
      } else {
        __z->parent()->SetRight(__x);
      }

      if (__header.left() == __z) {
        if (__z->_M_right == DULL) {
          __header.SetLeft(__z->parent());
        } else {
          __header.SetLeft(_Rb_tree_node<_Val>::_S_minimum(__x));
        }
      }

      if (__header.right() == __z) {
        if (__z->_M_left == DULL) {
          __header.SetRight(__z->parent());
        } else {
          __header.SetRight(_Rb_tree_node<_Val>::_S_maximum(__x));
        }
      }
    }
  }

  if (__y->_M_color != _S_red) {
    while (__x != __header.parent() &&
           (__x == 0 || __x->_M_color == _S_black)) {
      if (__x == __x_parent->left()) {
        _Rb_tree_node<_Val>* __w = __x_parent->right();
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          local_Rb_tree_rotate_left(__x_parent, __header);
          __w = __x->parent()->right();
        }
        if ((__w->_M_left == DULL || __w->left()->_M_color == _S_black) &&
            (__w->_M_right == DULL || __w->right()->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->parent();
        } else {
          if (__w->_M_right == DULL || __w->right()->_M_color == _S_black) {
            __w->left()->_M_color = _S_black;
            __w->_M_color = _S_red;
            local_Rb_tree_rotate_right(__w, __header);
            __w = __x_parent->right();
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_right != DULL) __w->right()->_M_color = _S_black;
          local_Rb_tree_rotate_left(__x_parent, __header);
          break;
        }
      } else {
        _Rb_tree_node<_Val>* __w = __x_parent->left();
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          local_Rb_tree_rotate_right(__x_parent, __header);
          __w = __x_parent->left();
        }
        if ((__w->_M_right == DULL || __w->right()->_M_color == _S_black) &&
            (__w->_M_left == DULL || __w->left()->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->parent();
        } else {
          if (__w->_M_left == DULL || __w->left()->_M_color == _S_black) {
            __w->right()->_M_color = _S_black;
            __w->_M_color = _S_red;
            local_Rb_tree_rotate_left(__w, __header);
            __w = __x_parent->left();
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_left != DULL) __w->left()->_M_color = _S_black;
          local_Rb_tree_rotate_right(__x_parent, __header);
          break;
        }
      }
    }
    if (__x) __x->_M_color = _S_black;
  }
  return __y;
}
*/

template <typename _Val>
unsigned int _Rb_tree_black_count(const _Rb_tree_node<_Val>* __node,
                                  const _Rb_tree_node<_Val>* __root) throw() {
  if (__node == 0) return 0;
  unsigned int __sum = 0;
  do {
    if (__node->_M_color == _S_black) ++__sum;
    if (__node == __root) break;
    __node = __node->parent();
  } while (1);
  return __sum;
}

#endif
