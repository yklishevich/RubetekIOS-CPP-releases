#include <rubetek/config.hpp>

#include <msw/config.hpp>

#include <lama/thrift/include/thrift.hpp>
#include <lama/thrift/include/protocol/tprotocol.hpp>

#ifdef MSW_MSVC
#   pragma warning (push)
#   pragma warning (disable: 4127)
#endif

#include <rubetek/imc_types.hpp>

#include "imc_constants.cpp"
#include "imc_types.cpp"

#ifdef MSW_MSVC
#   pragma warning (pop)
#endif
