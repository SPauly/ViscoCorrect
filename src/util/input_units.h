#ifndef VISCOCORRECT_SRC_UTIL_INPUT_UNITS_H
#define VISCOCORRECT_SRC_UTIL_INPUT_UNITS_H

namespace viscocorrect
{
    namespace util
    {
        enum class ViscoUnits
        {
            kMilliPascalSeconds,
            kCentiStoke
        };

        enum class FlowrateUnits
        {
            kLitersPerMinute,
            kGallonsPerMinute
        };
    } // namespace util

} // namespace viscocorrect

#endif // VISCOCORRECT_SRC_UTIL_INPUT_UNITS_H