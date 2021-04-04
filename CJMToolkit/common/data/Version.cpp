#include "Version.hpp"

namespace cjm::data
{
   Version::Version(unsigned int majour, unsigned int minor, unsigned int build) :
      majour_{ majour }, minor_{ minor }, build_{ build }
   {
      toString_();
   }

   std::string_view Version::getString() const
   {
      return versionString_;
   }

   void Version::toString_()
   {
      versionString_ = std::to_string(majour_) + separator.data() + std::to_string(minor_) + separator.data() +
                       std::to_string(build_);
   }
} // namespace cjm::data
