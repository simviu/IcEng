#include "DemoWindow.hpp"

int main(int argc, char *argv[])
{
    std::string sPathRes = "";
    auto pWin = ctl::makeSp<DemoWindow>(sPathRes);
     //---- Choose demo
    int demoSel = 0;
    if(argc>1)
    {
        std::string s(argv[1]);
        demoSel = s[0] - '0';
        pWin->setDemoSel(demoSel);
    }
    Ic3d::IcWindow::runCmd(argc, argv, pWin);
}
