#ifndef SVL_DETAIL_WIN32_HPP
#define SVL_DETAIL_WIN32_HPP

#include <svl/detail/types.hpp>
#include <svl/base/types.hpp>

namespace svl {
namespace win32
{
	POINT from_point(point const&);
	point to_point  (POINT const&);
	SIZE  from_size (size const&);
	size  to_size   (SIZE const&);
	RECT  from_rect (rect const&);
	rect  to_rect   (RECT const&);
	RECT  from_rect2(rect2 const&);
	rect2 to_rect2  (RECT const&);
	COLORREF rgb_from_color(color const&);
	color    rgb_to_color  (COLORREF);

	/*----------------------------------------------------------------------*/

	struct system_object_t {};
	const system_object_t system_object = system_object_t();

	template <typename GdiObjectHandle> class gdi_object
	{
	public:
		explicit gdi_object(GdiObjectHandle);
		gdi_object(GdiObjectHandle, system_object_t);
		gdi_object(gdi_object const&);
		gdi_object& operator = (gdi_object const&);
		~gdi_object();
		void swap(gdi_object&); // throw()

		operator GdiObjectHandle() const; // throw()

	private:
		GdiObjectHandle handle_;
		long*           refs_;
	};
}}

#endif // SVL_DETAIL_WIN32_HPP
