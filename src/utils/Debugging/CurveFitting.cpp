#include "CurveFitting.h"
#include "Application.h"

namespace ViscoCorrect
{
    namespace Debug
    {
        // FullDataCurve
        FullDataCurve::FullDataCurve(std::map<int, int> &_data, bool _bpoly = true, int _params = 3)
        {
            //populate x and y Data
            for (const auto &pair : _data)
            {
                xData.push_back(pair.first);
                yData.push_back(pair.second);
            }
            compressed_data.dataSize = xData.size();
            compressed_data.xData = xData.data();
            compressed_data.yData = yData.data();

            if(_bpoly)
            {
                compressed_data.ModelFunction = std::bind(CompressedCurveData::ModelFunctionPolynom, &compressed_data);
                parameters.resize(_params);
            }
        }

        CurveFitting::CurveFitting()
        {
            // reserve space for x and y data
            ncurves = datas.size();

            xData.resize(ncurves);
            yData.resize(ncurves);
            a.resize(ncurves);
            b.resize(ncurves);
            c.resize(ncurves);
            fittedX.resize(ncurves);
            fittedY.resize(ncurves);
            compressed_data.resize(ncurves);

            // populate x and y Data
            for (int i = 0; i < ncurves; i++)
            {
                for (const auto &pair : datas.at(i))
                {
                    xData.at(i).push_back(pair.first);
                    yData.at(i).push_back(pair.second);
                }
                compressed_data.at(i).dataSize = xData.at(i).size();
                compressed_data.at(i).xData = xData.at(i).data();
                compressed_data.at(i).yData = yData.at(i).data();
            }

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

            if (ImGui::Button("Approximate Curves") && !b_function)
            {
                for (int i = 0; i < ncurves; i++)
                {
                    FitCurve(datas.at(i), i);
                }
                b_function = true;
            }

            if (ImGui::Button("Delete Curves") && b_function)
            {
            }
            ImGui::Text("Approximated Function Q: %.20f * x^2 + %.20f * x + %.20f", a.at(0), b.at(0), c.at(0));
            ImGui::Text("Approximated Function V: %.20f * x^2 + %.20f * x + %.20f", a.at(1), b.at(1), c.at(1));
            ImGui::End();
        }

        void CurveFitting::RenderInPlot()
        {
            for (int i = 0; i < xData.size(); i++)
            {
                ImPlot::PlotScatter("datas", xData.at(i).data(), yData.at(i).data(), xData.at(i).size());
                ImPlot::PlotLine("fitted", fittedX.at(i).data(), fittedY.at(i).data(), fittedX.at(i).size());
            }
        }

        // Curve Fitting

        int CurveFitting::ResidualFunction(const gsl_vector *x, void *params, gsl_vector *f)
        {
            CompressedCurveData *_data = static_cast<CompressedCurveData *>(params);

            for (size_t i = 0; i < _data->dataSize; i++)
            {
                double _yModel = _data->ModelFunctionLogistic(_data->xData[i], x);
                gsl_vector_set(f, i, _data->yData[i] - _yModel);
            }

            return GSL_SUCCESS;
        }

        void CurveFitting::FitCurve(std::map<int, int> _data, int _iter)
        {
            size_t _datasize = xData.at(_iter).size();

            gsl_vector *xVec = gsl_vector_alloc(_datasize);
            gsl_vector *yVec = gsl_vector_alloc(_datasize);

            for (size_t i = 0; i < _datasize; i++)
            {
                gsl_vector_set(xVec, i, xData.at(_iter).at(i));
                gsl_vector_set(yVec, i, yData.at(_iter).at(i));
            }

            gsl_vector *_initial_params = gsl_vector_alloc(num_parameters);

            gsl_vector_set(_initial_params, 0, 174.0);
            gsl_vector_set(_initial_params, 1, 0.1);
            gsl_vector_set(_initial_params, 2, 300);

            gsl_multifit_nlinear_parameters fittingParams = gsl_multifit_nlinear_default_parameters();
            gsl_multifit_nlinear_workspace *workspace = gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &fittingParams, _datasize, num_parameters);

            gsl_multifit_nlinear_fdf fittingFunction;
            fittingFunction.f = &ResidualFunction;
            fittingFunction.df = nullptr;  // Set to nullptr for numerical derivatives
            fittingFunction.fvv = nullptr; // Set to nullptr for numerical derivatives
            fittingFunction.n = _datasize;
            fittingFunction.p = num_parameters;
            fittingFunction.params = static_cast<void *>(&compressed_data.at(_iter));

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
            a.at(_iter) = gsl_vector_get(solution, 0);
            b.at(_iter) = gsl_vector_get(solution, 1);
            c.at(_iter) = gsl_vector_get(solution, 2);

            gsl_vector_free(xVec);
            gsl_vector_free(yVec);
            gsl_vector_free(_initial_params);
            gsl_multifit_nlinear_free(workspace);

            // Produce fitted data
            for (int i = xData.at(_iter).at(0); i < xData.at(_iter).back(); i++)
            {
                fittedX.at(_iter).push_back(i);
                // fittedY.at(_iter).push_back(a.at(_iter) * i * i + b.at(_iter) * i + c.at(_iter)); //ax2+bx+c
                fittedY.at(_iter).push_back(a.at(_iter) / (1 + exp(-b.at(_iter) * (i - c.at(_iter))))); // L /(1+ exp(-k*(_x-x0)))
            }
        }
    } // namespace Debug

} // namespace ViscoCorrect
