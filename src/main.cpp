#include "application.hpp"

int main(int argc, char *argv[])
{
    auto gravitySimApplication = GravitySimApplication();

    gravitySimApplication.run();

    return 0;
}
