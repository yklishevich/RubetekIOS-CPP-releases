#ifndef SVL_DETAIL_SHARED_PTR
#define SVL_DETAIL_SHARED_PTR

namespace svl
{
	template <typename T> class shared_ptr
	{
	public:
		shared_ptr()
			: ptr_ ( 0 )
			, refs_( new long(1) )
		{}

		shared_ptr(T* p)
			: ptr_ ( p )
			, refs_( new long(1) )
		{}

		shared_ptr(shared_ptr const& other)
			: ptr_ ( other.ptr_ )
			, refs_( other.refs_ )
		{
			::InterlockedIncrement( refs_ );
		}

		shared_ptr& operator = (shared_ptr const& other) // throw()
		{
			if (this != &other)
			{
				if (::InterlockedDecrement( refs_ ) == 0)
				{
					delete ptr_;
					delete refs_;
				}
				ptr_  = other.ptr_;
				refs_ = other.refs_;
				::InterlockedIncrement( refs_ );
			}
			return *this;
		}

		~shared_ptr()
		{
			if (::InterlockedDecrement( refs_ ) == 0)
			{
				delete ptr_;
				delete refs_;
			}
		}

		T* get() const
		{
			return ptr_;
		}

		void reset(T* p = 0)
		{
			if ( *refs_ == 1 )
			{
				delete ptr_;
				ptr_ = p;
			}
			else
			{
				shared_ptr sptr( p );
				swap( sptr );
			}
		}

		T* operator -> () const
		{
			return ptr_;
		}

		T& operator * () const
		{
			return *ptr_;
		}

		void swap(shared_ptr& other) // throw()
		{
			using std::swap;
			swap( ptr_,  other.ptr_ );
			swap( refs_, other.refs_ );
		}

	private:
		T*    ptr_;
		long* refs_;
	};
}

#endif // SVL_DETAIL_SHARED_PTR
