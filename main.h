#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#define GRASS_GREEN CLITERAL(Color){155,255,125,255}

int camera_FOC=16;//成像范围为视角周围32格
const int a_size=65;
int b[a_size][a_size];

struct Position3D{
	int x;
	int y;
	int z;
};

void hdg(int last_x1,int last_x2,int y){
	int next_x=(last_x1+last_x2)/2,next_value=(b[last_x1][y]+b[last_x2][y])/2;
	b[next_x][y]=next_value;
	if(abs(last_x1-last_x2)==2){return ;}
	else{
		hdg(last_x1,next_x,y);
		hdg(next_x,last_x2,y);
	}
}

void zdg(int last_y1,int last_y2,int x){
	int next_y=(last_y1+last_y2)/2,next_value=(b[x][last_y1]+b[x][last_y2])/2;
	b[x][next_y]=next_value;
	if(abs(last_y1-last_y2)==2){return ;}
	else{
		zdg(last_y1,next_y,x);
		zdg(next_y,last_y2,x);
	}
}

void perlin_noise(int pos_x,int pos_y){
//	memset(a,0,sizeof(a));
	int pos[2]={pos_x,pos_y};
	hdg(pos[0],pos[0]+8,pos[1]);
	hdg(pos[0],pos[0]+8,pos[1]+8);
	for(int ix=pos[0];ix<=pos[0]+8;ix++)
	{
		zdg(pos[1],pos[1]+8,ix);
	}
}

const char* ConcatenateStrings(const char* str1, const char* str2)
{
	std::stringstream ss;
	ss << str1 << str2;
	
	std::string result = ss.str();
	
	char* resultCstr = new char[result.size()+1];
	strcpy(resultCstr, result.c_str());
	
	return resultCstr;
}

int Turn_MouseWhell()
{
	if(GetMouseWheelMove()>0) return 1;
	else if(GetMouseWheelMove()<0) return -1;
	else return 0;
}

void XYZ_LINE(Vector3 a)
{    
	//绘制X,Y,Z轴
	DrawCube(a,10,1,1,RED);
	DrawCube(a,1,10,1,GREEN);
	DrawCube(a,1,1,10,BLUE);
}

const char* GetMousePositionChar()
{
	Vector2 mousePos = GetMousePosition();
	char* posString = (char*)malloc(32 * sizeof(char));
	snprintf(posString, 32, "Mouse Position: (%.0f, %.0f)", mousePos.x, mousePos.y);
	return posString;
}

const char* Vector2_to_char(Vector2 input)
{
	Vector2 output = {input.x,input.y};
	char* outputString = (char*)malloc(32 * sizeof(char));
	snprintf(outputString, 32, "Mouse Position: (%.0f, %.0f)", output.x, output.y);
	return outputString;
}

const char* Vector3_to_char(const char* front_sentence,Vector3 input)
{
	Vector3 output = {input.x,input.y,input.z};
	char* outputString = (char*)malloc(32 * sizeof(char));
	const char* str="(%.0f, %.0f,%.0f) ";
	const char* output_sentence=ConcatenateStrings(front_sentence,str);
	snprintf(outputString, strlen(front_sentence)+strlen(str), output_sentence, output.x, output.y,output.z);
	return outputString;
}

void DrawMousePosition(int PosX,int PosY,int size,Color color){ DrawText(GetMousePositionChar(),PosX,PosY,size,color); }
void DrawCamera3DPosition(Camera3D camera,int PosX,int PosY,int size,Color color,const char* front_sentence){ DrawText(Vector3_to_char(front_sentence,camera.position), PosX, PosY, size, color); }

//***********************************************************************************************
//********************Draw cube with texture piece applied to all faces**************************
//***********************************************************************************************
void DrawCubeTexture_Mode1(Texture2D texture, Vector3 position, float width, float height, float length, Color color)
{    
	float x = position.x;
	float y = position.y;
	float z = position.z;
	
	// Set desired texture to be enabled while drawing following vertex data
	rlSetTexture(texture.id);
	
	// Vertex data transformation can be defined with the commented lines,
	// but in this example we calculate the transformed vertex data directly when calling rlVertex3f()
	//rlPushMatrix();
	// NOTE: Transformation is applied in inverse order (scale -> rotate -> translate)
	//rlTranslatef(2.0f, 0.0f, 0.0f);
	//rlRotatef(45, 0, 1, 0);
	//rlScalef(2.0f, 2.0f, 2.0f);
	
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Front Face
	rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
	// Back Face
	rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
	// Top Face
	rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
	// Bottom Face
	rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);       // Normal Pointing Right
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	// Left Face
	rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
	rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlEnd();
	//rlPopMatrix();
	
	rlSetTexture(0);
}
void DrawCubeTexture_Mode2(Texture2D texture, Vector3 position, float width, float height, float length,Color color)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;
	
	rlSetTexture(texture.id);
	
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Front Face
	rlNormal3f(0.0f, 0.0f, 1.0f);       // Normal Pointing Towards Viewer
	rlTexCoord2f(0.25, 0.5f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.5f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.25f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.25f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Left Of The Texture and Quad
	// Back Face
	rlNormal3f(0.0f, 0.0f, - 1.0f);     // Normal Pointing Away From Viewer
	rlTexCoord2f(0.75f, 0.5f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(0.75f, 0.25f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.25f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(1.0f, 0.5f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
	// Top Face
	rlNormal3f(0.0f, 1.0f, 0.0f);       // Normal Pointing Up
	rlTexCoord2f(0.5f, 0.25f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.25f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.0f); rlVertex3f(x + width/2, y + height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.0f); rlVertex3f(x + width/2, y + height/2, z - length/2);  // Top Right Of The Texture and Quad
	// Bottom Face
	rlNormal3f(0.0f, - 1.0f, 0.0f);     // Normal Pointing Down
	rlTexCoord2f(0.5f, 0.75f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.75f); rlVertex3f(x + width/2, y - height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.5f); rlVertex3f(x + width/2, y - height/2, z + length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.5f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlEnd();
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	// Right face
	rlTexCoord2f(0.75f, 0.25f);rlVertex3f(x + width/2, y + height/2, z + length/2);    // Bottom Right Of The Texture and Quad
	rlTexCoord2f(0.75f, 0.5f);rlVertex3f(x + width/2, y - height/2, z + length/2);   // Top Right Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.5f);rlVertex3f(x + width/2, y - height/2, z - length/2);   // Top Left Of The Texture and Quad
	rlTexCoord2f(0.5f, 0.25f);rlVertex3f(x + width/2, y + height/2, z - length/2);  // Bottom Left Of The Texture and Quad
	
	// Left Face
	rlNormal3f( - 1.0f, 0.0f, 0.0f);    // Normal Pointing Left
	rlTexCoord2f(0.25f, 0.5f); rlVertex3f(x - width/2, y - height/2, z - length/2);  // Bottom Left Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.5f); rlVertex3f(x - width/2, y - height/2, z + length/2);  // Bottom Right Of The Texture and Quad
	rlTexCoord2f(0.0f, 0.25f); rlVertex3f(x - width/2, y + height/2, z + length/2);  // Top Right Of The Texture and Quad
	rlTexCoord2f(0.25f, 0.25f); rlVertex3f(x - width/2, y + height/2, z - length/2);  // Top Left Of The Texture and Quad
	rlEnd();
	
	rlSetTexture(0);
}
void DrawCubeTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
	float x = position.x;
	float y = position.y;
	float z = position.z;
	float texWidth = (float)texture.width;
	float texHeight = (float)texture.height;
	
	// Set desired texture to be enabled while drawing following vertex data
	rlSetTexture(texture.id);
	
	// We calculate the normalized texture coordinates for the desired texture-source-rectangle
	// It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
	rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
	
	// Front face
	rlNormal3f(0.0f, 0.0f, 1.0f);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	
	// Back face
	rlNormal3f(0.0f, 0.0f, - 1.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	
	// Top face
	rlNormal3f(0.0f, 1.0f, 0.0f);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	
	// Bottom face
	rlNormal3f(0.0f, - 1.0f, 0.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	
	// Right face
	rlNormal3f(1.0f, 0.0f, 0.0f);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z - length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x + width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x + width/2, y - height/2, z + length/2);
	
	// Left face
	rlNormal3f( - 1.0f, 0.0f, 0.0f);
	rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z - length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
	rlVertex3f(x - width/2, y - height/2, z + length/2);
	rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z + length/2);
	rlTexCoord2f(source.x/texWidth, source.y/texHeight);
	rlVertex3f(x - width/2, y + height/2, z - length/2);
	
	rlEnd();
	
	rlSetTexture(0);
}


//                       我的Raylib Minecraft源代码                       
//                         ===============                                 
//                        //          // |                                
//                       //          //  |        Be made from C++        
//                      =============|   |            Raylib( ^_^ )       
//                      |            |   |                                
//                      |            |   |                                
//                      |            | //                                 
//                      |            |//                                  
//                      =============//                                   
//
