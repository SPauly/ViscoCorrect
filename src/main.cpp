#include "Application.h"


int main(){
    ViscoCorrect::Application *app = ViscoCorrect::CreateApplication();
    app->Run();
    delete app;
    return 0;
}