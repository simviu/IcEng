//
//  IcViewController.mm
//  DevEng
//
//  Created by Sherman Chen on 3/2/16.
//  Copyright (c) 2016 Simviu Technology Inc.
//  All rights reserved.
//  http://www.simviu.com/dev
//
//

#import "IcViewController.h"
#import <OpenGLES/ES2/glext.h>
#include "Ic3d.h"

using namespace ctl;
using namespace Ic3d;
using namespace std;

const static GLfloat K_bkColor[4] = {0.2, 0.4, 0.9, 1.0};
@interface IcViewController () {
	
	bool m_hasInit;
    bool m_reqViewSizeChange;
    IcApp*  m_pIcApp;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation IcViewController

//--------------------------------------
//  initIcApp
//--------------------------------------
-(void)initIcApp:(void*)pAppIn
{
    m_pIcApp = static_cast<IcApp*>(pAppIn);
    if(m_pIcApp==nullptr) return;
    IcApp::setInstance(m_pIcApp);

    auto& cfg = m_pIcApp->m_cfg;
    //---- Set App Res Path
    NSString* nsRes = [[NSBundle mainBundle] resourcePath];
    cfg.m_sPathRes = string([nsRes UTF8String]) +"/IcData/";

    //---- Set App Doc Path
    NSString *nsDoc = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
    cfg.m_sPathDoc = string([nsDoc UTF8String]) +"/";
}
//--------------------------------------
//  viewDidLoad
//--------------------------------------
- (void)viewDidLoad
{
	[super viewDidLoad];
	m_hasInit = false;
    m_reqViewSizeChange = false;
    
    //---- Make sure IcApp Init before GL context,
    // For cross platform consistence.
    //---- Call onInit()
    m_pIcApp->onInit();
    
    //----- Create OpenGL context
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	
	if (!self.context) {
		NSLog(@"Failed to create ES context");
	}
	
	GLKView *view = (GLKView *)self.view;
	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	[self setupGL];
}

//--------------------------------------
//  getScaledViewRect
//--------------------------------------
-(CGRect)getScaledViewRect
{
    float w = self.view.frame.size.width;
    float h = self.view.frame.size.height;
    float scrnScl = 1.0f;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        scrnScl = [[UIScreen mainScreen] scale];
    }
    CGRect vrect = CGRectMake(0, 0, w*scrnScl, h*scrnScl);
    return vrect;
}

//--------------------------------------
//  Ic3d_onInit
//--------------------------------------
-(void)Ic3d_onInitWindow
{
    auto pApp = m_pIcApp;
    if(pApp==nullptr) return;
    pApp->getWinMng()->initWindows();
}
-(void)Ic3d_onReleaseWindow
{
    auto pApp = m_pIcApp;
    if(pApp==nullptr) return;
    pApp->getWinMng()->releaseWindows();
}
//--------------------------------------
//  Ic3d_onInit
//--------------------------------------
-(void)Ic3d_onViewRect:(CGRect)viewRect
{
    auto pApp = m_pIcApp;
    if(pApp==nullptr) return;
    auto sz = viewRect.size;
    //---- convert to ctl::TSize from CGSize
    pApp->getWinMng()->onScreenSize(ctl::TSize(sz.width, sz.height));
};
//--------------------------------------
//  Ic3d_onInit
//--------------------------------------
-(void)Ic3d_onDrawUpdate:(double)deltaT
{
    auto pApp = m_pIcApp;
    if(pApp==nullptr) return;
    pApp->getWinMng()->drawUpdate(deltaT);
};

//--------------------------------------
//  viewWillLayoutSubviews
//--------------------------------------
-(void) viewWillLayoutSubviews
{
    if(!m_hasInit) return;
    CGRect vrect = [self getScaledViewRect];
    [self Ic3d_onViewRect:vrect];
    NSLog(@"[Debug]: VConIc3d: viewWillLayoutSubviews");
}

//--------------------------------------
//  viewWillTransitionToSize
//--------------------------------------
-(void) viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    // best call super just in case
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    
    // will execute before rotation
    [coordinator animateAlongsideTransition:^(id  _Nonnull context) {
        
        // will execute during rotation
        
    } completion:^(id  _Nonnull context) {
        
        m_reqViewSizeChange = true;
        NSLog(@"---- VConIc3d: deviceRoated ----");
    }];
}


- (void)dealloc
{
	[self tearDownGL];
	
	if ([EAGLContext currentContext] == self.context) {
		[EAGLContext setCurrentContext:nil];
	}
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	
	if ([self isViewLoaded] && ([[self view] window] == nil)) {
		self.view = nil;
		
		[self tearDownGL];
		
		if ([EAGLContext currentContext] == self.context) {
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
	
	// Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
	return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

}

- (void)tearDownGL
{
	[EAGLContext setCurrentContext:self.context];
    [self Ic3d_onReleaseWindow];

}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // this is handled by drawInRect
}

//--------------------------------------
//  drawInRect
//--------------------------------------
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //---- clear screen
    const auto& c = K_bkColor;
    glClearColor(c[0], c[1], c[2], c[3]);	// background color
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	//---------------
	//	call delegate
	//---------------
    if(!m_hasInit)
    {
        CGRect vrect = [self getScaledViewRect];
        [self Ic3d_onInitWindow];
        
        [self Ic3d_onViewRect:vrect];
        m_hasInit = true;
    }
    else
    {
        float deltaT = self.timeSinceLastDraw;
        [self Ic3d_onDrawUpdate:deltaT];
        
    }
}

@end
