#pragma once

#include "imgui.h"
#include "implot.h" 

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <vector>
#include <map>
#include <functional>
#include <memory>

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
            void FitCurve();

        private:
            double ModelFunction(double, const gsl_vector*);
            int ResidualFunction(const gsl_vector*, void *, gsl_vector*);

            void RenderInPlot();

        private:
            ImVec2 m_size{434, 284};

            std::vector<double> xData;
            std::vector<double> yData;
            double a,b,c;

            std::shared_ptr<std::function<void()>> PlotRender_func;

            std::map<int, int> data{
                {242,174},
                {242,173},
                {246,173},
                {253,173},
                {255,172},
                {262,172},
                {262,171},
                {263,171},
                {268,171},
                {272,169},
                {276,169},
                {277,168},
                {287,164},
                {288,164},
                {292,162},
                {302,157},
                {314,152},
                {322,145},
                {328,140},
                {334,136}
            };
        };
        
    } // namespace Debug
    
} // namespace ViscoCorrect
