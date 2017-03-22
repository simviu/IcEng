//
//  Ic3d.h
//  DevEng
//
//  Created by Sherman Chen on 3/11/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#ifndef _ICUBE_H
#define _ICUBE_H

#include "IcRenderAdp.h"
#include <atomic>

//---- TODO: Ic3d->IcEng
namespace Ic3d {
   	//-----------------------------------------------
    //	Util
    //-----------------------------------------------
    extern bool s2vec(const std::string& s, TVec2& d);
    extern bool s2vec(const std::string& s, TVec3& d);
    extern bool s2vec(const std::string& s, TVec4& d);
    extern bool s2quat(const std::string& s, TQuat& c);
    extern bool s2color(const std::string& s, TColor& d);
    
    extern std::string toStr(const TVec2& v);
    extern std::string toStr(const TVec3& v);
    extern std::string toStr(const TVec4& v);
    extern std::string toStr(const TQuat& v);
    extern std::string toStr(const TColor& v);
 
    TQuat quatFromVecs(const TVec3& v0, const TVec3& v1);
    //-----------------------------------------------
    //	IcMeshData
    //-----------------------------------------------
    class IcMeshData : public TMeshData
    {
    public:
        IcMeshData(){};
        void getSubMesh(TMeshData& mesh,
                        size_t faceStrt, size_t N) const;
       
        void createPlaneXZ(const ctl::TRect& rect,
                           const ctl::TRect& texRect = {0,0,1,1});
        void createCube(const TVec3& sz, const TVec3& ofst);
        void createSphere(float R, int N_stack, int N_slice);
        void createCylinder(float R, float height); // TODO : Implement
        void createCone(float R, float height); // TODO : Implement
        void createGridXZ(const ctl::TRect& rect,
                          int N_x, int N_y,
                          const ctl::TRect& texRect = ctl::TRect(0,0,1,1));
        
        void dbgPrint() const;
        //---- Cfg
        struct TCfg
        {
            // Counter clock wise winding, otherwise CR
            bool m_isWindingCCR = true;
        };
        TCfg m_cfg;
        
    };
	//-----------------------------------------------
	//	IcMaterial
	//-----------------------------------------------
	class IcMaterial : public TMaterial
	{
	public:
        IcMaterial(){}
        IcMaterial(const TColor& c){ setColor(c, 0.4, 0.1); };
		std::string m_sName;
		void draw() const;
	};
	

 	//-----------------------------------------
	//	IcTexture
	//-----------------------------------------
	class IcTexture
	{
	public:
		bool isValid()const
		{	if(m_pRenderAdp==nullptr) return false;
			return m_pRenderAdp->isValid(); };
		IcTexture();
        IcTexture(const ctl::TSize& sz);
        IcTexture(const std::string& sFile){ loadFile(sFile); };
		IcTexture(const ctl::IcImg& img);
		virtual ~IcTexture(){};
		virtual void draw() const
		{	 if(m_pRenderAdp!=nullptr)
				m_pRenderAdp->render(); };
        bool loadFile(const std::string& fname);
        ctl::TSize getSize() const;

        //---- Render Texture
        bool setAsRenderTarget();
        void startRenderOn() const;
        void finishRenderOn() const;
	protected:
		ctl::Sp<CRenderAdp::CTexAdp>	m_pRenderAdp = nullptr;
		bool m_isValid = false;
	};
 
    //-----------------------------------------
	//	IcMesh
	//-----------------------------------------
	// A Mesh is part of model (sub-object) with
	// it's own material and texture.
	// A model usually contain one or more Mesh.
	class IcMesh
	{
	public:
		IcMesh(const TMeshData& rMshd);
		IcMesh(){};
		virtual ~IcMesh	(){};
		std::string name;
		void draw() const;
		
	protected:
		
		ctl::Sp<const CRenderAdp::CMeshAdp>		m_renderAdp = nullptr;

		
	};
	//-----------------------------------------
	//	IcModel
	//-----------------------------------------
	class IcModel
	{
	protected:
		ctl::SpAry<const IcModel>	m_childModels;
        ctl::Sp<const IcMesh>		m_pMesh = ctl::makeSp<IcMesh>();
        ctl::Sp<const IcMaterial>	m_pMat  = ctl::makeSp<IcMaterial>();
        ctl::Sp<const IcTexture>	m_pTex  = ctl::makeSp<IcTexture>();

	public:
        IcModel(const IcMeshData& mshd):IcModel(ctl::makeSp<IcMesh>(mshd)){ };
        IcModel(ctl::Sp<const IcMesh> pMsh) { setMesh(pMsh); };
		IcModel(const std::string& sFile){ loadFile(sFile); };
		IcModel(){};
               
        bool loadFile(const std::string& sFile);
		virtual ~IcModel(){};
		
		std::string		m_sName;
		virtual	void draw() const;
		void addChildModel(ctl::Sp<const IcModel> p) { m_childModels.add(p); };
        
        //---- Mesh/Material/Texture setter/getter
        void setMshMtlTex(ctl::Sp<const IcMesh>       pMesh,
                          ctl::Sp<const IcTexture>    pTex,
                          ctl::Sp<const IcMaterial>   pMat)
            {m_pMesh=pMesh; m_pTex=pTex; m_pMat=pMat; };
		void setMesh        (ctl::Sp<const IcMesh> p        ){ m_pMesh = p; };
		void setMaterial    (ctl::Sp<const IcMaterial> p    ){ m_pMat = p;  };
		void setTexture     (ctl::Sp<const IcTexture> p     ){ m_pTex = p;  };
        ctl::Sp<const IcMesh>       getMesh()       const { return m_pMesh; };
        ctl::Sp<const IcMaterial>   getMaterial()   const { return m_pMat;  };
        ctl::Sp<const IcTexture>    getTexture()    const { return m_pTex;  };
        
        //---- dbg
        // create a 1x1x1 cubic model, with color
        static ctl::Sp<const IcModel> createCube(const TVec3& sz,
                                                    const TVec3& ofst,
                                                    const TColor& color);
        ctl::SpAry<const IcModel>& getChildModels() { return m_childModels; };
	};
    //-----------------------------------------
    //	IcModelLoader
    //-----------------------------------------
    class IcModelLoader
    {
    public:
        IcModelLoader(){};
        virtual ~IcModelLoader(){};
        bool loadFile(const std::string& sFile);
        ctl::SpAry<const IcModel> m_models;
        struct TModelInfo
        {
            size_t	m_N_face = 0;
        }m_info;
        
    protected:
        std::string		m_sPath;
        
        bool loadFileObj(const std::string& sFile);
        bool loadFileMtl(const std::string& sFile);
        
        //---- MTL Material is combination of Material and Texture
        struct TMtlItem
        {
            ctl::Sp<const IcMaterial>	m_pMat = ctl::makeSp<IcMaterial>();
            ctl::Sp<const IcTexture>	m_pTex = ctl::makeSp<IcTexture>();
        };
        ctl::SpMap<TMtlItem>	m_mtlLib;
        ctl::SpMap<IcTexture>	m_texLib;
        
        bool chkParaNum(size_t lnCnt, size_t paraNum, size_t paraNumExp);
        void createAddModel(const std::string& sName, const TMtlItem& mtl);
        ctl::Sp<IcTexture> checkLoadTex(const std::string& sFile);
        //---- The total mesh data
        IcMeshData	m_meshData;
        size_t		m_subMeshFaceStart=0;
        
    };
	//-----------------------------------------
	//	IcObject
	//-----------------------------------------
	class IcObject
	{
	protected:
		ctl::Sp<const IcModel> m_pModel = nullptr;
		TVec3	m_pos;
		TVec3	m_scale=TVec3(1,1,1);
		TQuat	m_quat;
		std::string	m_sName;
		ctl::SpAry<IcObject>	m_childObjs;
		
	public:
		IcObject(){};
		IcObject(ctl::Sp<const IcModel> p){ setModel(p); };
		virtual	~IcObject(){};

		//----
		inline void setModel(ctl::Sp<const IcModel> p) { m_pModel=p;};
		inline ctl::Sp<const IcModel>  getModel() const { return m_pModel; };
		virtual void draw() const;
        virtual void update(float deltaT){};
		
		//----Child Obj
		void addChildObj(ctl::Sp<IcObject> p){ m_childObjs.add(p); };
		const ctl::SpAry<IcObject>&
			getChildObjs()const{ return m_childObjs; };
		void clearChildObjs()
			{ m_childObjs.clear(); };
		//---- Transform
		inline void	setQuat(const TQuat& q) {m_quat=q;};
		inline void	setScale(const TVec3& scl) {m_scale=scl;};
		inline void setPos(const TVec3& ps) {m_pos=ps;};
		inline TVec3 getPos()const{ return m_pos; };
		inline TVec3 getScale()const{ return m_scale; };
		inline TQuat getQuat()const{ return m_quat; };
		TMat4 calcMat()const;

		//---- Name
		inline void setName(const std::string& sName) { m_sName = sName; };
		inline std::string getName()const{ return m_sName; };
		
	};
	//-----------------------------------------
	//	IcCamera
	//-----------------------------------------
	class IcCamera : public IcObject
	{
	public:
		IcCamera(){};
		virtual ~IcCamera(){};
		void drawObj(const IcObject& rootObj) const;
        // TODO: 1) let it auto, 2) Mat valid bit in IcObject::setPos/Quat/Scale
        void updateViewMat();
        //---- Camera Cfg
        struct TCfg
        {
            float   m_FOV   = 50; // in degree
            float   m_zNear = 0.1;
            float   m_zFar  = 1000;
        };
		void setFrustum(const ctl::TSize& viewSize,
						const TCfg& cfg);
		void lookAt(const TVec3& pos,
					const TVec3& vUp);
		void drawText(const std::string& str,
					  const TColor& c,
					  const TVec3& pos);
        TMat4   getViewMat()const { return m_matView; };
        TMat4   getProjMat()const { return m_matProj; };
 	protected:
		TMat4	m_matProj;
		TMat4	m_matView;
		void drawObjTree(const IcObject& obj,
						 const TMat4& matModelParent) const;
	};
    //-----------------------------------------
    //	IcCameraVR
    //-----------------------------------------
    class IcCameraVR : public IcCamera
    {
    public:
        using IcCamera::IcCamera;
        
    };

    //-------------------------------------
    //	IcLight
    //-------------------------------------
    class IcLight : public IcObject
    {
    public:
        const TVec3 K_dfltPointAt = TVec3(-1,-1,-1);
        IcLight(){ onInit(); };
        IcLight(int lightId):m_lightId(lightId){ onInit(); };
        
        int getLightId()const{ return m_lightId; };
        void setPara(const TLight& para){ m_para = para; };
        TLight& getPara(){ return m_para; };
        TVec3 getDir() const;
        void setSpotPara(float Kd2, float coneAngle, float cutOff);
        void setLightColor(const TColor& c);
        void pointAt(const TVec3& pos);
        
    protected:
        int     m_lightId = 0;
        TLight  m_para;
        
    private:
        void onInit(){ pointAt(K_dfltPointAt); };
    };
    //-----------------------------------------
    //	IcText
    //-----------------------------------------
    // Text implementation
    class IcText
    {
    public:
        //---- TFont
        struct TFont
        {
            std::string     m_sName;
            ctl::TSize           m_size;
            int             m_fontSize=10;
            // equal to m_size.h
        };
        IcText(){};
        IcText(const std::string& sText="",
               const TVec2& pos=TVec2(0,0),
               const TColor& c=TColor(1,1,1,1),
               int fontSize=10,
               ctl::Sp<const TFont> pFont = nullptr ):
            m_pFont(pFont),m_sText(sText), m_color(c),
            m_pos(pos), m_fontSize(fontSize){};
        virtual ~IcText() {};
        
        ctl::Sp<const TFont> m_pFont = nullptr;
        std::string     m_sText;  // Font Text
        TColor  m_color{1,1,1,1};
        TVec2   m_pos;
        int     m_fontSize=10;
        //---- Static sys functions
        static ctl::SpAry<const TFont>& getAvailableFonts();
        virtual void onDraw() const;
    };
    //-----------------------------------------
	//	IcScene
	//-----------------------------------------
	class IcScene
	{
	public:
		IcScene();
		virtual ~IcScene() {};
        virtual void onUpdate(double deltaT)
        {    if(m_pCallBk_onUpdate!=nullptr) m_pCallBk_onUpdate(deltaT); };
		virtual void onDraw();
		virtual void onWindowSize(const ctl::TSize& winSize);
        virtual void renderThis();
        virtual void renderSubScns();
        void addObj(ctl::Sp<IcObject> p){ m_rootObj.addChildObj(p); };
        void addText(ctl::Sp<IcText> p){ m_texts.add(p); };
        void clearObjs(){ m_rootObj.clearChildObjs(); };
        ctl::Sp<IcCamera> getCamera(){ return m_pCamera; };
        ctl::SpAry<IcLight>& getLights(){ return m_lights; };
        void addLight(ctl::Sp<IcLight> pLight){ m_lights.add(pLight); };
        //---- Configuration
        struct TCfg
        {
            IcCamera::TCfg  m_camCfg;
            TFogPara    m_fogPara;
            bool        m_enClrScrn = true;
            TColor      m_bkColor{0.2,0.5,0.7,1.0};
            bool        m_enAutoResize = true;
            ctl::TRect  m_viewRect;
            ctl::TRect  m_viewRectNorm{0,0,1,1};    // Normalized view Rect
        };
        TCfg m_cfg;
        
        //---- On Update call back function
        typedef std::function<void(float deltaT)> TFuncOnUpdate;
        void setOnUpdatCallBack(TFuncOnUpdate func);
        void setRenderToTexture(ctl::Sp<IcTexture> pTex);
        void addSubScn(ctl::Sp<IcScene> pScn);
        void setViewRect(const ctl::TRect& r);
	protected:
		virtual void onInit(){};
        void renderObjRecur(const IcCamera& cam,
                            const IcObject& obj,
                            const TMat4& matModelParent) const;
        // TODO: Camera Ary
        ctl::Sp<IcCamera>	m_pCamera = ctl::makeSp<IcCamera>();
		IcObject	m_rootObj;
        ctl::SpAry<IcLight>		m_lights;
        ctl::SpAry<IcText>      m_texts;
        ctl::SpAry<IcScene>     m_subScns;
		size_t	m_frmCnt = 0;   // TODO: Move to IcWindow
        std::atomic<bool>	    m_hasInit{false};

        void drawLights();
	//	void initCamera(const ctl::TRect& viewRect);
        //---- On Update call back function
        TFuncOnUpdate m_pCallBk_onUpdate = nullptr;
        //---- TODO: Not implemented yet
        ctl::Sp<IcTexture> m_pTargetTex = nullptr;
        
	};
    
    //----------------------------
    // IcFrameRateCalc
    //----------------------------
    // TODO: Chrono not supported well on other platform
	/*
    struct IcFrameRateCalc
    {
        using TMs = std::chrono::microseconds;
        void onNewFrame();
        float getDeltaT()const { return m_deltaT; };
        
    protected:
        double m_deltaT =0;
        std::chrono::steady_clock::time_point m_tFrame =
        std::chrono::steady_clock::now();
    };
	*/
    //-----------------
    // Window System
    //-----------------
    class IcWindow;
    class IcWinMng
    {
    public:
        IcWinMng(){};
        virtual ~IcWinMng(){};
        virtual void addWindow(ctl::Sp<IcWindow> pWin){ m_winAry.add(pWin); };
        virtual ctl::TSize getScreenSize(){ return m_screenSize; };
        virtual bool onScreenSize(const ctl::TSize& screenSize);
        
        virtual void drawUpdate(float deltaT);
        virtual void initWindows();
        virtual void releaseWindows();
        virtual void startMainLoop(){};
        
        virtual ctl::Sp<IcWindow> getWindow(int idx){ return m_winAry[idx]; } ;
        virtual void onQuit();
        virtual bool initMng(int argc, char **argv){return false;};
       //---- Singleton of IcWinMng can be set externally.
        static ctl::Sp<IcWinMng> getInstance();
        static void setInstance(ctl::Sp<IcWinMng> p);
        //-----------------
        // Configuration
        //-----------------
        struct TCfg
        {
            bool    m_quitOnKeyEsc = true; // Available only on PC
        };
        TCfg m_cfg;
        
    protected:
        ctl::TSize  m_screenSize = ctl::TSize(0,0);
        ctl::SpAry<IcWindow> m_winAry;
        //---- Singleton
        static ctl::Sp<IcWinMng> m_pInstance;
        
    };
    //-----------------------------------------------
    //	IcWindow
    //-----------------------------------------------
    class IcWindow
    {
    public:
        IcWindow(){};
        virtual void onDrawUpdate(float deltaT);
        virtual void onWindowSize(const ctl::TSize& size);
        void addScene(ctl::Sp<IcScene> pScn);
        void removeAllScene();
        //-----------------
        // PC input
        //-----------------
        enum class TE_MouseButton{L,M,R,NONE};
        virtual void onKeyboard(unsigned char key); // TODO: int
        virtual void onMouseClick(TE_MouseButton btn, bool isUp, int x, int y){};
        virtual void onMouseMove(int x, int y){};
        
        //-----------------
        // Mobile Input
        //-----------------
		virtual void onDeviceRot(const TQuat& q){};
        //-----------------
        // Configuration
        //-----------------
        struct TCfg
        {
            TColor  m_bkColor = TColor(0,0,0,1);
            ctl::TSize   m_size;
            ctl::TPos    m_pos;
        };
        TCfg m_cfg;
        ctl::SpAry<IcScene>& getScnAry(){ return m_scnAry; };
        void initWindow();
        void releaseWindow();

    protected:
        //---- Derive onInit() to create/add your scenes.
        virtual void onInit();
 		//---- Derive onRelease to release openGL res, usually that's not necessary.
		// It's automatically handled by IcWindow::onRelease()
		virtual void onRelease();
		ctl::SpAry<IcScene> m_scnAry;
        std::mutex          m_mtx_draw;
        std::atomic<bool>	m_isDrawing{false};
        std::atomic<bool>   m_hasInit{false};
  
    };
 
    //-----------------------------------------------
    //	IcWindowVR
    //-----------------------------------------------
    class IcWindowVR : public IcWindow
    {
    public:
        virtual void onInit() override;
        virtual void onMouseMove(int x, int y) override;
		virtual void onDeviceRot(const TQuat& q) override;
        struct T_VRCfg
        {
            float K_eyeDist = 0.3;
            //---- Distortion coef
            float K_distortion_k2 = -0.4;
            float K_distortion_k4 = -0.04;
        };
        T_VRCfg m_VRCfg;
        //-----------------------
        //	VRContext
        //-----------------------
        // VR related data is here
        class VRContext
        {
        protected:
            //---- The render target texture of L/R
            ctl::Sp<IcTexture> m_pTex[2]{nullptr, nullptr};
        public:
            VRContext(const T_VRCfg& rCfg):m_rCfg(rCfg){};
            decltype(m_pTex[0]) getTex(bool isR) { return m_pTex[isR]; };
            void onWindowSize(const ctl::TSize& winSize);
            const T_VRCfg& m_rCfg;
        };
        typedef ctl::Sp<VRContext> T_VRCntxSp;
        T_VRCntxSp m_pVRContext = ctl::makeSp<VRContext>(m_VRCfg);
        //-----------------------
        //	VRScnMain
        //-----------------------
        class VRScnMain : public IcScene
        {
        public:
            virtual void onInit() override;
            virtual void onDraw() override;
            void setContext(T_VRCntxSp p){m_pCntxt = p;};
       protected:
            T_VRCntxSp m_pCntxt = nullptr;
            void renderOneSide(bool isR);
        };
        //-----------------------
        //	VRScnDisp
        //-----------------------
        //---- A display Scene contain 2 quad,
        // use rendered texture from main scene
        class VRScnDisp : public IcScene
        {
        protected:
            T_VRCntxSp m_pCntxt = nullptr;
            //---- Distortion mesh
            ctl::Sp<IcMesh> m_pDistMesh = nullptr;
            ctl::Sp<IcObject> m_pObjPlane[2]{nullptr, nullptr};
        public:
            virtual void onInit() override;
            virtual void onWindowSize(const ctl::TSize& winSize) override;
            void setContext(T_VRCntxSp p){m_pCntxt = p;};
            void reInit();
            decltype(m_pDistMesh) createDistortMesh();
            
         };
        void initWithMainScn(ctl::Sp<VRScnMain> pScn);
        
    //-----------------------
    protected:
        ctl::Sp<VRScnMain>      m_pScnMain = nullptr;
        ctl::Sp<VRScnDisp>      m_pScnDisp = nullptr;
        //------------------------
        //  MouseHelper
        //------------------------
        // simulate cam tilte by mouse
        class CMouseHelper
        {
        public:
            TQuat onMouseMove(int x, int y);
        protected:
            TEuler  m_camAtt;
            TVec2   m_mousePrevPos;
            bool    m_isFirst = true;
        };
        CMouseHelper m_mouseHelper;

    };
    

    //-----------------------------------------------
    //	IcApp
    //-----------------------------------------------
    class IcApp
    {
    public:
        IcApp();
        virtual ~IcApp();
        struct TCfg
        {
            std::string m_sPathRes;
            std::string m_sPathDoc;
        };
        TCfg m_cfg;

		//---- Always Override onInit()
        virtual void onInit() ;
        virtual void onRelease();
        
        void addWindow(ctl::Sp<IcWindow> pWin);
        ctl::Sp<IcWinMng> getWinMng();

 		//---- Singleton
        static void setInstance(IcApp* pApp);
        static IcApp* getInstance();
        //-----------------
        // Cmd interface
        //-----------------
        // simple command interface with outside by string,
        // could be JSON or simple text cmd, by your implementation.
        // Return string back.
        virtual std::string onCmd(const std::string& sCmd){ return "OK";};

        //-----------------
        // runCmd()
        //-----------------
        // For PC, not mobile
        int runCmdLine(int argc, char **argv, const std::string& sResPath);
        //-----------------
        // misc util
        //-----------------
        static std::string getResPath();
    protected:
    };
     
    //-----------------------------------------
	//	IcEng
	//-----------------------------------------
	class IcEng
	{
    protected:
        ctl::Sp<CRenderAdp::CTexAdp>   m_pDfltTexAdp = nullptr;
        std::atomic<bool>    m_hasInit{false};
        std::atomic<bool>    m_isEnabled{false};

	public:
		IcEng(){};
		virtual ~IcEng(){};
        struct TCfg
        {
        }m_cfg;
        bool initEng(const std::string& sPathShader);
        void releaseEng();
        void clearScreen(const TColor& bkColor);
        static ctl::Sp<IcEng> getInstance();
        void onFrameStart();
        void onFrameEnd();
        bool hasInit() const { return m_hasInit; };
        bool isEnabled()const{ return m_isEnabled; };
        ctl::Sp<CRenderAdp> getCurRenderAdp();
        decltype(m_pDfltTexAdp) getDfltTexAdp();
        
 	};
	
}//namespace Ic3d
#endif // _ICUBE_H
