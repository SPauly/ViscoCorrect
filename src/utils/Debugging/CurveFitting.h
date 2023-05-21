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

            double ModelFunctionLogistic(double _x, const gsl_vector *_parameters)
            {
                double L = gsl_vector_get(_parameters, 0);
                double k = gsl_vector_get(_parameters, 1);
                double x0 = gsl_vector_get(_parameters, 2);

                return L /(1+ exp(-k*(_x-x0)));
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
                 {242, 173}, // n
                 {246, 173}, // n
                 {253, 173}, // n
                 {255, 172}, // n
                 {262, 170},
                 {262, 171}, // n
                 {263, 171}, // n
                 {268, 171}, // n
                 {272, 169}, // n
                 {276, 169}, // n
                 {277, 168}, // n
                 {287, 164}, // n
                 {288, 164}, // n
                 {293, 162}, // n
                 {302, 157}, // n
                 {313, 151},
                 {322, 145}, // n
                 {328, 140}, // n
                 {330, 140},
                 {338, 131},
                 {343, 124},
                 {348, 119},
                 {356, 108},
                 {364, 96},
                 {374, 82},
                 {378, 75},
                 {381, 71},
                 {383, 67}},
                {
                    // Test
                    {242, 74},
                    {242, 73},
                    {246, 73},
                    {253, 73},
                    {255, 72},
                }};
        };

    } // namespace Debug

} // namespace ViscoCorrect
