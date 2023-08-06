#ifndef SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H
#define SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H

#include <unordered_map>

namespace viscocorrect {
namespace util {

enum class FlowrateUnits : int {
  kCubicMetersPerHour,
  kLitersPerMinute,
  kGallonsPerMinute
};
enum class TotalHeadUnits : int { kMeters, kFeet };
enum class ViscoUnits : int {
  kSquareMillimetersPerSecond,
  kCentiStokes,
  kCentiPoise,
  kMilliPascalSeconds
};
enum class DensityUnits : int { kKilogramsPerCubiMeter, kGallonsPerLiter };

}  // namespace util

namespace internal {

const std::unordered_map<util::FlowrateUnits, float> kConversionFlowrate{
    {util::FlowrateUnits::kCubicMetersPerHour, 1.0f},
    {util::FlowrateUnits::kLitersPerMinute, 0.06f},
    {util::FlowrateUnits::kGallonsPerMinute, 0.2771f}};

}  // namespace internal

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H