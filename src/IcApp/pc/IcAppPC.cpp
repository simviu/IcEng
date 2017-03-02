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
    //  GlewHelper
    //----------------------------
    namespace GlewHelper
    {
        static bool m_hasInit = false;
        static bool m_valid = true;
        //------------------------
        //  Init Glew
        //------------------------
        bool checkInitGlew()
        {
            if(m_hasInit) return m_valid;
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
            m_hasInit = true;
#endif
            return m_valid;
       };
      
        
    };
   
    //----------------------------
    //  Glut Helper
    //----------------------------
    namespace GlutHelper
    {
        struct TWinSlot;
        //--------------------------
        //  static var
        //--------------------------
        bool m_hasInit = false;
        bool m_valid = false;
        std::map<int, ctl::Sp<TWinSlot>> m_winSlotAry;
        //--------------------------
        //  TWinSlot
        //--------------------------
        struct TWinSlot
        {
            TWinSlot(ctl::Sp<IcWindow> pIcWin)
            :m_pIcWin(pIcWin) { };
            //----- Update DeltaT
            float updateDeltaT()
            {
                unsigned long tCur = glutGet(GLUT_ELAPSED_TIME);
                unsigned long dtI = tCur - m_time;
                float deltaT = K_initDeltaT;
                if(m_time!=0) deltaT = (float)dtI/1000.0f;
                m_time = tCur;
                return deltaT;
            };
            Sp<IcWindow> getIcWin() { return m_pIcWin; };
            bool m_hasInit = false;
        protected:
            ctl::Sp<IcWindow> m_pIcWin = nullptr;
            unsigned long m_time = 0;
        };
        //--------------------------
        //  getCurWinSlot
        //--------------------------
        Sp<TWinSlot> getCurWinSlot()
        {
            int wid = glutGetWindow();
            auto p = m_winSlotAry.find(wid);
            if(p==m_winSlotAry.end()) return nullptr;
            return p->second;
        } ;
        //--------------------------
        //  getCurWin
        //--------------------------
        Sp<IcWindow> getCurWin()
        {
            int wid = glutGetWindow();
            auto p = m_winSlotAry.find(wid);
            if(p==m_winSlotAry.end()) return nullptr;
            return (p->second)->getIcWin();
        } ;
      

        //--------------------------
        //  getScreen
        //--------------------------
        ctl::TSize getScreenSize()
        {
            auto w = glutGet(GLUT_SCREEN_WIDTH);
            auto h = glutGet(GLUT_SCREEN_HEIGHT);
            return TSize(w,h);
        };
        //------------------------
        //  onQuit
        //------------------------
        void onQuit()
        {
            // TODO: call request release on IcWinMng
            m_winSlotAry.clear();
            IcApp::getInstance()->getWinMng()->releaseWindows();
            IcApp::getInstance()->onRelease();
            exit(0);
        };

        //--------------------------
        //  callBk Display
        //--------------------------
        void cbk_display()
        {
            auto pSlot = getCurWinSlot();
            if(pSlot==nullptr) return;
            float deltaT = pSlot->updateDeltaT();
            auto pIcWin = pSlot->getIcWin();
            if(pIcWin==nullptr) return;
            pIcWin->onDrawUpdate(deltaT);
            // in double buffer mode so we swap to avoid a flicker
            glutSwapBuffers();
            
            // instruct event system to call 'drawfunc' again
            glutPostRedisplay();
        };
        //--------------------------
        //  callBk reshape
        //--------------------------
        void cbk_reshape(GLint w, GLint h)
        {
            //---- Check init glew
            GlewHelper::checkInitGlew();
 
            //---- Reshape Window
            auto pSlot = getCurWinSlot();
            if(pSlot==nullptr) return;
            auto pIcWin = pSlot->getIcWin();
            if(pIcWin==nullptr) return;
            if(!pSlot->m_hasInit)
            {
             //   pIcWin->reqInit();
                pSlot->m_hasInit = true;
            }
            
            pIcWin->onWindowSize(TSize(w,h));
        };
        //--------------------------
        //  callBk on keyboard
        //--------------------------
        void cbk_keyboard(unsigned char key, int x, int y)
        {
            auto pIcWin = getCurWin();
            if(pIcWin==nullptr) return;
            pIcWin->onKeyboard(key);
            
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
        //  callBk on mouseMove
        //--------------------------
        void cbk_mouseMove(int x, int y)
        {
            auto pIcWin = getCurWin();
            if(pIcWin==nullptr) return;
            pIcWin->onMouseMove(x, y);
        }
        //--------------------------
        //  callBk on mouseClick
        //--------------------------
        void cbk_mouseClick(int glutButton, int state, int x, int y)
        {
            auto pIcWin = getCurWin();
            if(pIcWin==nullptr) return;
            //---- translate to IcWindow Mouse Button
            typedef IcWindow::TE_MouseButton TE_type;
            TE_type type =
                (glutButton==GLUT_LEFT_BUTTON)  ?TE_type::L :
                (glutButton==GLUT_RIGHT_BUTTON) ?TE_type::R :
                (glutButton==GLUT_MIDDLE_BUTTON)?TE_type::M :
                TE_type::NONE;
            pIcWin->onMouseClick(type, (state==GLUT_UP), x, y);
        }
        
        //--------------------------
        //  checkInitGlut
        //--------------------------
		bool onInit(int argc, char *argv[])
		{
			if (m_hasInit) return m_valid;
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
            
        //---- Statically matain glut windows
        size_t getWindowNum() { return m_winSlotAry.size(); };
        void addWindow(ctl::Sp<TWinSlot> pSlot, int wid)
        { m_winSlotAry[wid] = pSlot; };
        
        //------------------------
        //  createGlutWin
        //------------------------
        ctl::Sp<IcWindow> createGlutWin(ctl::Sp<IcWindow> pIcWin,
                                        const std::string sTitle,
                                        const ctl::TRect& rect)
        {
            auto pos = rect.pos0;
            auto wsz = rect.getSize();
            glutInitWindowPosition(pos.x, pos.y);
            glutInitWindowSize(wsz.w, wsz.h);
            int wid = glutCreateWindow(sTitle.c_str());
            glutSetWindowTitle(sTitle.c_str());
            glutDisplayFunc     (cbk_display);
            glutReshapeFunc     (cbk_reshape);
            glutKeyboardFunc    (cbk_keyboard);
            glutMouseFunc          (cbk_mouseClick);
            glutPassiveMotionFunc   (cbk_mouseMove);
           
            auto pWin = makeSp<TWinSlot>(pIcWin);
            addWindow(pWin, wid);
            return pIcWin;
            
        };
        
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
        bool isOK =  GlutHelper::onInit(argc, argv);
        /* This was post creation, absoleted
        m_screenSize = GlutHelper::getScreenSize();
        
        //---- Create Window after glut init
        for(auto pWin : m_winAry.getAry())
        {
            ctl::TRect rect(TPos(0,0), m_screenSize);
            GlutHelper::createGlutWin(pWin, "", rect);
        }
        */
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
        m_screenSize = GlutHelper::getScreenSize();
        
        ctl::TRect rect(TPos(0,0), m_screenSize);
        GlutHelper::createGlutWin(pWin, "", rect);
       
    }
 
    //----------------------------------
    //  IcApp::runCmdLine
    //----------------------------------
    int IcApp::runCmdLine(int argc, char **argv)
    {
        setInstance(this);
        auto pMng = ctl::makeSp<IcWinMngGlut>();
        IcWinMng::setInstance(pMng);
        pMng->initMng(argc, argv);
        onInit();
        pMng->initWindows();
        pMng->startMainLoop();
        return true;
    }

 }


