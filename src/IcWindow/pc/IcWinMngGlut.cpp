#include "Ic3d.h"

#define GLUT_STATIC
#if __APPLE__
	#include "GLUT/glut.h"
#else	// Windows
    #include "GL/glew.h"
    #include "GL/glut.h"
#endif	// __APPLE__


namespace Ic3d
{
    using namespace ctl;
	using namespace std;
    using namespace Ic3d;

    
	const static TRect  K_dfltWinRect(10,10,800,500);
	const static TSize  K_dfltWinSizeScl(0.8, 0.8);
	const static TPos   K_dfltWinPosScl(0.05, 0.05);
    const static float K_initDeltaT = 0.01;
    #define N_MAX_GLUT_WIN 8
    
    //---- Glut Key
    typedef unsigned char TKey;
    const static TKey K_key_ESC = 27;
   
    //----------------------------
    //  Glut Helper
    //----------------------------
    struct CGlutHelper
    {
        //--------------------------
        //  TWin
        //--------------------------
        struct TWin
        {
            TWin(ctl::Sp<IcWindow> pIcWin)
            :m_pIcWin(pIcWin)
            {
            };
            //---- onDrawFrame()
            void onDrawFrame()
            {
                if(m_pIcWin==nullptr) return;
                
                unsigned long tCur = glutGet(GLUT_ELAPSED_TIME);
                unsigned long dtI = tCur - m_time;
                float deltaT = K_initDeltaT;
                if(m_time!=0) deltaT = (float)dtI/1000.0f;
                m_pIcWin->onDrawUpdate(deltaT);
                m_time = tCur;
                // in double buffer mode so we swap to avoid a flicker
                glutSwapBuffers();
                
                // instruct event system to call 'drawfunc' again
                glutPostRedisplay();
            };
            //---- onReshape()
            void onReshape(int w, int h)
            {
                if(m_pIcWin==nullptr) return;
                if(!m_hasInit)
				{
	                m_pIcWin->onInit();				
	                m_hasInit = true;	
				}

                m_pIcWin->onWindowSize(TSize(w,h));
            };
            //---- onKeyboard()
            void onKeyboard(unsigned char k)
            {
                if(m_pIcWin==nullptr) return;
                m_pIcWin->onKeyboard(k);
            };
        protected:
            ctl::Sp<IcWindow> m_pIcWin = nullptr;
            unsigned long m_time = 0;
            bool m_hasInit = false;
        };
        //--------------------------
        //  getScreen
        //--------------------------
        ctl::TSize getScreenSize()
        {
            auto w = glutGet(GLUT_SCREEN_WIDTH);
            auto h = glutGet(GLUT_SCREEN_HEIGHT);
            return TSize(w,h);
        };
        //--------------------------
        //  callBk Display
        //--------------------------
        void cbk_disp(int wid)
        {
            auto pWin = findWindow(wid);
            if(pWin!=nullptr) pWin->onDrawFrame();
        };
        //--------------------------
        //  callBk reshape
        //--------------------------
        void cbk_rshp(int wid, GLint w, GLint h)
        {
	    //---- Check init glew
	    if(!m_hasGlewInit)
	    {
		initGlew();
		m_hasGlewInit = true;
	    }
            //---- Reshape Window
            auto pWin = findWindow(wid);
            if(pWin==nullptr) return;
            pWin->onReshape(w,h);
        };
        //--------------------------
        //  callBk on keyboard
        //--------------------------
        void cbk_keyb(int wid, unsigned char key, int x, int y)
        {
            auto pWin = findWindow(wid);
            if(pWin!=nullptr) pWin->onKeyboard(key);
            
            const auto& cfg = IcWinMng::getInstance()->m_cfg;
            switch(key)
            {
                case K_key_ESC:
                    if(cfg.m_quitOnKeyEsc)
                        onQuit();
                    break;
                default:break;
            }
        };
        
        //--------------------------
        //  checkInitGlut
        //--------------------------
		bool onInit(int argc, char *argv[])
		{
            		logInfo("Init Glut");

			if (m_hasGlutInit) return m_valid;
			m_valid = true;

			//------------------------
			//  Init Glut
			//------------------------
			// init GLUT and create Window
			logInfo("Init Glut...");
			glutInit(&argc, argv);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

			//---- Check Screen Size
			TRect winRect = K_dfltWinRect;
			unsigned int w = glutGet(GLUT_SCREEN_WIDTH);
			unsigned int h = glutGet(GLUT_SCREEN_HEIGHT);
			if (w != 0 && h != 0)
			{
				auto sz = K_dfltWinSizeScl;
				auto ps = K_dfltWinPosScl;
				sz.w *= w; sz.h *= h;
				ps.x *= w; ps.y *= h;
				winRect = TRect(ps, sz);
			}
			return m_valid;
		};
            
        //------------------------
        //  Init Glew
        //------------------------
		bool initGlew()
		{
#if __APPLE__
#else
            logInfo("Init Glew");
            auto err = glewInit();
            if (GLEW_OK != err)
            {
                // failed to initialize GLEW!
                ctl::logErr("Failed to init glew :" +
                            string((char*)glewGetErrorString(err)));
                m_valid = false;
                return false;
            }
            logInfo("Glew Init OK");
            string sVer((char*)glewGetString(GLEW_VERSION));
            ctl::logInfo("Using GLEW Version: "+sVer);
            
            if(GLEW_VERSION_1_3)
                logInfo("OpenGL 1.3 Supported");
            else
            {
                logErr("OpenGL 1.3 Not supported");
                m_valid = false;
                return false;
            }
#endif
            m_hasGlutInit = true;
            return m_valid;
        };
        
        //---- Statically matain glut windows
        size_t getWindowNum() { return m_winSet.size(); };
        ctl::Sp<TWin> findWindow(int winId)
        {
            auto p = m_winSet.find(winId);
            if(p==m_winSet.end()) return nullptr;
            return p->second;
        } ;
        ctl::Sp<IcWindow> createGlutWin(ctl::Sp<IcWindow> pIcWin,
                                        const std::string sTitle,
                                        const ctl::TRect& rect);
        void addWindow(ctl::Sp<TWin> pWin, int wid)
            { m_winSet[wid] = pWin; };

        //------------------------
        //  onQuit
        //------------------------
        void onQuit()
        {
            m_winSet.clear();
            exit(0);
        };
    private:
        bool m_hasGlutInit = false;
	bool m_hasGlewInit = false;
        bool m_valid = false;
        std::map<int, ctl::Sp<TWin>> m_winSet;
       
    };
    static CGlutHelper l_glutHelper;
    //--------------------------
    //  Call Back Func Array
    //--------------------------
     // TODO: Maybe call back on C++ member function is doable.
    namespace TGlutCallBk{
        CGlutHelper& glt = l_glutHelper;
        //-----------------
        // CallBk display()
        //-----------------
        static void cbk_disp()
        { int wid = glutGetWindow(); glt.cbk_disp(wid); }
        //-----------------
        // CallBk reshape()
        //-----------------
        static void cbk_rshp(int w, int h)
        { int wid = glutGetWindow(); glt.cbk_rshp(wid, w, h); }
        //-----------------
        // CallBk keyboard()
        //-----------------
        static void cbk_keyb(unsigned char k, int x, int y)
        { int wid = glutGetWindow(); glt.cbk_keyb(wid, k,x,y); }
       
    };
    //--------------------------
    //  CGlutHelper::createWindow
    //--------------------------
    Sp<IcWindow> CGlutHelper::createGlutWin(ctl::Sp<IcWindow> pIcWin,
                                            const std::string sTitle,
                                            const ctl::TRect& rect)
    {
        
        auto pos = rect.pos0;
        auto wsz = rect.getSize();
        glutInitWindowPosition(pos.x, pos.y);
        glutInitWindowSize(wsz.w, wsz.h);
        int wid = glutCreateWindow(sTitle.c_str());
        glutSetWindowTitle(sTitle.c_str());
        glutDisplayFunc     (TGlutCallBk::cbk_disp);
        glutReshapeFunc     (TGlutCallBk::cbk_rshp);
        glutKeyboardFunc    (TGlutCallBk::cbk_keyb);

        auto pWin = makeSp<TWin>(pIcWin);
        addWindow(pWin, wid);
        return pIcWin;
    };
   
    //----------------------------
    //  IcWinMngGlut
    //----------------------------
    class IcWinMngGlut : public IcWinMng
    {
    public:
        
        //---- This is only available on GLUT for PC
        virtual bool initMng(int argc, char **argv) override;
        virtual void startMainLoop() override;
        virtual void addWindow(ctl::Sp<IcWindow> pWin) override;
        //---- No need to drawUpdate windows, as Glut will
        // automatically call display function for each window.
        // So this override do nothing.
        virtual void drawUpdate(float deltaT) override{};
    
    };
    //-------------------------------------------
    //	onInit
    //-------------------------------------------
    bool IcWinMngGlut::initMng(int argc, char *argv[])
    {
        bool isOK =  l_glutHelper.onInit(argc, argv);
        m_screenSize = l_glutHelper.getScreenSize();
        
        //---- Create Window after glut init
        for(auto pWin : m_winAry.getAry())
        {
            ctl::TRect rect(TPos(0,0), m_screenSize);
            l_glutHelper.createGlutWin(pWin, "", rect);
        }
        
        return isOK;
    }
    
    //----------------------------
    //  startMainLoop
    //----------------------------
    void IcWinMngGlut::startMainLoop()
    {
        logInfo("Glut Main Loop Start...");
        glutMainLoop();
    }
    //-------------------------------------------
    //	addWindow
    //-------------------------------------------
    void IcWinMngGlut::addWindow(ctl::Sp<IcWindow> pWin)
    {
        IcWinMng::addWindow(pWin);
    }
    //----------------------------
    //  Implementation of Glut Win Mng
    //----------------------------
    ctl::Sp<IcWinMng> IcWinMng::createWinMngImpl()
    {
        return ctl::makeSp<IcWinMngGlut>();
    }
 }


