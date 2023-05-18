#include "CurveFitting.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        CurveFitting::CurveFitting()
        {
            //populate x and y Data
            for(const auto& pair : data)
            {
                xData.push_back(pair.first);
                yData.push_back(pair.second);
            }
        }
        
        CurveFitting::~CurveFitting()
        {
        }

        void CurveFitting::Render()
        {
            ImGui::Begin("CurveFitting");
            if(ImPlot::BeginPlot("Curve Fitting", m_size))
            {
                            // Set up Graph
                ImPlot::SetupAxis(ImAxis_X1, nullptr);
                ImPlot::SetupAxis(ImAxis_Y1, nullptr);
                ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, m_size.x);
                ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, m_size.y);
                ImPlot::PlotScatter("Data CQ", xData.data(), yData.data(), xData.size());
                ImPlot::EndPlot();
            }
            ImGui::End();
        }
    } // namespace Debug
    
} // namespace ViscoCorrect
