
namespace svl {
namespace ext
{
	template <typename T> void phase_diagram::draw(T x, T y)
	{
		perform_drawing(cdouble(x, y));
	}

	template <typename T> void phase_diagram::draw(std::complex<T> const& val)
	{
		perform_drawing(cdouble(static_cast<double>(val.real()), static_cast<double>(val.imag())));
	}

	template<> inline void phase_diagram::draw<double>(std::complex<double> const& val)
	{
		perform_drawing(val);
	}
}}
