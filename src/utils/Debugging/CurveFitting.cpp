#include "CurveFitting.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        CurveFitting::CurveFitting()
        {

        }
        
        CurveFitting::~CurveFitting()
        {
        }

        void CurveFitting::Render()
        {
            ImGui::Begin("CurveFitting");
            if(ImPlot::BeginPlot("Curve Fitting", m_size))
            {
                
                ImPlot::EndPlot();
            }
            ImGui::End();
        }
    } // namespace Debug
    
} // namespace ViscoCorrect
