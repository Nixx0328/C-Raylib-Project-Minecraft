//以下为代码
#include <bits/stdc++.h>
#include <raylib.h>
#include <rlgl.h>
#include "main.h"
using namespace std;
#define WIDTH 1500
#define HEIGHT 940
#define CENTER (Vector2){750,470}

int a[65][130][65];
int zx=INT_MAX,zd=INT_MIN;
int ly;


void world_init()
{
//	int ly;
	memset(a,-1,sizeof(a));
	memset(b,0,sizeof(b));
	int ns_height=GetRandomValue(0,32),ns_difference=GetRandomValue(1,32);
	
	b[0][0]=GetRandomValue(ns_height-ns_difference,ns_height+ns_difference);
	for(int x=8;x<a_size;x+=8){b[x][0]=b[x-8][0]+GetRandomValue(0,ns_difference/2);}
	for(int y=8;y<a_size;y+=8){b[0][y]=b[0][y-8]+GetRandomValue(0,ns_difference/2);}
	for(int x=8;x<a_size;x+=8)
	{
		for(int y=8;y<a_size;y+=8)
		{
			b[x][y]=b[x][y-8]+GetRandomValue(0,ns_difference/2);
		}
	}
	for(int x=0;x<a_size;x+=8)
	{
		for(int y=0;y<a_size;y+=8)
		{
			perlin_noise(x,y);
		}
	}
	for(int x=0;x<a_size;x++)
	{
		for(int y=0;y<a_size;y++)
		{
			if(b[x][y]<zx) zx=b[x][y];
			else if(b[x][y]>zd) zd=b[x][y];
//			cout<<setw(4)<<b[x][y];
		}
//		cout<<endl;
	}
	//******************************************************************************/
	//***********************************生成基岩层*********************************/
	//******************************************************************************/
	{
		int noise_line;
		for(int z=0;z<65;z++)
		{
			for(int x=0;x<65;x++)
			{
				noise_line=GetRandomValue(1,3);
				a[x][0][z]=noise_line;
				for(int y=1;y<=noise_line;y++)
				{
					a[x][y][z]=0;
				}
				if(noise_line!=3)
				{
					for(int s=noise_line;s<=3;s++)
					{
						a[x][s][z]=1;
					}
				}
			}
		}
	}
//	int y;
	//******************************************************************************/
	//**********************************生成深板岩层********************************/
	//******************************************************************************/
//	ly=4;
	for(int x=0;x<65;x++)
	{
		for(int z=0;z<65;z++)
		{
			for(int y=4;y<=b[x][z]+4;y++)
			{
				a[x][y][z]=1;
			}
		}
	}
	//******************************************************************************/
	//**********************************生成岩石层**********************************/
	//******************************************************************************/
//	ly=5;
	for(int x=0;x<65;x++)
	{
		for(int z=0;z<65;z++)
		{
			for(int y=b[x][z]+5;y<=b[x][z]+37;y++)
			{
				a[x][y][z]=2;
			}
		}
	}
	//******************************************************************************/
	//**********************************生成土层************************************/
	//******************************************************************************/
//	ly=4;
	for(int x=0;x<65;x++)
	{
		for(int z=0;z<65;z++)
		{
			for(int y=b[x][z]+37;y<=b[x][z]+39;y++)
			{
				a[x][y][z]=3;
			}
			a[x][b[x][z]+40][z]=4;
		}
	}
	//*******************************************************************************/
	//**********************************生成橡树************************************/
	//******************************************************************************/
//	for(int x=4;x<a_size-4;x++)
//	{
//		for(int z=4;z<a_size-4;z++)
//		{
//			if(GetRandomValue(0,128)==64)
//			{
//				cout<<x<<" "<<z<<"build a tree!"<<endl;
//				int tree_height=GetRandomValue(4,7);
//				for(int i=x-4;i<x+4;i++)
//				{
//					for(int j=z-4;j<z+4;j++)
//					{
//						a[x][b[x][z]+tree_height+1][z]=6;
//						a[x][b[x][z]+tree_height+2][z]=6;
//					}
//					for(int i=1;i<=tree_height;i++){a[x][a[x][0][z]+i][z]=5;}
//				}
//			}
//		}
//	}
//	int x=10,z=10;
//	int tree_height=GetRandomValue(4,7);
//	for(int i=x-4;i<x+4;i++)
//	{
//		for(int j=z-4;j<z+4;j++)
//		{
//			a[x][a[x][0][z]+tree_height+1][z]=6;
//			a[x][a[x][0][z]+tree_height+2][z]=6;
//		}
//		for(int i=1;i<=tree_height;i++){a[x][a[x][0][z]+i][z]=5;}
//	}
	cout<<"INFO: Map init seccuse!"<<endl<<"Here we go!"<<endl;
}

int main()
{
	InitWindow(WIDTH,HEIGHT,"Minecraft made of C++");//初始化窗口
	SetConfigFlags(FLAG_MSAA_4X_HINT);//开启抗锯齿
	
	//--------↓以下为定义变量部分↓--------
	Image CUBE_IMAGE[20]={LoadImage("Textures/cube/bedrock.png"),
		LoadImage("Textures/cube/deepslate.png"),
		LoadImage("Textures/cube/stone.png"),	
		LoadImage("Textures/cube/dirt.png"),	
		LoadImage("Textures/cube/grass_all.png"),
		LoadImage("Textures/cube/oak_all.png"),
		LoadImage("Textures/cube/oak_leaves.png")
	},USER_GUI_IMAGE[20]={LoadImage("Textures/entity/player/gui_invrow.png"),
		LoadImage("Textures/entity/player/gui_invsel.png"),
		LoadImage("Textures/entity/player/Package.png"),
		LoadImage("Textures/entity/player/hearts_background.png"),
		LoadImage("Textures/entity/player/half a heart1.png"),
		LoadImage("Textures/entity/player/half a heart2.png")
	};
	Texture2D CUBE_TEXTURE[20]={LoadTextureFromImage(CUBE_IMAGE[0]),//0:基岩
		LoadTextureFromImage(CUBE_IMAGE[1]),//1:深层板岩
		LoadTextureFromImage(CUBE_IMAGE[2]),//2:石头
		LoadTextureFromImage(CUBE_IMAGE[3]),//3:泥土
		LoadTextureFromImage(CUBE_IMAGE[4]),//4:草方块整体
		LoadTextureFromImage(CUBE_IMAGE[5]),//5:橡木原木整体
		LoadTextureFromImage(CUBE_IMAGE[6])//6:橡木树叶
	},USER_GUI_TEXTURE[20]={LoadTextureFromImage(USER_GUI_IMAGE[0]),//0:物品栏
		LoadTextureFromImage(USER_GUI_IMAGE[1]),//1:选中框
		LoadTextureFromImage(USER_GUI_IMAGE[2]),//2:背包界面GUI
		LoadTextureFromImage(USER_GUI_IMAGE[3]),//3:血条（空）
		LoadTextureFromImage(USER_GUI_IMAGE[4]),//4:左半颗心
		LoadTextureFromImage(USER_GUI_IMAGE[5]) //5:右半颗心
	};
	Vector3 cube_position;
	Camera3D player={0};
	Vector2 INVROW_POSITION={CENTER.x/2,850};
	int INVSEL_CHOOSE=1,player_life_value=20;
	bool show_player_package_table=false,IskeyDown_r=false;
	//--------↑以上为定义变量部分↑--------
	
	//赋值↓
	world_init();
	cout<<"INFO:World Init successfully!"<<endl;
	player.position=(Vector3){static_cast<float>(GetRandomValue(camera_FOC*5,640-camera_FOC*5)),0.0,static_cast<float>(GetRandomValue(camera_FOC*5,640-camera_FOC*5))};//坐标
	player.target=(Vector3){-90.0,143.5,232.5};//朝向
	player.up=(Vector3){0.0,3.0,0.0};//正上方朝向矢量
	player.fovy=90;//视角场大小
	player.projection=CAMERA_PERSPECTIVE;//摄像机模式
	//赋值↑
	Position3D player_pos={
		static_cast<int>(player.position.x)/10,
		static_cast<int>(player.position.y)/10,
		static_cast<int>(player.position.z)/10
	},
	printer_pos[2]={
		{player_pos.x-camera_FOC/2,player_pos.y-camera_FOC,player_pos.z-camera_FOC/2},
		{player_pos.x+camera_FOC/2,player_pos.y+camera_FOC,player_pos.z+camera_FOC/2}
	};
	//向上搜索player安全的y坐标
	player.position.y=static_cast<float>((b[player_pos.x][player_pos.z]+40)*10); 
	cout<<player.position.y<<endl;
	
	SetTargetFPS(120);//设置FPS为每秒80次
	DisableCursor();//隐藏鼠标
	
	for(int x=player_pos.x-2;x<=player_pos.x+2;x++)
	{
		for(int z=player_pos.z-2;z<=player_pos.z+2;z++)
		{
			a[x][static_cast<int>(player.position.y)/10+5][z]=6;
			a[x][static_cast<int>(player.position.y)/10+4][z]=6;
		}
	}
	for(int x=player_pos.x-1;x<=player_pos.x+1;x++)
	{
		for(int z=player_pos.z-1;z<=player_pos.z+1;z++)
		{
			a[x][static_cast<int>(player.position.y)/10+6][z]=6;
			a[x][static_cast<int>(player.position.y)/10+7][z]=6;
		}
	}
	if(GetRandomValue(0,2)==1){a[player_pos.x-1][static_cast<int>(player.position.y)/10+7][player_pos.z-1]=-1;}
	if(GetRandomValue(0,2)==1){a[player_pos.x+1][static_cast<int>(player.position.y)/10+7][player_pos.z-1]=-1;}
	if(GetRandomValue(0,2)==1){a[player_pos.x-1][static_cast<int>(player.position.y)/10+7][player_pos.z+1]=-1;}
	if(GetRandomValue(0,2)==1){a[player_pos.x+1][static_cast<int>(player.position.y)/10+7][player_pos.z+1]=-1;}
	for(int y=static_cast<int>(player.position.y)/10;y<=static_cast<int>(player.position.y)/10+6;y++)
	{
		a[player_pos.x][y][player_pos.z]=5;
	}
	while(!WindowShouldClose())
	{
		player_pos.x=static_cast<int>(player.position.x)/10;
		player_pos.y=static_cast<int>(player.position.y)/10;
		player_pos.z=static_cast<int>(player.position.z)/10;
		//y坐标fall
//		if(player_pos.y>b[player_pos.x][player_pos.z]+ly+3) player.position.y-=0.5;
//		cout<<player_pos.x<<" "<<player_pos.y<<" "<<player_pos.z<<endl;
//		player_life_value++;
//		if(player_life_value==21) player_life_value=0;//这两句千万不可以写进去=D
		
//***********************************************************************************************
//*****************************************杂项按键侦测******************************************
//***********************************************************************************************
		if(show_player_package_table!=true) UpdateCamera(&player, CAMERA_FIRST_PERSON);
		if(IsKeyDown(' ')){player.position.y++;}
		if(IsKeyDown(KEY_LEFT_SHIFT)||IsKeyDown(KEY_RIGHT_SHIFT)){player.position.y--;}
		if(IsKeyDown(',')||IsKeyDown('<')){player.fovy--;}
		if(IsKeyDown('.')||IsKeyDown('>')){player.fovy++;}
		if(IsKeyDown('+')||IsKeyDown('=')){if (IsCursorHidden()) EnableCursor();}
		if(IsKeyDown('-')||IsKeyDown('_')){if (!IsCursorHidden()) DisableCursor();}
//***********************************************************************************************
//***************************************主要功能按键侦测****************************************
//***********************************************************************************************
		if(IsKeyDown('/')){
			int x,y,z;
			cin>>x>>y>>z;
			player.position.x=static_cast<float>(x*10);
			player.position.y=static_cast<float>(y*10);
			player.position.z=static_cast<float>(z*10);
		}
		if(GetMouseWheelMove()!=0)
		{
			if(GetMouseWheelMove()<0&&INVSEL_CHOOSE==9) INVSEL_CHOOSE=1;
			else if(GetMouseWheelMove()>0&&INVSEL_CHOOSE==1)INVSEL_CHOOSE=9;
			else INVSEL_CHOOSE-=Turn_MouseWhell();
		}
		if(IskeyDown_r==false&&IsKeyDown('r')==true)
		{
			IskeyDown_r=true;
			cout<<IskeyDown_r<<endl;
		}
		if(IskeyDown_r==true&&IsKeyDown('r')==false)
		{
			if(show_player_package_table==false) show_player_package_table=true;
			else show_player_package_table=false;
			IskeyDown_r=false;
			cout<<IskeyDown_r<<endl;
		}
		if(player_pos.x-camera_FOC/2>=0&&player_pos.x+camera_FOC/2<=64&&player_pos.y-camera_FOC/2>=0&&player_pos.y+camera_FOC/2<=130&&player_pos.z-camera_FOC/2>=0&&player_pos.z+camera_FOC/2<=64){
			printer_pos[0]={player_pos.x-camera_FOC/2,player_pos.y-camera_FOC/2,player_pos.z-camera_FOC/2};
			printer_pos[1]={player_pos.x+camera_FOC/2,player_pos.y+camera_FOC/2,player_pos.z+camera_FOC/2};
		}
		else{
			if(player_pos.x-camera_FOC/2<0) printer_pos[0].x=0;
			else if(player_pos.x+camera_FOC/2>64) printer_pos[1].x=64;
			if(player_pos.y-camera_FOC/2<=0) printer_pos[0].y=1;
			else if(player_pos.y+camera_FOC/2>130) printer_pos[1].y=130;
			if(player_pos.z-camera_FOC/2<0) printer_pos[0].z=0;
			else if(player_pos.z+camera_FOC/2>64) printer_pos[1].z=64;
		}
		BeginDrawing();//开始绘制
		{
			ClearBackground(SKYBLUE);
			BeginMode3D(player);
			{
				XYZ_LINE((Vector3){player.position.x,player.position.y-10,player.position.z});
				for(int x=printer_pos[0].x;x<printer_pos[1].x;x++)
				{
					for(int y=printer_pos[0].y;y<=printer_pos[1].y;y++)
					{
						for(int z=printer_pos[0].z;z<printer_pos[1].z;z++)
						{
							if(a[x][y][z]!=-1)
							{
								cube_position={static_cast<float>(x)*10,static_cast<float>(y)*10,static_cast<float>(z)*10};
								if(a[x][y][z]==4||a[x][y][z]==5) DrawCubeTexture_Mode2(CUBE_TEXTURE[a[x][y][z]],cube_position,10,10,10,WHITE);
								else DrawCubeTexture_Mode1(CUBE_TEXTURE[a[x][y][z]],cube_position,10,10,10,WHITE);
							}
						}
					}
				}
			}
			EndMode3D();
			//***********************************************
			//*************绘制屏幕中央的十字准心************
			//***********************************************
			DrawLineEx((Vector2){CENTER.x-15,CENTER.y},(Vector2){CENTER.x+15,CENTER.y},3.0,(Color){255,255,255,140});
			DrawLineEx((Vector2){CENTER.x,CENTER.y-15},(Vector2){CENTER.x,CENTER.y+15},3.0,(Color){255,255,255,140});
			//***********************************************
			//**********绘制屏幕下方的物品栏和生命值*********
			//***********************************************
			DrawTexture(USER_GUI_TEXTURE[0],INVROW_POSITION.x,INVROW_POSITION.y,WHITE);
			DrawTexture(USER_GUI_TEXTURE[1],INVROW_POSITION.x-82+INVSEL_CHOOSE*80,INVROW_POSITION.y-3,WHITE);
			DrawTexture(USER_GUI_TEXTURE[3],INVROW_POSITION.x,INVROW_POSITION.y-50,WHITE);
			int i;
			for(i=1;i<=player_life_value;i++)
			{
				if(i%2==1) DrawTexture(USER_GUI_TEXTURE[4],INVROW_POSITION.x+i*16-11,INVROW_POSITION.y-46,WHITE);
				else DrawTexture(USER_GUI_TEXTURE[5],INVROW_POSITION.x+i*16-13,INVROW_POSITION.y-46,WHITE);
			}
			DrawFPS(10, 10);
			if(show_player_package_table==true) DrawTexture(USER_GUI_TEXTURE[2],CENTER.x,CENTER.y,WHITE);
			DrawCamera3DPosition(player,10,150,20,DARKGRAY,"Player's Position:");
			DrawText("Minecraft Powerd by C++-Raylib!",10,30,30,YELLOW);
			DrawText("Key + to show the mouse,and key - to hide the mouse",10,60,30,DARKGRAY);
			DrawText("Key < or > to change the fovy",10,90,30,DARKGRAY);
			DrawText("Key ESC to EXIT",10,120,30,DARKGRAY);
		}
		EndDrawing();//结束绘制
	}
	//释放材质和贴图
	for(int i=1;i<=5;i++)
	{
		UnloadTexture(CUBE_TEXTURE[i]);
		UnloadImage(CUBE_IMAGE[i]);
	}
	return 0;
}
