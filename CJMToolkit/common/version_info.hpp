#ifndef VERSION_INFO_HPP
#define VERSION_INFO_HPP

#include "common/data/Version.hpp"

namespace cjm
{
   constexpr unsigned int version_majour{ 1U }; /**< Majour version of the CJM common functionality. */
   constexpr unsigned int version_minor{ 1U };  /**< Minor version of the CJM common functionality. */
   constexpr unsigned int version_build{ 1U };  /**< Build version of the CJM common functionality. */

   /**
    * @brief Version of the CJM common functionality.
    */
   const data::Version version{ version_majour, version_minor, version_build };
} // namespace cjm

#endif // VERSION_INFO_HPP
