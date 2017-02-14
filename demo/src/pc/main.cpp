#include "Ic3d.h"

int main(int argc, char *argv[])
{
    auto& app = Ic3d::getIcAppInstance();
    app.runCmd(argc, argv);
}
