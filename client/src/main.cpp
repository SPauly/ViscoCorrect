#include "Application.h"


int main(){
    Your_Project::Application *app = Your_Project::CreateApplication();
    app->Run();
    delete app;
    return 0;
}