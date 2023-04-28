#pragma once

#include <string>
#include <memory>

#include "imgui.h"
#include "Layer.h"
#include "Types.h"
#include "Calculator.h"

namespace ViscoCorrect
{

    class Project : public Layer
    {
    public:
        virtual void OnUIRender() override;

    private:
        //metadata
        std::string m_name = "";

        //data
        CalcParameters m_parameters;
        CorrectionFactors m_correction; 

        //Calculator --> shared with Graph
        std::shared_ptr<Calculator> m_calculator;
    };
}