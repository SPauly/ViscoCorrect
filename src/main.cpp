#include "Application.h"


int main(){
    viscocorrect::Application *app = viscocorrect::CreateApplication();
    app->Run();
    delete app;
    return 0;
}