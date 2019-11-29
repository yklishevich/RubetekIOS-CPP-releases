
namespace svl
{
	template <typename IIter> combo_box::combo_box(IIter first, IIter last)
	{
		create_combo_box();
		append( first, last );
	}

	template <typename IIter> void combo_box::append(IIter first, IIter last)
	{
		for (; first != last; ++first)
			append( *first );
	}


#if SVL_USE_BOOST_RANGE
	template <class Range> combo_box::combo_box(Range const& r)
	{
		create_combo_box();
		append_range( r );
	}

	template <class Range> void combo_box::append_range(Range const& r)
	{
		append( boost::begin(r), boost::end(r) );
	}
#endif
}