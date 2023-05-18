#include "CurveFitting.h"
#include "Application.h"

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

            PlotRender_func = std::make_shared<std::function<void()>>(std::bind(RenderInPlot, this)); 
        }
        
        CurveFitting::~CurveFitting()
        {
        }

        void CurveFitting::Render()
        {
            ImGui::Begin("Curve Fitting");
            if(ImGui::Button("Enable Render"))
            {
                Application::GetInstance()->GetGraph()->AddCallbackToPlot(PlotRender_func, 1);
            }
            if(ImGui::Button("Disable Render"))
            {
                Application::GetInstance()->GetGraph()->RemoveCallbackFromPlot(PlotRender_func, 1);
            }
            ImGui::End();
        }

        void CurveFitting::RenderInPlot()
        {
            ImPlot::PlotScatter("CQ", xData.data(), yData.data(), xData.size());
        }
    } // namespace Debug
    
} // namespace ViscoCorrect
