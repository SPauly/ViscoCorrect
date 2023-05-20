#include "CurveFitting.h"
#include "Application.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        CurveFitting::CurveFitting()
        {
            // populate x and y Data
            for (const auto &pair : data)
            {
                xData.push_back(pair.first);
                yData.push_back(pair.second);
            }
            compressed_data.dataSize = xData.size();
            compressed_data.xData = xData.data();
            compressed_data.yData = yData.data();

            PlotRender_func = std::make_shared<std::function<void()>>(std::bind(RenderInPlot, this));
        }

        CurveFitting::~CurveFitting()
        {
        }

        void CurveFitting::Render()
        {
            ImGui::Begin("Curve Fitting");
            if (ImGui::Button("Enable Render") && !b_renderplot)
            {
                Application::GetInstance()->GetGraph()->AddCallbackToPlot(PlotRender_func, 1);
                b_renderplot = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Disable Render") && b_renderplot)
            {
                Application::GetInstance()->GetGraph()->RemoveCallbackFromPlot(PlotRender_func, 1);
                b_renderplot = false;
            }

            if (ImGui::Button("Approximate Curve"))
            {
                FitCurve();
                for(int i = xData.at(0); i < xData.back(); i++)
                {
                    fittedX.push_back(i);
                    fittedY.push_back(a * i * i + b * i + c);
                }
                b_function = true;
            }
            ImGui::Text("Approximated Function: %.20f * x^2 + %.20f * x + %.20f", a, b, c);
            ImGui::End();
        }

        void CurveFitting::RenderInPlot()
        {
            ImPlot::PlotScatter("CQ", xData.data(), yData.data(), xData.size());
            ImPlot::PlotLine("fit", fittedX.data(), fittedY.data(), fittedX.size());
        }

        // Curve Fitting


        int CurveFitting::ResidualFunction(const gsl_vector *x, void *params, gsl_vector *f)
        {
            CompressedCurveData *_data = static_cast<CompressedCurveData *>(params);

            for (size_t i = 0; i < _data->dataSize; i++)
            {
                double _yModel = _data->ModelFunction(_data->xData[i], x);
                gsl_vector_set(f, i, _data->yData[i] - _yModel);
            }

            return GSL_SUCCESS;
        }

        void CurveFitting::FitCurve()
        {
            size_t _datasize = xData.size();

            gsl_vector *xVec = gsl_vector_alloc(_datasize);
            gsl_vector *yVec = gsl_vector_alloc(_datasize);

            for (size_t i = 0; i < _datasize; i++)
            {
                gsl_vector_set(xVec, i, xData.at(i));
                gsl_vector_set(yVec, i, yData.at(i));
            }

            gsl_vector *_initial_params = gsl_vector_alloc(num_parameters);

            gsl_vector_set(_initial_params, 0, 1.0);
            gsl_vector_set(_initial_params, 1, 1.0);
            gsl_vector_set(_initial_params, 2, 1.0);

            gsl_multifit_nlinear_parameters fittingParams = gsl_multifit_nlinear_default_parameters();
            gsl_multifit_nlinear_workspace *workspace = gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &fittingParams, _datasize, num_parameters);

            gsl_multifit_nlinear_fdf fittingFunction;
            fittingFunction.f = &ResidualFunction;
            fittingFunction.df = nullptr;  // Set to nullptr for numerical derivatives
            fittingFunction.fvv = nullptr; // Set to nullptr for numerical derivatives
            fittingFunction.n = _datasize;
            fittingFunction.p = num_parameters;
            fittingFunction.params = static_cast<void *>(&compressed_data);

            int info;

            gsl_multifit_nlinear_init(_initial_params, &fittingFunction, workspace);

            if (gsl_multifit_nlinear_driver(100, 1e-6, 1e-6, 1e-6, nullptr, nullptr, &info, workspace) != GSL_SUCCESS)
            {
                gsl_vector_free(xVec);
                gsl_vector_free(yVec);
                gsl_vector_free(_initial_params);
                gsl_multifit_nlinear_free(workspace);
                return;
            }

            // Retrieve the optimized parameter values
            gsl_vector *solution = gsl_multifit_nlinear_position(workspace);
            a = gsl_vector_get(solution, 0);
            b = gsl_vector_get(solution, 1);
            c = gsl_vector_get(solution, 2);

            gsl_vector_free(xVec);
            gsl_vector_free(yVec);
            gsl_vector_free(_initial_params);
            gsl_multifit_nlinear_free(workspace);
        }
    } // namespace Debug

} // namespace ViscoCorrect
