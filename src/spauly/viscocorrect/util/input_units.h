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
enum class ViscosityUnits : int {
  kSquareMillimetersPerSecond,
  kCentiStokes,
  kCentiPoise,
  kMilliPascalSeconds
};
enum class DensityUnits : int { kGramPerLiter, kKilogramsPerCubicMeter };

}  // namespace util

namespace internal {

const std::unordered_map<util::FlowrateUnits, float> kConversionFlowrate{
    {util::FlowrateUnits::kCubicMetersPerHour, 1.0f},
    {util::FlowrateUnits::kLitersPerMinute, 0.06f},
    {util::FlowrateUnits::kGallonsPerMinute, 0.2771f}};

const std::unordered_map<util::TotalHeadUnits, float> kConversionTotalHead{
    {util::TotalHeadUnits::kMeters, 1.0f},
    {util::TotalHeadUnits::kFeet, 0.3048}};

const std::unordered_map<util::DensityUnits, float> kConversionDensity{
    {util::DensityUnits::kGramPerLiter, 1.0f},
    {util::DensityUnits::kKilogramsPerCubicMeter, 1000.0f}};

}  // namespace internal

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTIL_INPUT_UNITS_H