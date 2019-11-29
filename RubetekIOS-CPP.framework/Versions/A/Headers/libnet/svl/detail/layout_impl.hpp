
namespace svl
{
	placement::placement(i_container& container)
		: container_( container )
		, rect_( 0, 0, 0, 0 )
	{}

	void placement::place(rect const& r, widget& w)
	{
		w.resize( r );
		container_.add_widget( w );
	}

	placement& placement::operator % (rect const& r)
	{
		rect_ = r;
		return *this;
	}

	placement& placement::operator % (widget& w)
	{
		w.resize( rect_ );
		container_.add_widget( w );
		return *this;
	}

	/*----------------------------------------------------------------------*/

	extent::extent()
		: size_      ( 0 )
		, multiplier_( 0 )
		, is_default_( 1 )
	{}

	extent::extent(unsigned sz)
		: size_      ( sz )
		, multiplier_( 0  )
		, is_default_( 0  )
	{}

	extent::extent(unsigned sz, unsigned m)
		: size_      ( sz )
		, multiplier_( m  )
		, is_default_( 0  )
	{}

	unsigned extent::size() const
	{
		return size_;
	}

	unsigned extent::multiplier() const
	{
		return multiplier_;
	}

	bool extent::is_default() const
	{
		return is_default_;
	}


	/*----------------------------------------------------------------------*/

	var_extent::var_extent()
		: size      ( 0 )
		, multiplier( 1 )
	{}

	var_extent::operator extent() const
	{
		return extent(size, multiplier);
	}

	var_extent var_extent::operator * (unsigned val) const
	{
		var_extent r;
		r.size = size * val;
		r.multiplier = multiplier * val;
		return r;
	}

	var_extent var_extent::operator + (unsigned val) const
	{
		var_extent r;
		r.size = size + val;
		r.multiplier = multiplier;
		return r;
	}

	var_extent operator * (unsigned val, var_extent const& v)
	{
		return v * val;
	}

	var_extent operator + (unsigned val, var_extent const& v)
	{
		return v + val;
	}

	/*----------------------------------------------------------------------*/

	base_layout::base_layout( unsigned count )
		: total_sizes_( 0 )
		, total_muls_ ( 2 * count - 1 )
		, margin_     ( 0 )
		, margins_sum_( 0 )
		, def_size_   ( V )
		, def_spacing_( V )
	{
		def_size_.is_default_ = 1;
		def_spacing_.is_default_ = 1;
		sizes_.assign( 2 * count - 1, def_size_ );
		set_def_spacing( 3 );
	}

	base_layout::~base_layout()
	{}

	unsigned base_layout::count() const
	{
		return static_cast<unsigned>( (sizes_.size() + 1) / 2 );
	}

	range2 base_layout::margins() const
	{
		return range2( margin_, margins_sum_ - margin_ );
	}

	extent base_layout::def_size() const
	{
		return def_size_;
	}

	extent base_layout::size( size_t pos ) const
	{
		return sizes_.at( 2 * pos );
	}

	extent base_layout::def_spacing() const
	{
		return def_spacing_;
	}

	extent base_layout::spacing( size_t pos) const
	{
		return sizes_.at( 2 * pos + 1 );
	}

	void base_layout::set_margins( int val )
	{
		margin_ = val;
		margins_sum_ = 2 * val;
	}

	void base_layout::set_margins( range2 const& r )
	{
		margin_ = r.r0;
		margins_sum_ = r.r0 + r.r1;
	}

	void base_layout::update_size( extent const& val, extent& ext )
	{
		total_sizes_   -= ext.size_;
		total_muls_    -= ext.multiplier_;
		ext.size_       = val.size_;
		ext.multiplier_ = val.multiplier_;
		total_sizes_   += ext.size_;
		total_muls_    += ext.multiplier_;
	}

	void base_layout::set_def_size( extent const&  e )
	{
		if (e.is_default_)
			throw std::invalid_argument("размер не может задаваться значением по умолчанию");

		def_size_.size_       = e.size_;
		def_size_.multiplier_ = e.multiplier_;

		for (unsigned i = 0; i < sizes_.size(); i += 2)
		{
			extent& ext = sizes_[i];
			if (ext.is_default_)
				update_size( def_size_, ext );
		}
	}

	void base_layout::set_size( size_t pos, extent sz )
	{
		update_size( sz.is_default_ ? def_size_ : sz, sizes_.at(2 * pos) );
	}

	void base_layout::set_def_spacing( extent const& e )
	{
		if (e.is_default_)
			throw std::invalid_argument("размер не может задаваться значением по умолчанию");

		def_spacing_.size_       = e.size_;
		def_spacing_.multiplier_ = e.multiplier_;

		for (unsigned i = 1; i < sizes_.size(); i += 2)
		{
			extent& ext = sizes_[i];
			if (ext.is_default_)
				update_size( def_spacing_, ext );
		}
	}

	void base_layout::set_spacing( size_t pos, extent sz )
	{
		update_size( sz.is_default_ ? def_spacing_ : sz, sizes_.at(2 * pos + 1) );
	}

	void base_layout::build_ranges( range const& R, ranges2& result )
	{
		range r = norm( R );

		unsigned size = static_cast<unsigned>( r.dr <= margins_sum_ ? 0 : (r.dr - margins_sum_) );
		double v_k = size <= total_sizes_ ? 0. : (static_cast<double>(size - total_sizes_) / total_muls_);
		double v_pos = r.r + margin_;

		size_t count = sizes_.size();
		result.resize( (count + 1) / 2 );
		for (unsigned i = 0; i < count; i++)
		{
			extent const& e = sizes_[i];
			range2& r = result[i / 2];
			((i & 0x01) ? r.r1 : r.r0) = static_cast<int>(v_pos + 0.5);
			v_pos += e.size_ + (e.multiplier_ == 0 ? 0. : v_k * e.multiplier_);
		}
		result.back().r1 = static_cast<int>(v_pos + 0.5);
	}

	/*----------------------------------------------------------------------*/

	grid_layout::widget_datas::iterator grid_layout::find_widget(event::target* t)
	{
		for (widget_datas::iterator iter = wdata_.begin(), end = wdata_.end(); iter != end; ++iter)
			if (iter->w == t)
				return iter;
		return wdata_.end();
	}

	grid_layout::grid_layout(unsigned rows, unsigned cols, profile set_profile)
		: vl_( rows )
		, hl_( cols )
	{
		if (rows == 0)
			throw std::invalid_argument("количество строк не должно равняться 0");
		if (cols == 0)
			throw std::invalid_argument("количество столбцов не должно равняться 0");

		if (set_profile)
			set_profile( *this );
	}

	grid_layout::~grid_layout()
	{
		for (widget_datas::iterator iter = wdata_.begin(), end = wdata_.end(); iter != end; ++iter)
		{
			(*iter).w->on_destroy().remove( this );
		}
	}

	void grid_layout::on_destroy_widget(event::target* target) // throw()
	{
		widget_datas::iterator iter = find_widget(target);
		SVL_ASSERT( iter != wdata_.end() );
		wdata_.erase( iter );
	}

	void grid_layout::set_h_margins(int left, int right)
	{
		hl_.set_margins( range2(left, right) );
	}

	void grid_layout::set_v_margins(int top, int bottom)
	{
		vl_.set_margins( range2(top, bottom) );
	}

	void grid_layout::set_h_def_size( extent s )
	{
		hl_.set_def_size( s );
	}

	void grid_layout::set_v_def_size( extent s )
	{
		vl_.set_def_size( s );
	}

	void grid_layout::set_h_size( unsigned column, extent s )
	{
		hl_.set_size( column, s );
	}

	void grid_layout::set_v_size( unsigned row, extent s )
	{
		vl_.set_size( row, s );
	}

	void grid_layout::set_h_def_spacing( extent s )
	{
		hl_.set_def_spacing( s );
	}

	void grid_layout::set_v_def_spacing( extent s )
	{
		vl_.set_def_spacing( s );
	}

	void grid_layout::set_h_spacing( unsigned column, extent s )
	{
		hl_.set_spacing( column, s );
	}

	void grid_layout::set_v_spacing( unsigned row, extent s )
	{
		vl_.set_spacing( row, s );
	}

	grid_layout::size_t grid_layout::h_count() const
	{
		return hl_.count();
	}

	grid_layout::size_t grid_layout::v_count() const
	{
		return vl_.count();
	}

	void grid_layout::pin_widget( widget& w, unsigned row, unsigned column, unsigned v_span, unsigned h_span )
	{
		if (v_span == 0)
			throw std::invalid_argument("количество занимаемых ячеек по вертикали не может быть нулевым");
		if (v_span == 0)
			throw std::invalid_argument("количество занимаемых ячеек по горизонтали не может быть нулевым");
		if (vl_.count() < (row + v_span))
			throw std::invalid_argument("неверно заданы позиции элемента в сетке по вертикали");
		if (hl_.count() < (column + h_span))
			throw std::invalid_argument("неверно заданы позиции элемента в сетке по горизонтали");

		widget_datas::iterator iter = find_widget( &w );
		if (iter == wdata_.end())
		{
			w.on_destroy().add( &grid_layout::on_destroy_widget, this );
			try
			{
				iter = wdata_.insert( wdata_.end(), widget_data() );
			}
			catch (...)
			{
				w.on_destroy().remove( this );
				throw;
			}
			iter->w = &w;
		}
		iter->v.from = static_cast<index>(row);
		iter->v.to   = static_cast<index>(row + v_span - 1);
		iter->h.from = static_cast<index>(column);
		iter->h.to   = static_cast<index>(column + h_span - 1);
	}

	void grid_layout::unpin_widget( widget& w )
	{
		widget_datas::iterator iter = find_widget( &w );
		if (iter != wdata_.end() )
		{
			w.on_destroy().remove( this );
			wdata_.erase( iter );
		}
	}

	void grid_layout::resize_widgets( rect const& r )
	{
		if ( !is_norm(r) )
			throw std::invalid_argument("прямоугольник, охватывающий клиентскую область, должен быть нормированным");

		base_layout::ranges2 hp, vp;
		hl_.build_ranges( r.x_range(), hp );
		vl_.build_ranges( r.y_range(), vp );

		for (widget_datas::iterator iter = wdata_.begin(), end = wdata_.end(); iter != end; ++iter)
		{
			widget_data& wd = *iter;
			wd.w->resize( rect2( hp[wd.h.from].r0, vp[wd.v.from].r0, hp[wd.h.to].r1, vp[wd.v.to].r1 ));
		}
	}

	/*----------------------------------------------------------------------*/

	void layouts::lines(grid_layout& layout)
	{
		layout.set_h_def_spacing( 3 );
		layout.set_v_def_size( 22 );
		layout.set_v_def_spacing( 3 );
	}
}

/*----------------------------------------------------------------------*/

namespace
{
	namespace axes { enum type
	{
		none,
		vertical,
		horizontal
	};}

	const unsigned no_mul = 0x7FFFFFFF;
}

/*----------------------------------------------------------------------*/

namespace svl
{
	extent const layout::n = extent(0, no_mul);

	layout::layout(i_container& cont, unsigned rows, unsigned columns, grid_layout::profile profile)
		: container_( cont )
		, layout_   ( cont.create_layout( rows, columns, profile ))
		, axes_     ( axes::none )
		, row_      ( 0 )
		, col_      ( 0 )
		, cols_     ( columns )
	{}

	layout& layout::horizontal()
	{
		axes_ = axes::horizontal;
		return *this;
	}

	layout& layout::vertical()
	{
		axes_ = axes::vertical;
		return *this;
	}

	layout& layout::def_size(extent sz)
	{
		if (axes_ == axes::horizontal)
			layout_.set_h_def_size( sz );
		else if (axes_ == axes::vertical)
			layout_.set_v_def_size( sz );
		else
			{ SVL_ASSERT_FALSE(); }

		return *this;
	}

	layout& layout::def_spacing(extent sz)
	{
		if (axes_ == axes::horizontal)
			layout_.set_h_def_spacing( sz );
		else if (axes_ == axes::vertical)
			layout_.set_v_def_spacing( sz );
		else
			{ SVL_ASSERT_FALSE(); }

		return *this;
	}

	layout& layout::sizes(extent e0, extent e1, extent e2, extent e3, extent e4, extent e5, extent e6, extent e7, extent e8, extent e9)
	{
		if (axes_ == axes::horizontal || axes_ == axes::vertical)
		{
			void (grid_layout::*set)(unsigned, extent) = axes_ == axes::horizontal ? &grid_layout::set_h_size : &grid_layout::set_v_size;

			for (;;)
			{
				(layout_.*set)(0, e0);
				if (e1.multiplier() == no_mul) break;
				(layout_.*set)(1, e1);
				if (e2.multiplier() == no_mul) break;
				(layout_.*set)(2, e2);
				if (e3.multiplier() == no_mul) break;
				(layout_.*set)(3, e3);
				if (e4.multiplier() == no_mul) break;
				(layout_.*set)(4, e4);
				if (e5.multiplier() == no_mul) break;
				(layout_.*set)(5, e5);
				if (e6.multiplier() == no_mul) break;
				(layout_.*set)(6, e6);
				if (e7.multiplier() == no_mul) break;
				(layout_.*set)(7, e7);
				if (e8.multiplier() == no_mul) break;
				(layout_.*set)(8, e8);
				if (e9.multiplier() == no_mul) break;
				(layout_.*set)(9, e9);
				break;
			}
		}
		else
		{
			SVL_ASSERT_FALSE();
		}

		return *this;
	}

	layout& layout::size(unsigned pos, extent sz)
	{
		if (axes_ == axes::horizontal)
			layout_.set_h_size( pos, sz );
		else if (axes_ == axes::vertical)
			layout_.set_v_size( pos, sz );
		else
			{ SVL_ASSERT_FALSE(); }

		return *this;
	}

	layout& layout::spacing(unsigned pos, extent sz)
	{
		if (axes_ == axes::horizontal)
			layout_.set_h_spacing( pos, sz );
		else if (axes_ == axes::vertical)
			layout_.set_v_spacing( pos, sz );
		else
			{ SVL_ASSERT_FALSE(); }

		return *this;
	}

	layout& layout::spacings(extent e0, extent e1, extent e2, extent e3, extent e4, extent e5, extent e6, extent e7, extent e8, extent e9)
	{
		if (axes_ == axes::horizontal || axes_ == axes::vertical)
		{
			void (grid_layout::*set)(unsigned, extent) = axes_ == axes::horizontal ? &grid_layout::set_h_spacing : &grid_layout::set_v_spacing;

			for (;;)
			{
				(layout_.*set)(0, e0);
				if (e1.multiplier() == no_mul) break;
				(layout_.*set)(1, e1);
				if (e2.multiplier() == no_mul) break;
				(layout_.*set)(2, e2);
				if (e3.multiplier() == no_mul) break;
				(layout_.*set)(3, e3);
				if (e4.multiplier() == no_mul) break;
				(layout_.*set)(4, e4);
				if (e5.multiplier() == no_mul) break;
				(layout_.*set)(5, e5);
				if (e6.multiplier() == no_mul) break;
				(layout_.*set)(6, e6);
				if (e7.multiplier() == no_mul) break;
				(layout_.*set)(7, e7);
				if (e8.multiplier() == no_mul) break;
				(layout_.*set)(8, e8);
				if (e9.multiplier() == no_mul) break;
				(layout_.*set)(9, e9);
				break;
			}
		}
		else
		{
			SVL_ASSERT_FALSE();
		}

		return *this;
	}

	layout& layout::margins(unsigned sz)
	{
		return margins(sz, sz);
	}

	layout& layout::margins(unsigned sz0, unsigned sz1)
	{
		if (axes_ == axes::horizontal)
			layout_.set_h_margins( sz0, sz1 );
		else if (axes_ == axes::vertical)
			layout_.set_v_margins( sz0, sz1 );
		else
			{ SVL_ASSERT_FALSE(); }

		return *this;
	}

	layout& layout::operator % (row r)
	{
		row_ = r.value;
		return *this;
	}

	layout& layout::operator % (column c)
	{
		col_ = c.value;
		return *this;
	}

	layout& layout::operator % (skip_t s)
	{
		for (unsigned i = 0; i < s.count; i++)
			to_next_item( 1 );

		return *this;
	}

	layout& layout::operator % (widget& w)
	{
		container_.add_widget( w );
		layout_.pin_widget( w, row_, col_ );
		to_next_item( 1 );

		return *this;
	}

	layout& layout::operator % (span const& s)
	{
		SVL_ASSERT( s.widget_ != 0 );

		container_.add_widget( *s.widget_ );
		layout_.pin_widget( *s.widget_, row_, col_, s.rows, s.cols );
		to_next_item( s.cols );

		return *this;
	}

	void layout::to_next_item( unsigned cc )
	{
		col_ += cc;
		if (cols_ <= col_)
		{
			col_ = 0;
			row_++;
		}

	}
}
