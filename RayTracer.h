//May not compile without -lm and -O3
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifdef _OPENMP
  #include <omp.h>
#endif
#ifdef __GNUC__
    #define likely(x)       __builtin_expect((x),1)
    #define unlikely(x)     __builtin_expect((x),0)
#else
    #define likely(x) (x)
    #define unlikely(x) (x)
#endif
//#define DEBUG
//#define PROFILE
#if defined(PROFILE) && defined(__GNUC__)
   #define PROFILEFUNC __attribute__((noinline))
   #define PROForINLINE __attribute__((noinline))
#else
   #define PROFILEFUNC 
   #define PROForINLINE inline
#endif
#define IMP 0.1
#define TDS(X) typedef struct X X;struct X;
#define MAX_FLOAT 9999999999999

TDS(Point)
TDS(Vec3)
TDS(Camera)
TDS(Ray)
TDS(Color)
TDS(Sphere)
TDS(Object)
TDS(Light)
TDS(Scene)
TDS(Image)

unsigned char *BMP;
int X=640,Y=400;
struct Point
{
    float x,y,z;
};
struct Vec3
{
    float x,y,z;
};
Vec3  Vadd(Vec3 a,Vec3 b)
{
    return (struct Vec3){a.x+b.x,a.y+b.y,a.z+b.z};
}
Point  PVadd(Point a,Vec3 b)
{
    return (struct Point){a.x+b.x,a.y+b.y,a.z+b.z};
}
PROFILEFUNC
Vec3  Psub(Point a,Point b)
{
    return (struct Vec3){a.x-b.x,a.y-b.y,a.z-b.z};
}

Point  pPVadd(Point *a,Vec3 *b)
{
    a->x+=b->x;
    a->y+=b->y;
    a->z+=b->z;
}
float fastInvSqrt(float x) {
  int i = *(int*)&x;
  i = 0x5f3759df - (i >> 1);
  float y = *(float*)&i;
  return y * (1.5F - 0.5F * x * y * y);
}
float invVMag(Vec3 a)
{
    return fastInvSqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}
void  pNorm(Vec3 *a)//Normalize
{
    #ifdef FAST//Using fastInvSqrt here tortures image quality a lot.
      float imag=fastInvSqrt(a->x*a->x+a->y*a->y+a->z*a->z);
    #else
      float imag=1/sqrt(a->x*a->x+a->y*a->y+a->z*a->z);
    #endif
    a->x*=imag;
    a->y*=imag;
    a->z*=imag;
}
Vec3  Vsub(Vec3 a,Vec3 b)
{
    return (struct Vec3){a.x-b.x,a.y-b.y,a.z-b.z};
}
PROFILEFUNC
float VSmul(Vec3 a,Vec3 b)//Dot
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vec3  Vmul(Vec3 a,Vec3 b)//Cross
{
    return (struct Vec3){a.y*b.z-a.z*b.y,
                          a.z*b.x-a.x*b.z,
                          a.x*b.y-a.y*b.x};
}

PROFILEFUNC
Vec3  Norm(Vec3 a)//Normalized
{
    //float imag=1/sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
    float imag=fastInvSqrt(a.x*a.x+a.y*a.y+a.z*a.z);
    return (struct Vec3){a.x*imag,a.y*imag,a.z*imag};
}
PROFILEFUNC
float VMag(Vec3 a)
{
    return sqrtf(a.x*a.x+a.y*a.y+a.z*a.z);
}
Vec3  Vscale(Vec3 a,float mag)//Scale
{
    mag*=fastInvSqrt(a.x*a.x+a.y*a.y+a.z*a.z);
    return (struct Vec3){a.x*mag,a.y*mag,a.z*mag};
}

struct Camera
{
   Point pos;
   Vec3 dir;
};

struct Ray
{
   Point start;
   Vec3 dir;
};

struct Color
{
    unsigned char r,g,b;
};
Color CScale(float A,Color C)
{
    int r=A*C.r,g=A*C.g,b=A*C.b;
    Color c;
    c.r=r>255 ? 255 : (r<0 ? 0 : r);
    c.g=g>255 ? 255 : (g<0 ? 0 : g);
    c.b=b>255 ? 255 : (b<0 ? 0 : b);;
    return c;
}
Color CMul(Color A,Color B)
{
    return(struct Color){((int)A.r)*B.r/255,((int)A.g)*B.g/255,((int)A.b*B.b)/255};
}
Color CSCom(Color A,Color B,float b)
{
    float a=1-b;
    return(struct Color){A.r*a+B.r*b,A.g*a+B.g*b,A.b*a+B.b*b};
}
Color CAdd(Color A,Color B)
{
    return(struct Color){A.r+B.r,A.g+B.g,A.b+B.b};
}

struct Image
{
    int width,height;
    Color *Pixels;
};
Image NewImage(int x,int y)
{
    return (struct Image){x,y,(Color*)malloc(x*y*sizeof(Color))};
}

Point PixelPos(int x, int y)
{
    return (struct Point){x-(X/2),y-(Y/2),100};
}
Ray RayofPix(Point cam_pos,int x,int y)
{
    Point p=PixelPos(x,y);
    Vec3 dir=Psub(p,cam_pos);
    pNorm(&dir);
    return (struct Ray){cam_pos,dir};
}

struct Sphere
{
    Point C;
    float r;
};
#define SPHERE 1

struct Object
{
    int type;
    float MathA,MathB;
    Color color;
    union
    {
       Sphere sphere;
    }u;
};

struct Light
{
    Point pos;
    Color color;
    float x;
};

struct Scene
{
    int numObj,numLight;
    Object *Objects;
    Light  *Lights;
};


PROFILEFUNC
Color shade(Scene scene,Point P,int obj)
{
    Object Obj=scene.Objects[obj];
    if(Obj.type=SPHERE)
    {
        Vec3 SufNorm=Norm(Psub(P,Obj.u.sphere.C));
        int i=0,NL=scene.numLight;
        Color ret,c;
        Vec3 PtoLight;
        float idist;
        for(i=0;i<NL;i++)
        {
          PtoLight=Psub(scene.Lights[i].pos,P);
          idist=invVMag(PtoLight);idist*=idist*idist;
          idist=VSmul(SufNorm,PtoLight)*idist;         
          c=CScale(idist*scene.Lights[i].x,CMul(Obj.color,scene.Lights[i].color));
          ret=CSCom(ret,c,(i<1?1:i)/(float)(i+1));
        }
        return ret;
    }
}


PROForINLINE
float intersect(Ray ray,Object obj)
{
   if(likely(obj.type=SPHERE))
   {
      Vec3 CamtoCen=Psub(obj.u.sphere.C,ray.start);
      float tca =VSmul(CamtoCen,ray.dir);      
      if (unlikely(tca < 0))return -0.0;
      float dist2 = VSmul(CamtoCen,CamtoCen) - tca * tca;
      float r2=obj.u.sphere.r;r2*=r2;
      return dist2 > r2? -0.0: tca-sqrt(r2 - dist2);          
   }
}
PROFILEFUNC
Color RayTrace(Ray ray,Scene scene,float imp,const int numObj)
{
    int i=0,firsti=-1;float min=MAX_FLOAT,dist;
    Object *O=scene.Objects;
    Object obj;
    for(i=0;i<numObj;i++)
    {   dist=intersect(ray,O[i]);
        firsti=(dist&&dist<min)?i:firsti;
        min=(dist&&dist<min)?dist:min;       
    }  
    if(firsti==-1)return (struct Color){0,0,0};
    obj=O[firsti];
    if(likely(obj.type==SPHERE))
    {
        Point intersectionP=PVadd(ray.start,Vscale(ray.dir,min));
        Color c1=CScale(obj.MathA,shade(scene,intersectionP,firsti));
        Color c2;
        if(unlikely(obj.MathB*imp>IMP))
          c2=RayTrace((struct Ray){intersectionP, Norm(Psub(intersectionP,obj.u.sphere.C))},scene,obj.MathB*imp,numObj);
       return c1;
    }
}
void RTRender(Image* I,Camera cam,Scene scene)
{
    int x,y;
    Point cam_pos={0,0,0};
    Color* P=I->Pixels;
    int numObj=scene.numObj;
    
    for(y=0;y<Y;y++)
        #pragma omp parallel for        
        for(x=0;x<X;x++)
            P[y*X+x]=RayTrace(RayofPix(cam_pos,x,y),scene,1.0,numObj);
}


