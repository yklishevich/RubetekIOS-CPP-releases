#ifndef SVL_BASE_LAYOUT_HPP
#define SVL_BASE_LAYOUT_HPP

#include <svl/base/types.hpp>

namespace svl
{
	class placement
		: noncopyable
	{
	public:
		placement(i_container&);

		void place(rect const&, widget&);

		placement& operator % (rect const&);
		placement& operator % (widget&);

	private:
		i_container& container_;
		rect          rect_;
	};

	/*----------------------------------------------------------------------*/

	class extent
	{
	public:
		extent(); // default value
		extent(unsigned size);
		extent(unsigned size, unsigned multiplier);

		unsigned size() const;
		unsigned multiplier() const;
		bool     is_default() const;
		
	private:
		friend class base_layout;
		unsigned size_;
		unsigned multiplier_: 31;
		unsigned is_default_: 1;
	};

	/*----------------------------------------------------------------------*/

	class var_extent
	{
	public:
		var_extent();
		operator extent() const;
		var_extent operator * (unsigned) const;
		var_extent operator + (unsigned) const;

	private:
		unsigned size;
		unsigned multiplier;
	};

	var_extent operator * (unsigned, var_extent const& v);
	var_extent operator + (unsigned, var_extent const& v);

	const extent D = extent();
	const var_extent V = var_extent();

	/*----------------------------------------------------------------------*/

	class base_layout
	{
	public:
		typedef std::vector<range2> ranges2;

		base_layout( unsigned count );
		~base_layout();

		unsigned count() const;
		range2 margins() const;
		extent def_size() const;
		extent size( size_t pos ) const;
		extent def_spacing() const;
		extent spacing( size_t pos) const;

		void set_margins( int );
		void set_margins( range2 const& );
		void set_def_size( extent const& );
		void set_size( size_t pos, extent );
		void set_def_spacing( extent const& );
		void set_spacing( size_t pos, extent );

		void build_ranges( range const&, ranges2& );

	private:
		void update_size( extent const&, extent& );

		unsigned            total_sizes_;
		unsigned            total_muls_;
		int                 margin_;
		int                 margins_sum_;
		extent              def_size_;
		extent              def_spacing_;
		std::vector<extent> sizes_;
	};

	/*----------------------------------------------------------------------*/

	class grid_layout
		: noncopyable
	{
	public:
		typedef std::size_t size_t;
		typedef void (*profile)(grid_layout&);

		grid_layout(unsigned rows, unsigned cols, profile = 0);
		~grid_layout();

		void set_h_margins(int left, int right);
		void set_v_margins(int top, int bottom);

		void set_h_def_size(extent);
		void set_v_def_size(extent);

		void set_h_size(unsigned column, extent);
		void set_v_size(unsigned row, extent);

		void set_h_def_spacing(extent);
		void set_v_def_spacing(extent);

		void set_h_spacing(unsigned column, extent);
		void set_v_spacing(unsigned row, extent);

		size_t h_count() const;
		size_t v_count() const;

		void pin_widget(widget&, unsigned row, unsigned column, unsigned v_span = 1, unsigned h_span = 1);
		void unpin_widget(widget&);

		void resize_widgets(rect const&);

	private:
		typedef unsigned short index;
		struct widget_axes_data
		{
			index from;
			index to;
		};
		struct widget_data
		{
			widget_axes_data h;
			widget_axes_data v;
			widget*          w;
		};
		typedef std::list<widget_data> widget_datas;

		widget_datas::iterator find_widget(event::target*);
		void on_destroy_widget(event::target* target); // throw()

		base_layout  vl_;
		base_layout  hl_;
		widget_datas wdata_;
	};

	/*----------------------------------------------------------------------*/

	namespace layouts
	{
		void lines(grid_layout&);
	}

	/*----------------------------------------------------------------------*/

	struct row
	{
		unsigned value;
		row(unsigned v) : value(v) {}
	};

	/*----------------------------------------------------------------------*/

	struct column
	{
		unsigned value;
		column(unsigned v) : value(v) {}
	};

	/*----------------------------------------------------------------------*/

	struct skip_t
	{
		unsigned count;
		skip_t(unsigned c) : count( c ) {}
		skip_t operator() ( unsigned c ) const { return skip_t( c ); }
	};
	skip_t const skip = skip_t(1);

	/*----------------------------------------------------------------------*/

	struct span
	{
		unsigned rows;
		unsigned cols;
		widget*  widget_;

		span(widget& w, unsigned r, unsigned c) : rows(r), cols(c), widget_(&w) {}
	};

	/*----------------------------------------------------------------------*/

	class layout
		: noncopyable
	{
		static extent const n;

	public: 
		typedef std::size_t size_t;

		layout(i_container&, unsigned rows, unsigned columns, grid_layout::profile = 0);

		layout& horizontal();
		layout& vertical();

		layout& def_size(extent);
		layout& size(unsigned pos, extent);
		layout& sizes(extent, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n);

		layout& def_spacing(extent);
		layout& spacing(unsigned pos, extent);
		layout& spacings(extent, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n, extent=n);

		layout& margins(unsigned);
		layout& margins(unsigned margin0, unsigned margin1);

		layout& operator% (row);
		layout& operator% (column);
		layout& operator% (skip_t);
		layout& operator% (widget&);
		layout& operator% (span const&);

	private:
		void to_next_item(unsigned);
		i_container& container_;
		grid_layout& layout_;
		unsigned     axes_;
		unsigned     row_;
		unsigned     col_;
		unsigned     cols_;
	};
}

#endif // SVL_BASE_LAYOUT_HPP
