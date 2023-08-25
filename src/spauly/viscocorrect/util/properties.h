// ViscoCorrect - Correction factors centrifugal pumps
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

#ifndef SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H
#define SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H

#include <map>
#include <vector>

namespace viscocorrect {

// implemented in this file
struct InputRestrictions;
enum CurvesOrderH : int { k06 = 0, k08 = 1, k10 = 2, k12 = 3 };
bool IsFlowrateInputOkay(const float &flowrate);
bool IsTotalHeadInputOkay(const float &total_head);
bool IsViscosityInputOkay(const float &viscosity);

namespace internal {
struct Properties;
extern const Properties kProperties;

bool ValidateXH(const double &x);
bool ValidateXQ(const double &x);
bool ValidateXEta(const double &x);

//---------------------
// implementation
struct Properties {
  // Table sizes
  const float kTableWidth = 434.0f;
  const float kTableParameterHeight = 304.0f;
  const float kTableCorrectionHeight = 284.0f;

  // Input restrictions
  const float kInputFlowrate[2]{6, 2000};
  const float kInputTotalHead[2]{5, 200};
  const float kInputViscosity[2]{10, 4000};

  // Correction Factors
  const int kCorrectionScale =
      22;  // pixels between each correction factor on scale
  const int kCutoffQ[2]{242, 384};
  const int kCutoffEta[2]{122, 363};
  const int kCutoffH[2]{146, 382};
  const std::vector<double> kCoefficientsQ{
      4.3286373442021278e-09, -6.5935466655309209e-06, 0.0039704102541411324,
      -1.1870337647376101,    176.52190832690891,      -10276.558815133236};
  const std::vector<double> kCoefficientsEta{
      2.5116987378131985e-10, -3.2416532447274418e-07, 0.00015531747394399714,
      -0.037300324399145976,  4.2391803778160968,      -6.2364025573465849};
  const std::vector<std::vector<double>> kCoefficientsH{
      {285.39113639063004, -0.019515612319848788, 451.79876054847699}, //0.6
      {286.44331640461877, -0.016739174282778945, 453.11949555301783}, //0.8
      {285.70823636118865, -0.016126836943018912, 443.60573501332937}, //1.0
      {285.91175890816675, -0.015057232233799856, 436.03377039579027}  // 1.2
  };  // yet to be determined

  // Parameters
  const int kStartFlowrate[2]{0, 0};
  const int kStartTotalH[2]{4, 1};     // have to check if that is correct
  const int kStartVisco[2]{105, 304};  // have to check this
  const double kPitchTotalH = (553.0 - 327.0) / 430.0;
  const double kPitchVisco = (31.0 - 304.0) / (377.0 - 234.0);

  const std::map<int, int> kFlowrateScale{
      {6, 0},    {7, 14},    {8, 9},    {9, 9},    {10, 9},   {15, 30},
      {20, 21},  {30, 30},   {40, 21},  {50, 17},  {60, 13},  {70, 12},
      {80, 9},   {90, 9},    {100, 9},  {150, 30}, {200, 21}, {300, 30},
      {400, 21}, {500, 17},  {600, 14}, {700, 11}, {800, 10}, {900, 8},
      {1000, 8}, {1500, 30}, {2000, 22}};

  const std::map<int, int> kTotalHeadScale{
      {5, 0}, {10, 15}, {20, 12}, {40, 14}, {50, 8}, {100, 9}, {200, 13}};

  std::map<int, int> kViscoScale{
      {10, 0},   {20, 27},  {30, 16},   {40, 10},   {60, 15},  {80, 11},
      {100, 8},  {200, 26}, {300, 16},  {400, 11},  {500, 8},  {600, 6},
      {800, 12}, {1000, 9}, {2000, 26}, {3000, 14}, {4000, 10}};
};
}  // namespace internal

struct InputRestrictions {
  // Input restrictions
  const float *kInputFlowrate = internal::kProperties.kInputFlowrate;
  const float *kInputTotalHead = internal::kProperties.kInputTotalHead;
  const float *kInputViscosity = internal::kProperties.kInputViscosity;
};

}  // namespace viscocorrect

#endif  // SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H