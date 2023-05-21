#pragma once

#include "imgui.h"
#include "implot.h"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlinear.h>
#include <gsl/gsl_blas.h>

#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace ViscoCorrect
{
    namespace Debug
    {
        struct CompressedCurveData
        {
            size_t dataSize;
            double *xData;
            double *yData;
            double ModelFunction(double _x, const gsl_vector *_parameters)
            {
                double a = gsl_vector_get(_parameters, 0);
                double b = gsl_vector_get(_parameters, 1);
                double c = gsl_vector_get(_parameters, 2);

                // Evaluate the model function for the given x-value
                return a * _x * _x + b * _x + c;
            };
        };
        class CurveFitting
        {
        public:
            CurveFitting();
            ~CurveFitting();

            void Render();
            void FitCurve(std::map<int, int> _data, int _iter);

        private:
            static int ResidualFunction(const gsl_vector *, void *, gsl_vector *);

            void RenderInPlot();

        private:
            ImVec2 m_size{434, 284};

            size_t ncurves = 2;

            std::vector<std::vector<double>> xData;
            std::vector<std::vector<double>> yData;
            std::vector<std::vector<double>> fittedX;
            std::vector<std::vector<double>> fittedY;

            std::vector<CompressedCurveData> compressed_data;
            size_t num_parameters = 3;
            std::vector<double> a, b, c;

            bool b_renderplot = false, b_function = false;

            std::shared_ptr<std::function<void()>> PlotRender_func;

            std::vector<std::map<int, int>> datas{
                {// CQ
                 {242, 174},
                 {242, 173},
                 {246, 173},
                 {253, 173},
                 {255, 172},
                 {262, 172},
                 {262, 171},
                 {263, 171},
                 {268, 171},
                 {272, 169},
                 {276, 169},
                 {277, 168},
                 {287, 164},
                 {288, 164},
                 {292, 162},
                 {302, 157},
                 {314, 152},
                 {322, 145},
                 {328, 140},
                 {334, 136}
                },
                { //Test
                    {242, 74},
                    {242, 73},
                    {246, 73},
                    {253, 73},
                    {255, 72},
                }};
        };

    } // namespace Debug

} // namespace ViscoCorrect
