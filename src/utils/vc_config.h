#pragma once

#include <map>
#include <vector>

#include "imgui.h"

namespace ViscoCorrect
{
    namespace VCConfig
    {
        struct Config
        {

        };
        
        enum EventType
        {
            CALC_REQ,
            REND_REQ
        };

        struct RawData
        {
            //Table sizes
            const ImVec2 table_size{434,625};
            const ImVec2 table_param_size{434, 304};
            const ImVec2 table_correct_size{434, 284};

            //Correction Factors
            int stepsize_correction = 22;
            std::vector<double> cq{4.3286373442021278e-09, -6.5935466655309209e-06, 0.0039704102541411324, -1.1870337647376101, 176.52190832690891, -10276.558815133236};
            std::vector<double> cv{2.5116987378131985e-10, -3.2416532447274418e-07, 0.00015531747394399714, -0.037300324399145976, 4.2391803778160968, -6.2364025573465849};
            std::vector<std::vector<double>> ch;

            //flowrate
            int startpos_f[2]{0,0};
            std::map<int, int> flowrates{
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

            //total head
            int startpos_th[2]{4, 1}; // have to check if that is correct
            const double pitch_th = (553.0 - 327.0) / 430.0;
            std::map<int, int> totalhead{
            {5, 0},
            {10, 15},
            {20, 12},
            {40, 14},
            {50, 8},
            {100, 9},
            {200, 13}};

            //viscosity
            int startpos_v[2] {105, 304}; // have to check this
            const double pitch_v = (31.0 - 304.0) / (377.0 - 234.0);
            std::map<int, int> viscosity{
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

        static Config s_config_instance;

        inline static Config &GetConfig() { return s_config_instance; }
        
    } // namespace VCConfig
    
} // namespace ViscoCorrect
