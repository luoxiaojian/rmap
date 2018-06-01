#include "allocator.h"

enum _Rb_tree_color { _S_red = false, _S_black = true; }

struct _Rb_tree_node_base {
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef const _Rb_tree_node_base* _Const_Base_ptr;

  _Rb_tree_color _M_color;
  /*
  _Base_ptr _M_parent;
  _Base_ptr _M_left;
  _Base_ptr _M_right;
  */

  ptrdiff_t _M_parent;
  ptrdiff_t _M_left;
  ptrdiff_t _M_right;

  static _Base_ptr _S_minimum(_Base_ptr __x) {
    while (__x->_M_left != 0) __x += __x->_M_left;
    return __x;
  }

  static _Const_Base_ptr _S_minimum(_Const_Base_ptr __x) {
    while (__x->_M_left != 0) __x += __x->_M_left;
    return __x;
  }

  static _Base_ptr _S_maximum(_Base_ptr __x) {
    while (__x->_M_right != 0) __x += __x->_M_right;
    return __x;
  }

  static _Const_Base_ptr _S_maximum(_Const_Base_ptr __x) {
    while (__x->_M_right != 0) __x += __x->_M_right;
    return __x;
  }

  _Base_ptr get_parent() { return _M_parent ? this + _M_parent : NULL; }
  _Base_ptr get_left() { return _M_left ? this + _M_left : NULL; }
  _Base_ptr get_right() { return _M_right ? this + _M_right : NULL; }

  void set_parent(_Base_ptr* ptr) { _M_parent = ptr ? ptr - this : 0; }
  void set_left(_Base_ptr* ptr) { _M_left = ptr ? ptr - this : 0; }
  void set_right(_Base_ptr* ptr) { _M_right = ptr ? ptr - this : 0; }
};

template <typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base {
  typedef _Rb_tree_node<_Val>* _Link_type;

  _Val _M_value_field;

  _Val* _M_valptr() { return std::__addressof(_M_value_filed); }

  const _Val* _M_valptr() const { return std::__addressof(_M_value_filed); }
};

_Rb_tree_node_base* _Rb_tree_increment(_Rb_tree_node_base* __x) throw();

const _Rb_tree_node_base* _Rb_tree_increment(
    const _Rb_tree_node_base* __x) throw();

_Rb_tree_node_base* _Rb_tree_decrement(_Rb_tree_node_base* __x) throw();

const _Rb_tree_node_base* _Rb_tree_decrement(
    const _Rb_tree_node_base* __x) throw();

template <typename _Tp>
struct _Rb_tree_iterator {
  typedef _Tp value_type;
  typedef _Tp& reference;
  typedef _Tp* pointer;

  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_iterator<_Tp> _Self;
  typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
  typedef _Rb_tree_node<_Tp>* _Link_type;

  _Rb_tree_iterator() : _M_node() {}

  explicit _Rb_tree_iterator(_Link_type __x) : _M_node(__x) {}

  reference operator*() const {
    return *static_cast<_Link_type>(_M_node)->_M_valptr();
  }

  pointer operator->() const {
    return static_cast<_Link_type>(_M_node)->_M_valptr();
  }

  _Self& operator++() {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self& operator++(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self& operator--(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return __tmp;
  }

  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }

  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }

  _Base_ptr _M_node;
};

template <typename _Tp>
struct _Rb_tree_const_iterator {
  typedef _Tp value_type;
  typedef const _Tp& reference;
  typedef const _Tp* pointer;

  typedef _Rb_tree_iterator<_Tp> iterator;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  typedef _Rb_tree_const_iterator<_Tp> _Self;
  typedef _Rb_tree_node_base::_Const_Base_ptr _Base_ptr;
  typedef const _Rb_tree_node<_Tp>* _Link_type;

  _Rb_tree_iterator() : _M_node() {}

  explicit _Rb_tree_iterator(_Link_type __x) : _M_node(__x) {}

  _Rb_tree_const_iterator(const iterator& __it) : _M_node(__it._M_node) {}

  iterator _M_const_cast() const {
    return iterator(static_cast<typename iterator::_Link_type>(
        const_cast<typename iterator::_Base_ptr>(M_node)));
  }

  reference operator*() const {
    return *static_cast<_Link_type>(_M_node)->_M_valptr();
  }

  pointer operator->() const {
    return static_cast<_Link_type>(_M_node)->_M_valptr();
  }

  _Self& operator++() {
    _M_node = _Rb_tree_increment(_M_node);
    return *this;
  }

  _Self& operator++(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_increment(_M_node);
    return __tmp;
  }

  _Self& operator--() {
    _M_node = _Rb_tree_decrement(_M_node);
    return *this;
  }

  _Self& operator--(int) {
    _Self __tmp = *this;
    _M_node = _Rb_tree_decrement(_M_node);
    return __tmp;
  }

  bool operator==(const _Self& __x) const { return _M_node == __x._M_node; }

  bool operator!=(const _Self& __x) const { return _M_node != __x._M_node; }

  _Base_ptr _M_node;
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

void _Rb_tree_insert_and_rebalance(const bool __insert_left,
                                   _Rb_tree_node_base* __x,
                                   _Rb_tree_node_base* __p,
                                   _Rb_tree_node_base& __header) throw();

void _Rb_tree_node_base* _Rb_tree_rebalance_for_erase(
    _Rb_tree_node_base* const __z, _Rb_tree_node_base& __header) throw();

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare>
class _Rb_tree {
 protected:
  typedef _Rb_tree_node_base* _Base_ptr;
  typedef const _Rb_tree_node_base* _Const_Base_ptr;

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
  typedef ptrdiff_t differnce_type;

 protected:
  _Link_type _M_get_node() { return alloc_.GetNode(); }

  void _M_put_node(_Link_type __p) {}

  _Link_type _M_create_node(const value_type& __x) {
    _Link_type __tmp = _M_get_node();
    __tmp->_M_value_field = __x;
    return __tmp;
  }

  void _M_destrory_node(_Link_type __p) {}

  _Link_type _M_clone_node(_Const_Link_type __x) {
    _Link_type __tmp = _M_create_node(*__x->_M_valptr());
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = 0;
    __tmp->_M_right = 0;
    return __tmp;
  }

 protected:
  template <typename _Key_compare>
  struct _Rb_tree_impl {};

  _Rb_tree_impl<_Compare> _M_impl;

  _Base_ptr& _M_root() { return this->_M_impl._M_header._M_parent; }

  _Base_ptr& _M_leftmost() { return this->_M_impl._M_header._M_left; }

  _Base_ptr& _M_rightmost() { return this->_M_impl._M_header._M_right; }

  _Const_Base_ptr& _M_root() const { return this->_M_impl._M_header._M_parent; }

  _Const_Base_ptr& _M_leftmost() const {
    return this->_M_impl._M_header._M_left;
  }

  _Const_Base_ptr& _M_rightmost() const {
    return this->_M_impl._M_header._M_right;
  }



 protected:
 private:
  Allocator<_Rb_tree_node<_Val>>& alloc_;
};
