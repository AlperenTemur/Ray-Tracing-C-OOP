#include "RayTracer.h"
#include "BmpWriter.h"
#include <time.h>


void testintersection();
void testRT();
void bmpdraw(Image I)
{
  int i=0,j=0;
  for(;i<X;i++)
   for(j=0;j<Y;j++)
     putpixel(i+1,j+1,I.Pixels[(Y-j-1)*X+i].r,I.Pixels[(Y-j-1)*X+i].g,I.Pixels[(Y-j-1)*X+i].b);
}
main()
{   
    clock_t start = clock(), diff;
    testRT(); 
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("%.2f FPS %d seconds %d milliseconds",60000.0/msec,msec/1000, msec%1000);    
    //int a;scanf("%d",&a);printf("%d",a);                                      
}
PROFILEFUNC
void testRT()
{
    int x=0,y;
    Point s1c={0,0,200};
    Vec3 camdir={0,0,100};
    Point campos={0,0,0};
    Camera cam={campos,camdir};
    Color c1={50,250,255};
    Sphere s={s1c,70};
    Object Obj={SPHERE,0.8,0.0,c1,s};

    Point s1c2={-100,0,270};
    Color c2={255,50,50};
    Sphere s2={s1c2,90};
    Object Obj2={SPHERE,0.8,0.0,c2,s2};
    Object *Objects=(Object*)malloc(sizeof(Object)*2);
    Objects[0]=Obj;
    Objects[1]=Obj2;
    Light *Lights=(Light*)malloc(sizeof(Light)*2);
    Point Pos={0,220,160};
    Color lc1={255,255.255};
    Lights[0]=(struct Light){Pos,lc1,100000.0};
    Point Pos2={300,300.200};
    Color lc2={255,200.200};
    Lights[1]=(struct Light){Pos2,lc2,100000.0};
    Scene scene=(struct Scene){2,2,Objects,Lights};
    Image I=NewImage(1920,1080);
    X=1920,Y=1080;
    for(x=0;x<60;x++)
  {
    scene.Lights[1].pos.x-=2.0;
    RTRender(&I,cam,scene);   
    
  }
  bmpinit(1920,1080);
  bmpdraw(I);
  bmpsave("RT4.bmp",1920,1080);
}
