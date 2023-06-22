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
            bool b_poly;
            
            double ModelFunctionPolynom(double _x, const gsl_vector *_parameters)
            {
                double y = 0;
                int inv_i = _parameters->size - 1;

                for (int i = 0; i < _parameters->size; i++)
                {
                    y += static_cast<double>(gsl_vector_get(_parameters, i) * std::pow((double)_x, (double)inv_i));
                    --inv_i;
                }

                return y;
            };

            double ModelFunctionLogistic(double _x, const gsl_vector *_parameters)
            {
                double L = gsl_vector_get(_parameters, 0);
                double k = gsl_vector_get(_parameters, 1);
                double x0 = gsl_vector_get(_parameters, 2);

                return L / (1 + exp(-k * (_x - x0)));
            };

            double ModelFunction(double _x, const gsl_vector *_parameters)
            {
                return (b_poly) ? ModelFunctionPolynom(_x, _parameters) : ModelFunctionLogistic(_x, _parameters);
            }

            double ModelFunction(double _x, const std::vector<double> &_parameters)
            {
                gsl_vector *_params_g = gsl_vector_alloc(_parameters.size());
                for(int i = 0; i < _parameters.size(); i++)
                {
                    gsl_vector_set(_params_g, i, _parameters.at(i));
                }

                double y = this->ModelFunction(_x, _params_g);
                gsl_vector_free(_params_g);
                return y;
            }
        };

        struct FullDataCurve
        {
            FullDataCurve(std::map<int, int>& _data, bool _bpoly = true, int _params = 3);

            std::vector<double> xData, yData, fittedX, fittedY;
            std::vector<double> parameters;
            
            CompressedCurveData compressed_data;
        };

        class CurveFitting
        {
        public:
            CurveFitting();
            ~CurveFitting();

            void Render();
            void FitCurve(FullDataCurve&);

        private:
            static int ResidualFunction(const gsl_vector *, void *, gsl_vector *);

            void RenderInPlot();

        private:
            ImVec2 m_size{434, 284};

            size_t ncurves = 2;
            int param1 = 6, param2 = 6;
            int poly1 = 1, poly2 = 1;

            bool b_renderplot = false, b_function = false;

            std::shared_ptr<std::function<void()>> PlotRender_func;

            std::vector<FullDataCurve> curves;

            std::vector<std::map<int, int>> raw_points{
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
                {// CV
                 {122, 173},
                 {143, 170},
                 {159, 168},
                 {177, 163},
                 {193, 156},
                 {202, 153},
                 {211, 149},
                 {228, 141},
                 {242, 133},
                 {262, 120},
                 {277, 108},
                 {293, 94},
                 {300, 87},
                 {310, 76},
                 {314, 71},
                 {319, 65},
                 {327, 54},
                 {330, 51},
                 {336, 43},
                 {344, 32},
                 {350, 21},
                 {356, 13},
                 {358, 10},
                 {363, 1}}
                 //{ //1.2 Qopt
                 //   {146, 283},
                 //   {159, 282},
                 //   {172, 281},
                 //   {184, 280},
                 //   {193, 279},
                 //   {202, 278},
                 //   {211, 276},
                 //   {238, 271},
                 //   {241, 270},
                 //   {262, 266},
                 //   {276, 262},
                 //   {291, 257},
                 //   {313, 248},
                 //   {330, 239},
                 //   {344, 229},
                 //   {355, 221},
                 //   {361, 217},
                 //   {364, 213},
                 //   {373, 205},
                 //   {381, 199},
                 //   {382, 198}
                 //}    
                 };
        };

    } // namespace Debug

} // namespace ViscoCorrect
