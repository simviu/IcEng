#include "DemoApp.hpp"

int main(int argc, char *argv[])
{
    DemoApp app;
    app.m_cfg.m_sPathRes = "../../../../../IcData/";
    app.runCmdLine(argc, argv);
}
