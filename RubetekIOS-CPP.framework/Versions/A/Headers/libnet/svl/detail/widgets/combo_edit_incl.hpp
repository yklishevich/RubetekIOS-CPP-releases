
namespace svl
{
	template <typename IIter> combo_edit::combo_edit(str_ref text, IIter first, IIter last)
	{
		create_combo_edit();
		append(first, last);
		set_text(text);
	}

	template <typename IIter> void combo_edit::append(IIter first, IIter last)
	{
		for (; first != last; ++first)
			append(*first);
	}


#if SVL_USE_BOOST_RANGE
	template <class Range> combo_edit::combo_edit(str_ref text, Range const& r)
	{
		create_combo_edit();
		append_range(r);
		set_text(text);
	}

	template <class Range> void combo_edit::append_range(Range const& r)
	{
		append(boost::begin(r), boost::end(r));
	}
#endif
}