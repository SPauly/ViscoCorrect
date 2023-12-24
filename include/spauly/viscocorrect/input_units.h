// ViscoCorrect - Correction factors for centrifugal pumps
// Copyright (C) 2023  Simon Pauly
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contact via <https://github.com/SPauly/ViscoCorrect> 

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