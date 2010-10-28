#include "acsetup.hpp"
#include "nextweb/utils/MappedFile.hpp"

namespace nextweb { namespace utils {

MappedFileImpl::Pointer const
MappedFileImpl::INVALID_POINTER = reinterpret_cast<MappedFileImpl::Pointer>(-1);

}} // namespaces

#ifdef NEXTWEB_DEBUG
#include "nextweb/inlines/utils/MappedFile.hpp"
#endif 
