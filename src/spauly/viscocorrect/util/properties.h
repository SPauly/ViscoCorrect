#ifndef SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H
#define SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H

#include <map>
#include <vector>

namespace viscocorrect
{
    namespace internal
    {
        struct Properties
        {
            // Table sizes
            const float kTableWidth = 434.0f;
            const float kTableParameterHeight = 304.0f;
            const float kTableCorrectionHeight = 284.0f;

            // Correction Factors
            const int kCorrectionScale = 22; // pixels between each correction factor on scale
            const int kCutoffQ[2]{242, 384};
            const int kCutoffV[2]{122, 363};
            const int kCutoffH[2]{0, 1}; // yet to be determined
            const std::vector<double> kCoefficientsQ{4.3286373442021278e-09, -6.5935466655309209e-06, 0.0039704102541411324, -1.1870337647376101, 176.52190832690891, -10276.558815133236};
            const std::vector<double> kCoefficientsV{2.5116987378131985e-10, -3.2416532447274418e-07, 0.00015531747394399714, -0.037300324399145976, 4.2391803778160968, -6.2364025573465849};
            const std::vector<std::vector<double>> kCoefficientsH; // yet to be determined

            // Parameters
            const int kStartFlowrate[2]{0, 0};
            const int kStartTotalH[2]{4, 1};    // have to check if that is correct
            const int kStartVisco[2]{105, 304}; // have to check this
            const double kPitchTotalH = (553.0 - 327.0) / 430.0;
            const double kPitchVisco = (31.0 - 304.0) / (377.0 - 234.0);

            const std::map<int, int> kFlowrateScale{
                {6, 0},
                {7, 14},
                {8, 9},
                {9, 9},
                {10, 9},
                {15, 30},
                {20, 21},
                {30, 30},
                {40, 21},
                {50, 17},
                {60, 13},
                {70, 12},
                {80, 9},
                {90, 9},
                {100, 9},
                {150, 30},
                {200, 21},
                {300, 30},
                {400, 21},
                {500, 17},
                {600, 14},
                {700, 11},
                {800, 10},
                {900, 8},
                {1000, 8},
                {1500, 30},
                {2000, 22}};

            const std::map<int, int> kTotalHeadScale{
                {5, 0},
                {10, 15},
                {20, 12},
                {40, 14},
                {50, 8},
                {100, 9},
                {200, 13}};

            std::map<int, int> kViscoScale{
                {10, 0},
                {20, 27},
                {30, 16},
                {40, 10},
                {60, 15},
                {80, 11},
                {100, 8},
                {200, 26},
                {300, 16},
                {400, 11},
                {500, 8},
                {600, 6},
                {800, 12},
                {1000, 9},
                {2000, 26},
                {3000, 14},
                {4000, 10}};
        };

        extern const Properties kProperties;
    } // namespace internal

} // namespace ViscoCorrect

#endif // SPAULY_VISCOCORRECT_UTIL_PROPERTIES_H