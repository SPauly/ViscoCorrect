#ifndef SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H
#define SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H

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

#endif // SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H