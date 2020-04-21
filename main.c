//May need -lm and -O3 for compile
//Enable vectorization and OpenMP for best performance

//#define AA //Uncomment for activate antialiasing
//#define DoF 5//Uncomment both lines for activate depth of field
//#define DoFy 4//Uncomment both lines for activate depth of field 
#include "RayTrace0.7.5.h"
#include "bmpwriter.h"
#include <time.h>

int FRAMES;
void testRT();
void bmpdraw(Image I)
{
  int i=0,j=0;
  for(;i<X;i++)
   for(j=0;j<Y;j++)
     putpixel(i+1,j+1,I.Pixels[(Y-j-1)*X+i].r,I.Pixels[(Y-j-1)*X+i].g,I.Pixels[(Y-j-1)*X+i].b);
}
int main()
{   
printf("%d\n",sizeof(Object));
	X=1920,Y=1080,FRAMES=300;
    bmpinit(X,Y);
    Image I=NewImage(X,Y);
    clock_t start = clock(), diff;
    testRT(&I); 
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    bmpdraw(I);
    bmpsave("RT4.bmp");
    printf("%dx%d  ",X,Y);
    printf("%.2f FPS\n%d frames in %d seconds %d milliseconds\n",1000.0*FRAMES/msec,FRAMES,msec/1000, msec%1000);
    #ifdef _OPENMP
    	printf("OPENMP Enabled.\n");
    	printf("Timing may work wrong if compiled OpenMP enabled under Linux/UNIX\n"); 
    #else
    	printf("OPENMP Disabled.\n");
    #endif                                     
}
PROFILEFUNC
void testRT(Image* I)
{
	
	Vec3 camdir={0,0,1};
    Point campos={0,0,-1600};
   /* Vec3 camdir={0,-1,1};
    Point campos={0,1000,-1000};*/
    Camera cam={campos,camdir};
    int x;
    Point s1c={0,0,300};
    Color c1={5,100,255};
    Sphere s={s1c,250};
    Object Obj={s,SPHERE,c1,0.8,0.2};

    Point s1c2={-600,50,320};
    Color c2={240,250,255};
    Sphere s2={s1c2,300};
    Object Obj2={s2,SPHERE,c2,0.92,0.90};
    
    Point s3c={-0,-100000,100};
    Color c3={255,255,255};
    Sphere s3={s3c,100000-250};
    Object Obj3={s3,SPHERE,c3,1,0.2};
    
    Point s4c={-300,-210,100};
    Color c4={255,100,5};
    Sphere s4={s4c,40};
    Object Obj4={s4,SPHERE,c4,1,0};
    
    Point s5c={-100,-210,-50};
    Color c5={0,255,0};
    Sphere s5={s5c,40};
    Object Obj5={s5,SPHERE,c5,1,0};
    
    Point s6c={700,-210,3700};
    Color c6={0,255,0};
    Sphere s6={s6c,40};
    Object Obj6={s6,SPHERE,c6,1,0};
    
    Object *Objects=(Object*)malloc(sizeof(Object)*6);
    Objects[0]=Obj;
    Objects[1]=Obj2;
    Objects[2]=Obj3;
    Objects[3]=Obj4;
    Objects[4]=Obj5;
    Objects[5]=Obj6;
    
    Light *Lights=(Light*)malloc(sizeof(Light)*2);
    Point Pos={50,550,30};
    Color lc1={255,255,255};
    Lights[0]=(struct Light){Pos,lc1,800000.0/255};
    
    Point Pos2={520,120,10};
    Color lc2={200,100,100};
    Lights[1]=(struct Light){Pos2,lc2,300000.0/255};
    
    Scene scene=(struct Scene){5,2,Objects,Lights};
        
    for(x=0;x<FRAMES;x++)
	{
	    //scene.Lights[1].pos.x-=1.0;
	    RTRender(I,cam,scene);       
	}
}
