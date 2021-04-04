#ifndef VERSION_INFO_HPP
#define VERSION_INFO_HPP

#include "common/data/Version.hpp"

constexpr unsigned int version_majour{ 0U }; /**< Majour version of the CJMToolkit application. */
constexpr unsigned int version_minor{ 5U };  /**< Minor version of the CJMToolkit application. */
constexpr unsigned int version_build{ 1U };  /**< Build version of the CJMToolkit application. */

/**
 * @brief Version of the CJMToolkit application.
 */
const cjm::data::Version version{ version_majour, version_minor, version_build };

#endif // VERSION_INFO_HPP
