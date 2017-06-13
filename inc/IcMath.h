//
//  IcMath.h
//  IcEngLib
//
//  Created by Sherman Chen on 4/20/17.
//  Copyright © 2017 Simviu Technology Inc. All rights reserved.
//

#ifndef IcMath_h
#define IcMath_h

#include "ctl.h"

//---- GLM : Matrix Math Lib
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace Ic3d
{
    //-----------------------------------------------
    //	From vector vmath
    //-----------------------------------------------
    typedef glm::quat	TQuat;
    typedef glm::vec4	TVec4;
    typedef glm::vec3	TVec3;
    typedef glm::vec2	TVec2;
    typedef glm::mat3	TMat3;
    typedef glm::mat4	TMat4;
    typedef glm::vec4	TColor;
    
    inline TVec2 vecConv(const ctl::TPos& d){ return TVec2(d.x, d.y); };
    inline ctl::TPos vecConv(const TVec2& d){ return ctl::TPos(d.x, d.y); };
    //-----------------------------------------
    //	TTransform
    //-----------------------------------------
    class TTransform
    {
    protected:
        TVec3	m_pos;
        TVec3	m_scale=TVec3(1,1,1);
        TQuat	m_quat;
        TMat4   m_mat;
    private:
        
    public:
        virtual	~TTransform(){};
        
        //---- Transform
        inline void	setQuat(const TQuat& q)     {m_quat=q;      };
        inline void	setScale(const TVec3& scl)  {m_scale=scl;   };
        inline void setPos(const TVec3& ps)     {m_pos=ps;      };
        inline TVec3 getPos()const{ return m_pos; };
        inline TVec3 getScale()const{ return m_scale; };
        inline TQuat getQuat()const{ return m_quat; };
        TMat4 getMat()
        {
            TMat4 m0;
            TMat4 ms = glm::scale(m0, m_scale);
            TMat4 mr = glm::mat4_cast(m_quat);
            TMat4 mt = glm::translate(m0, m_pos);
            return mt*mr*ms;
        };
        
    };
    
    //-----------------------------------------------
    //	TEuler
    //-----------------------------------------------
    struct TEuler
    {
        TEuler(){};
        TEuler(float pi, float ri, float yi):
        p(pi), r(ri), y(yi){};
        float p = 0;    // pitch
        float r = 0;    // roll
        float y = 0;    // yaw
        TQuat toQuat()const{ return TQuat(TVec3(p,y,r));};
    };
    

    
} // namespace Ic3d


#endif /* IcMath_h */
