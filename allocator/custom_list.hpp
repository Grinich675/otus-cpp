#pragma once

#include <memory>

template<typename _Tp>
struct _list_node
    {
	  _Tp storage;
      _list_node* next;
      _list_node* prev;

      _list_node():storage{},next(nullptr),prev(nullptr)
      {}

      _list_node(_Tp val,_list_node* _prev, _list_node* _next):storage(val),next(_next),prev(_prev)
      {}
    };


template<typename _Tp>
struct _custom_list_iterator
{
	 typedef _custom_list_iterator<_Tp>         _Self;
     typedef _list_node<_Tp>                    _Node;
     typedef std::bidirectional_iterator_tag    iterator_category;
     typedef _Tp                                value_type;
     typedef _Tp*                               pointer;
     typedef _Tp&                               reference;

     _Node* _M_node;

     _custom_list_iterator(): _M_node()
     {}

     _custom_list_iterator(_Node* _x): _M_node(_x)
     {}

     reference  operator*()
     {
   		 return _M_node->storage;
     }

     pointer operator->()
     {
    	 if(_M_node!=nullptr)
    		 return std::addressof(_M_node->storage);
    	 else return nullptr;
     }

     _Self& operator++()
     {
    	 _M_node = _M_node->next;
    	 return *this;
     }

     _Self operator++(int)
     {
		_Self __tmp = *this;
		_M_node = _M_node->next;
		return __tmp;
     }

     _Self& operator--()
     {
		_M_node = _M_node->prev;
		return *this;
     }

     _Self operator--(int)
     {
		_Self __tmp = *this;
		_M_node = _M_node->prev;
		return __tmp;
     }

      bool operator==(const _Self& __x)
      { return _M_node == __x._M_node; }

      bool operator!=(const _Self& __x)
      { return _M_node != __x._M_node; }

 };



template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class custom_list
	{

    using _Node = _list_node<_Tp>;

	using NodeAlloc =
	  typename std::allocator_traits<_Alloc>::template rebind_alloc<_Node>;

	NodeAlloc _a;

	_Node* head;
	_Node* tail;


	public:

	typedef _custom_list_iterator<_Tp>                        iterator;
    typedef _Alloc	allocator_type;



	custom_list():_a(),head(nullptr),tail(nullptr)
	{
		auto fake_node=_a.allocate(1);
		tail=fake_node;
		tail->prev = nullptr;
		tail->next= nullptr;
		head=tail;

	}

	void push_back(const _Tp& value)
	{
		auto new_node=_a.allocate(1);

		_a.construct(new_node,value,tail->prev,tail);

		if(tail->prev!= nullptr)
		{
			tail->prev->next=new_node;
		}
		else if( head == tail)
		{
			head=new_node;
		}

		tail->prev=new_node;


	}


	void push_front(const _Tp& value)
	{
		auto new_node=_a.allocate(1);

		_a.construct(new_node,value,nullptr,nullptr);

		if(head == tail)
		{
			tail->prev = new_node;
			head = new_node;
			new_node->next =tail;
		}
		else
		{
			head->prev=new_node;
			new_node->next = head;
			head = new_node;
		}
	}

	iterator begin() noexcept
	{
		return iterator(this->head);
	}

	//return iterator to space after last element
	iterator end() noexcept
	{
		return iterator(this->tail);
	}

};



