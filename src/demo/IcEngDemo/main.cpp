#include "DemoApp.hpp"

int main(int argc, char *argv[])
{
    //---- Instance of our app
    DemoApp app;
    
    //---- Pass optional para
    if(argc>1)
    {
        int demoSel=0;
        ctl::s2v(argv[1], demoSel);
        app.reqSetDemo(demoSel);
    }
    
    
    //---- Run Command Line, GLUT option is supported.
    //  Also the res data path is provided.
    app.runCmdLine(argc, argv, "../../../../../IcData/");
}
