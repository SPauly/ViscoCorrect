#pragma once

#include "imgui.h"
#include "implot.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        class CurveFitting
        {
        public:
            CurveFitting();
            ~CurveFitting();

            void Render();

        private:
            ImVec2 m_size{434, 284};
            
        };
        
    } // namespace Debug
    
} // namespace ViscoCorrect
