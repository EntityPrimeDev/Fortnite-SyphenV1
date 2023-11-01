#include "win_utils.h"
#include "d3d9_x.h"
#include "xor.hpp"
#include <dwmapi.h>
#include <vector>
#include "godfather.h"
#include "skStr.h"
#include <string>
#include <iostream>
#include "skStr.h"
#include "Controller.h"
#include <Windows.h>
#include <string>
#include "auth.hpp"
#include <fstream>

#define OFFSET_UWORLD 0xcbf3538;


#define OFFSET_UWORLD 0xcbf3538

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;

std::string name = "Syphen Slotted"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "pqS83QAUXN"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "364cd0919364c75d3f448a60e5c9ff43502061d72ce0947abdd931a07b4b697d"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.1/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update it


api KeyAuthApp(name, ownerid, secret, version, url, sslPin);


bool ShowMenu = false;
bool Esp = true;
bool Esp_box = true;
bool Esp_fbox = false;
bool Esp_Distance = false;
bool while_rotating = false;
static int increased_value;
static bool Esp_Skeleton = true;
static bool carFly = false;
static bool backround = false;
float rapidFireVaule = 10;
static bool VBuck = false;
static bool playerFly = false;
bool Esp_line = true;
static bool Aimbot = true;
bool memoryaim = false;
bool debug = false;
static bool fovCircle = true;
bool speedhack = false;
float bA1mb0tSm00th1ngV4lue = 3;
static bool AimWhileJumping = false;
static bool NoEquipAnim = false;
static bool RapidFire = false;
static bool AirStuck = false;
static bool InstaRes = false;
static bool FovChanger = false;
static bool spinbot = false;
static bool doublePump = false;
static bool BulletTp = false;
static bool infAmmo = false;


ImFont* m_pFont;

static int VisDist = 500;
float AimFOV = 100;
float FOVChangerValue = 100;
static int aimDist = 200;
static int aimkey;
static int hitbox;
static int aimkeypos = 3;
static int hitboxpos = 0;

float speedVlaue = 10;
float FOVChangerValueX;
float FOVChangerValueY;
float FOVChangerValueZ;

DWORD_PTR Uworld;
DWORD_PTR LocalPawn;
DWORD_PTR PlayerState;
DWORD_PTR Localplayer;
DWORD_PTR Rootcomp;
DWORD_PTR PlayerController;
DWORD_PTR Persistentlevel;

Vector3 localactorpos;

uint64_t TargetPawn;
int localplayerID;

RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;

DWORD ScreenCenterX;
DWORD ScreenCenterY;

int CurrentActorId;

static std::string ReadGetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(base_address + 0xcbf3538c + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);
	uint16_t nameEntry = read<uint16_t>(NamePoolChunk);

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ((uint32_t)nameLength)
	{
		for (int x = 0; x < nameLength; ++x)
		{
			buff[x] = read<char>(NamePoolChunk + 4 + x);
		}

		char* v2 = buff;
		signed int v4 = nameLength;
		uint64_t result;
		uintptr_t  v5;
		uintptr_t v7;
		byte v8;
		uint64_t v9;

		v5 = 0;
		result = 30;

		if (v4 > 0)
		{
			do
			{
				v7 = (uintptr_t)(v5 | result);
				++v2;
				++v5;
				v8 = (byte)~(byte)v7;
				result = (uintptr_t)(2 * v7);
				*(v2 - 1) ^= v8;
			} while (v5 < v4);
		}
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else
	{
		return "";
	}
}

std::string GetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(base_address + 0xcbf3538 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);

	if (read<uint16_t>(NamePoolChunk) < 64)
	{
		DWORD a1 = read<DWORD>(NamePoolChunk + 4);
		return ReadGetNameFromFName(a1);
	}
	else
	{
		return ReadGetNameFromFName(key);
	}
}

static void xCreateWindow();
static void xInitD3d();
void DrawESP();
static void xMainLoop();
static void xShutdown();
static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
static LPDIRECT3DDEVICE9 D3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;

DWORD Menuthread(LPVOID in)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			ShowMenu = !ShowMenu;
		}
		Sleep(2);
	}
}

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray;
	bonearray = read<DWORD_PTR>(mesh + 0x590);

	if (bonearray == NULL)
	{
		bonearray = read<DWORD_PTR>(mesh + 0x590);
	}
	return read<FTransform>(bonearray + (index * 0x60));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

struct CamewaDescwipsion
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};

CamewaDescwipsion UndetectedCamera(__int64 a1)
{
	CamewaDescwipsion VirtualCamera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;

	v1 = read<__int64>(Localplayer + 0xC8);
	__int64 v9 = read<__int64>(v1 + 8);

	VirtualCamera.FieldOfView = 80.f / (float)((float)read<double>(v9 + 0x690) / 1.19f);

	VirtualCamera.Rotation.x = read<double>(v9 + 0x7E0);
	VirtualCamera.Rotation.y = read<double>(a1 + 0x148);

	v6 = read<__int64>(Localplayer + 0x70);
	v7 = read<__int64>(v6 + 0x98);
	v8 = read<__int64>(v7 + 0xF8);

	VirtualCamera.Location = read<Vector3>(v8 + 0x20);
	return VirtualCamera;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	CamewaDescwipsion vCamera = UndetectedCamera(Rootcomp);
	vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);

	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}

void DrawStrokeText(int x, int y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}

float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	return y;
}

void DrawText1(int x, int y, const char* str, RGBA* color)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}

void DrawBox(float X, float Y, float W, float H, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	//black outlines
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void DrawCrossHair()
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(ScreenCenterX, ScreenCenterY), 5, ImColor(255, 255, 255, 255));
}

void DrawFilledRect(int x, int y, int w, int h, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}

void DrawLeftProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x - (w / 2) - 3, y, thick, (h)*m_health / 100, &healthcol);
}
void DrawRightProgressBar(int x, int y, int w, int h, int thick, int m_health)
{
	int G = (255 * m_health / 100);
	int R = 255 - G;
	RGBA healthcol = { R, G, 0, 255 };

	DrawFilledRect(x + (w / 2) - 25, y, thick, (h)*m_health / 100, &healthcol);
}

typedef struct _FNlEntity
{
	uint64_t Actor;
	int ID;
	uint64_t mesh;
}FNlEntity;

std::vector<FNlEntity> entityList;


void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}



using namespace std;



int main(int argc, const char* argv[])
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, 11);


	SetConsoleTitleA(skCrypt("Loader"));
	std::cout << skCrypt("\n\n Connecting..");
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}


	std::string key;

	clear();

	std::cout << "[+] Loading Syphen Loader External...";



	Sleep(10000);

	clear();


	SetConsoleTextAttribute(hConsole, 12);
	std::cout << "E";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "n";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "t";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << "e";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 13);
	std::cout << "r";
	Sleep(100);
	std::cout << " ";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 12);
	std::cout << "K";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "e";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "y";
	Sleep(100);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << ":  ";
	Sleep(100);

	std::cin >> key;
	KeyAuthApp.license(key);


	 if (!KeyAuthApp.data.success)
	 {
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	 }

	clear();

	std::cout << "[+] Please Open Fortnite!";

	CreateThread(NULL, NULL, Menuthread, NULL, NULL, NULL);

	while (hwnd == NULL)
	{
		XorS(wind, "Fortnite  ");
		hwnd = FindWindowA(0, wind.decrypt());
		Sleep(100);
	}
	GetWindowThreadProcessId(hwnd, &processID);

	// ADD YOUR GET BASE ADDRESS
	if (driver->Init(FALSE)) {

		driver->Attach(processID);

		base_address = driver->GetModuleBase(L"FortniteClient-Win64-Shipping.exe");

	}


	clear();
	XorS(base, "[+] Fortnites Id Base Is: %p.\n");
	printf(base.decrypt(), (void*)base_address);
	std::cout << "\nIf The Id Base Is All 0'S Reboot Pc And Redo All The Steps";



	xCreateWindow();
	xInitD3d();

	xMainLoop();
	xShutdown();

	return 0;
}

void SetWindowToTarget()
{
	while (true)
	{
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);
		}
		else
		{
			exit(0);
		}
	}
}

const MARGINS Margin = { -1 };

void xCreateWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = "Windows18";
	wc.lpfnWndProc = WinProc;
	RegisterClassEx(&wc);

	if (hwnd)
	{
		GetClientRect(hwnd, &GameRect);
		POINT xy;
		ClientToScreen(hwnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else
		exit(2);

	Window = CreateWindowEx(NULL, "Windows18", "Windows189", WS_POPUP | WS_VISIBLE, 0, 0, Width, Height, 0, 0, 0, 0);

	DwmExtendFrameIntoClientArea(Window, &Margin);
	SetWindowLong(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);
}

void xInitD3d()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = Window;
	d3dpp.Windowed = TRUE;

	p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(D3dDevice);



	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();


	style->WindowRounding = 3;
	style->GrabRounding = 1;
	style->GrabMinSize = 20;
	style->FrameRounding = 3;


	style->Colors[ImGuiCol_Text] = ImColor(189, 5, 188);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_Border] = ImColor(0, 0, 0);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	style->Colors[ImGuiCol_TitleBg] = ImColor(8, 54, 88);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(30, 28, 32);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(30, 28, 32);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(189, 5, 188);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(189, 5, 188);
	style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(78, 27, 116);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0);
	style->Colors[ImGuiCol_Header] = ImColor(13, 12, 13);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(13, 12, 13);
	style->Colors[ImGuiCol_Column] = ImColor(37, 37, 37);
	style->Colors[ImGuiCol_ColumnHovered] = ImColor(37, 37, 37);
	style->Colors[ImGuiCol_ColumnActive] = ImColor(37, 37, 37);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);

	XorS(font, "C:\\Windows\\Fonts\\Arial.ttf");
	m_pFont = io.Fonts->AddFontFromFileTTF(font.decrypt(), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

	p_Object->Release();
}

void aimbot(float x, float y)
{
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);
	int AimSpeed = bA1mb0tSm00th1ngV4lue;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	//WriteAngles(TargetX / 3.5f, TargetY / 3.5f);
	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	return;
}

struct FZiplinePawnState {
	struct AFortAthenaZiplineBase* Zipline; // 0x00(0x08)
	struct AFortAthenaZiplineBase* PreviousZipline; // 0x00(0x08)
	struct UPrimitiveComponent* InteractComponent; // 0x10(0x08)
	bool bIsZiplining; // 0x18(0x01)
	bool bJumped; // 0x19(0x01)
	bool bReachedEnd; // 0x1a(0x01)
	char pad_1B[0x1]; // 0x1b(0x01)
	int32_t AuthoritativeValue; // 0x1c(0x04)struct sdk::structs::FVector SocketOffset; // 0x20(0x0c)
	float TimeZipliningBegan; // 0x2c(0x04)
	float TimeZipliningEndedFromJump; // 0x30(0x04)
	char pad_34[0x4]; // 0x34(0x04)
};


void AimAt(DWORD_PTR entity)
{
	uint64_t currentactormesh = read<uint64_t>(entity + 0x2F0);
	auto rootHead = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);

	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{
		aimbot(rootHeadOut.x, rootHeadOut.y);
	}
}

RGBA FovColor = Col.purple;
RGBA SnapLineColor = Col.blue;
RGBA BoxColor = Col.red;
float FovThickness = 25;


bool once = false;
namespace Framework {
	namespace Structs {


		struct FMatrix
		{
			float M[4][4];
		};
		static FMatrix* myMatrix = new FMatrix();


		template<class T>
		struct TArray
		{
			friend struct FString;

		public:
			inline TArray()
			{
				Data = nullptr;
				Count = Max = 0;
			};

			inline int Num() const
			{
				return Count;
			};

			inline T& operator[](int i)
			{
				return Data[i];
			};

			inline const T& operator[](int i) const
			{
				return Data[i];
			};

			inline bool IsValidIndex(int i) const
			{
				return i < Num();
			}

		private:
			T* Data;
			int32_t Count;
			int32_t Max;
		};

		struct FString : private TArray<wchar_t>
		{
			inline FString()
			{
			};

			FString(const wchar_t* other)
			{
				Max = Count = *other ? std::wcslen(other) + 1 : 0;

				if (Count)
				{
					Data = const_cast<wchar_t*>(other);
				}
			};

			inline bool IsValid() const
			{
				return Data != nullptr;
			}

			inline const wchar_t* c_str() const
			{
				return Data;
			}

			std::string ToString() const
			{
				auto length = std::wcslen(Data);
				std::string str(length, '\0');
				std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);
				return str;
			}
		};
	}

}


void DrawESP() {

	if (fovCircle)
	{
		DrawCircle(ScreenCenterX, ScreenCenterY, AimFOV, &FovColor, 25);
	}
	
	XorS(frame, "%.1f Fps\n");
	char dist[360];
	sprintf_s(dist, frame.decrypt(), ImGui::GetIO().Framerate);
	DrawText1(15, 15, dist, &Col.darkgreen_);

	if (hitboxpos == 0)
	{
		hitbox = 66; //head
	}
	else if (hitboxpos == 1)
	{
		hitbox = 65; //neck
	}
	else if (hitboxpos == 2)
	{
		hitbox = 7; //chest
	}
	else if (hitboxpos == 3)
	{
		hitbox = 2; //pelvis
	}

	if (aimkeypos == 0)
	{
		aimkey = 0x01;//left mouse button
	}
	else if (aimkeypos == 1)
	{
		aimkey = 0x02;//right mouse button
	}
	else if (aimkeypos == 2)
	{
		aimkey = 0x04;//middle mouse button
	}
	else if (aimkeypos == 3)
	{
		aimkey = 0x05;//x1 mouse button
	}
	else if (aimkeypos == 4)
	{
		aimkey = 0x06;//x2 mouse button
	}
	else if (aimkeypos == 5)
	{
		aimkey = 0x03;//control break processing
	}
	else if (aimkeypos == 6)
	{
		aimkey = 0x08;//backspace
	}
	else if (aimkeypos == 7)
	{
		aimkey = 0x09;//tab
	}
	else if (aimkeypos == 8)
	{
		aimkey = 0x0c;//clear
	}
	else if (aimkeypos == 9)
	{
		aimkey == 0x0D;//enter
	}
	else if (aimkeypos == 10)
	{
		aimkey = 0x10;//shift
	}
	else if (aimkeypos == 11)
	{
		aimkey = 0x11;//ctrl
	}
	else if (aimkeypos == 12)
	{
		aimkey == 0x12;//alt
	}
	else if (aimkeypos == 13)
	{
		aimkey == 0x14;//caps lock
	}
	else if (aimkeypos == 14)
	{
		aimkey == 0x1B;//esc
	}
	else if (aimkeypos == 15)
	{
		aimkey == 0x20;//space
	}
	else if (aimkeypos == 16)
	{
		aimkey == 0x30;//0
	}
	else if (aimkeypos == 17)
	{
		aimkey == 0x31;//1
	}
	else if (aimkeypos == 18)
	{
		aimkey == 0x32;//2
	}
	else if (aimkeypos == 19)
	{
		aimkey == 0x33;//3
	}
	else if (aimkeypos == 20)
	{
		aimkey == 0x34;//4
	}
	else if (aimkeypos == 21)
	{
		aimkey == 0x35;//5
	}
	else if (aimkeypos == 22)
	{
		aimkey == 0x36;//6
	}
	else if (aimkeypos == 23)
	{
		aimkey == 0x37;//7
	}
	else if (aimkeypos == 24)
	{
		aimkey == 0x38;//8
	}
	else if (aimkeypos == 25)
	{
		aimkey == 0x39;//9
	}
	else if (aimkeypos == 26)
	{
		aimkey == 0x41;//a
	}
	else if (aimkeypos == 27)
	{
		aimkey == 0x42;//b
	}
	else if (aimkeypos == 28)
	{
		aimkey == 0x43;//c
	}
	else if (aimkeypos == 29)
	{
		aimkey == 0x44;//d
	}
	else if (aimkeypos == 30)
	{
		aimkey == 0x45;//e
	}
	else if (aimkeypos == 31)
	{
		aimkey == 0x46;//f
	}
	else if (aimkeypos == 32)
	{
		aimkey == 0x47;//g
	}
	else if (aimkeypos == 33)
	{
		aimkey == 0x48;//h
	}
	else if (aimkeypos == 34)
	{
		aimkey == 0x49;//i
	}
	else if (aimkeypos == 35)
	{
		aimkey == 0x4A;//j
	}
	else if (aimkeypos == 36)
	{
		aimkey == 0x4B;//k
	}
	else if (aimkeypos == 37)
	{
		aimkey == 0x4C;//L
	}
	else if (aimkeypos == 38)
	{
		aimkey == 0x4D;//m
	}
	else if (aimkeypos == 39)
	{
		aimkey == 0x4E;//n
	}
	else if (aimkeypos == 40)
	{
		aimkey == 0x4F;//o
	}
	else if (aimkeypos == 41)
	{
		aimkey == 0x50;//p
	}
	else if (aimkeypos == 42)
	{
		aimkey == 0x51;//q
	}
	else if (aimkeypos == 43)
	{
		aimkey == 0x52;//r
	}
	else if (aimkeypos == 44)
	{
		aimkey == 0x53;//s
	}
	else if (aimkeypos == 45)
	{
		aimkey == 0x54;//t
	}
	else if (aimkeypos == 46)
	{
		aimkey == 0x55;//u
	}
	else if (aimkeypos == 47)
	{
		aimkey == 0x56;//v
	}
	else if (aimkeypos == 48)
	{
		aimkey == 0x57;//w
	}
	else if (aimkeypos == 49)
	{
		aimkey == 0x58;//x
	}
	else if (aimkeypos == 50)
	{
		aimkey == 0x59;//y
	}
	else if (aimkeypos == 51)
	{
		aimkey == 0x5A;//z
	}
	else if (aimkeypos == 52)
	{
		aimkey == 0x60;//numpad 0
	}
	else if (aimkeypos == 53)
	{
		aimkey == 0x61;//numpad 1
	}
	else if (aimkeypos == 54)
	{
		aimkey == 0x62;//numpad 2
	}
	else if (aimkeypos == 55)
	{
		aimkey == 0x63;//numpad 3
	}
	else if (aimkeypos == 56)
	{
		aimkey == 0x64;//numpad 4
	}
	else if (aimkeypos == 57)
	{
		aimkey == 0x65;//numpad 5
	}
	else if (aimkeypos == 58)
	{
		aimkey == 0x66;//numpad 6
	}
	else if (aimkeypos == 59)
	{
		aimkey == 0x67;//numpad 7
	}
	else if (aimkeypos == 60)
	{
		aimkey == 0x68;//numpad 8
	}
	else if (aimkeypos == 61)
	{
		aimkey == 0x69;//numpad 9
	}
	else if (aimkeypos == 62)
	{
		aimkey == 0x6A;//multiply
	}

	auto entityListCopy = entityList;
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;
	Uworld = read<DWORD_PTR>(base_address + 0xcbf3538);
	DWORD_PTR Gameinstance = read<DWORD_PTR>(Uworld + 0x1A8);
	DWORD_PTR LocalPlayers = read<DWORD_PTR>(Gameinstance + 0x38);
	Localplayer = read<DWORD_PTR>(LocalPlayers);
	PlayerController = read<DWORD_PTR>(Localplayer + 0x30);
	LocalPawn = read<DWORD_PTR>(PlayerController + 0x310);
	DWORD_PTR CurrentWeapon = read<DWORD_PTR>(Localplayer + 0x550);
	PlayerState = read<DWORD_PTR>(LocalPawn + 0x290);
	Rootcomp = read<DWORD_PTR>(LocalPawn + 0x188);
	Persistentlevel = read<DWORD_PTR>(Uworld + 0x30);
	DWORD ActorCount = read<DWORD>(Persistentlevel + 0xA0);
	DWORD_PTR AActors = read<DWORD_PTR>(Persistentlevel + 0x98);
	PVOID TargetPawn = nullptr;

	for (int i = 0; i < ActorCount; i++)
	{
		uint64_t CurrentActor = read<uint64_t>(AActors + i * 0x8);
		CurrentActorId = read<int>(CurrentActor + 0x18);
		if (read<float>(CurrentActor + 0x4060) != 10) continue;
		uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + 0x2F0);
		int MyTeamId = read<int>(PlayerState + 0x1048);
		DWORD64 otherPlayerState = read<uint64_t>(CurrentActor + 0x290);
		int ActorTeamId = read<int>(otherPlayerState + 0x1048);
		int isBot = read<int>(Localplayer + 0x27a);
		if (MyTeamId == ActorTeamId) continue;
		if (CurrentActor == LocalPawn) continue;

		Vector3 Headpos = GetBoneWithRotation(CurrentActorMesh, 98);
		localactorpos = read<Vector3>(Rootcomp + 0x128);

		float distance = localactorpos.Distance(Headpos) / 100.f;

		Vector3 bone0 = GetBoneWithRotation(CurrentActorMesh, 0);
		Vector3 bottom = ProjectWorldToScreen(bone0);
		Vector3 Headbox = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		Vector3 w2shead = ProjectWorldToScreen(Headpos);


		float BoxHeight = (float)(Headbox.y - bottom.y);
		float BoxWidth = BoxHeight * 0.380f;

		float LeftX = (float)Headbox.x - (BoxWidth / 1);
		float LeftY = (float)bottom.y;

		float CornerHeight = abs(Headbox.y - bottom.y);
		float CornerWidth = CornerHeight * 0.75;



		if (AimWhileJumping) { //Allows you to ADS when You are in the Air
			write<bool>(LocalPawn + 0x4915, true); //bADSWhileNotOnGround
		}
		else {
			write<bool>(LocalPawn + 0x4915, false); //bADSWhileNotOnGround
		}

		if (AirStuck) {  //Freezes You in the Air
			if (GetAsyncKeyState(VK_MENU), Controller::IsPressingLeftTrigger()) { //Alt Keybind
				write<float>(LocalPawn + 0x64, 0); //CustomTimeDilation
			}
			else {
				write<float>(LocalPawn + 0x64, 1); //CustomTimeDilation
			}
		}
		if (InstaRes) { //Revives Teammates in 1 Second
			write<float>(LocalPawn + 0x4060, 1); //ReviveFromDBNOTime
		}


		if (spinbot)
		{
			if (GetAsyncKeyState(VK_RBUTTON) or Controller::IsPressingRightTrigger())
			{
				while_rotating = true;

				if (LocalPawn)
				{

					uint64_t MeshA = read<uint64_t>(LocalPawn + 0x2F0);
					increased_value = increased_value + 100;
					write<Vector3>(MeshA + 0x140, Vector3(1, increased_value, 1));
				}
			}
			else if (!GetAsyncKeyState(VK_RBUTTON) or Controller::IsPressingRightTrigger())
			{
				if (LocalPawn)
				{
					if (while_rotating)
					{
						uint64_t MeshA = read<uint64_t>(LocalPawn + 0x2F0);
						write<Vector3>(MeshA + 0x140, Vector3(0, -90, 0));
						while_rotating = false;
					}
				}
			}
		}

		if (carFly)
		{
			if (LocalPawn)
			{
				uint64_t VEHICLE_STATS = read<uint64_t>(LocalPawn + 0x21B8); //AFortPlayerPawn    CurrentVehicle    0x21b8    AActor *
				if (GetAsyncKeyState(VK_SPACE))write<char>(VEHICLE_STATS + 0x672, 1);//AFortAthenaVehicle    bUseGravity : 1    0x672    char
			}
		}
	

		if (distance < VisDist)
		{
			if (Esp)
			{

				Vector3 LookingAtPoint;
				bool IsVisible;
				bool VisibleCheck = false;

				if (VisibleCheck) {
					IsVisible = (PVOID)PlayerController, (PVOID)CurrentActor, &LookingAtPoint;
				}

				if (Esp_box)
				{

					if (Esp_Distance)
					{
	
							DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, IM_COL32(221, 160, 221, 160), 1.5);

							XorS(dst, "Distance [%.fm]\n Player\n");
							char dist[64];
							sprintf_s(dist, dst.decrypt(), distance);
							DrawOutlinedText(m_pFont, dist, ImVec2(Headbox.x, Headbox.y - 35), 16.0f, IM_COL32(0, 255, 0, 255), true);
					}
				}

				if (Esp_fbox)
				{
					DrawBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, &BoxColor);

					if (Esp_Distance)
					{
						XorS(dst, "Distance [%.fm]\n Player\n");
						char dist[64];
						sprintf_s(dist, dst.decrypt(), distance);
						DrawOutlinedText(m_pFont, dist, ImVec2(Headbox.x, Headbox.y - 35), 16.0f, IM_COL32(0, 255, 0, 255), true);
					}
				}

				if (Esp_line)
				{

					DrawLine(Width / 2, Height, bottom.x, bottom.y, &SnapLineColor, 1.5);

				}
				if (Esp_Skeleton)
				{
					{

						Vector3 vHeadBoneOut, vNeckOut, vPelvisOut;
						Vector3 vUpperArmLeftOut, vLeftHandOut, vlowerarmLeftOut;
						Vector3 vUpperArmRightOut, vLowerarmRightOut, vRightHandOut;
						Vector3 vLeftThighOut, vLeftCalfOut, vLeftLballOut, vLeftFootOut;
						Vector3 vRightThighOut, vRightCalfOut, vRightLballut, vRightFootOut;

						Vector3 vHeadBone = GetBoneWithRotation(CurrentActorMesh, 68);
						vHeadBoneOut = ProjectWorldToScreen(vHeadBone);
						Vector3 vNeck = GetBoneWithRotation(CurrentActorMesh, 67);
						vNeckOut = ProjectWorldToScreen(vNeck);
						Vector3 vPelvis = GetBoneWithRotation(CurrentActorMesh, 2);
						vPelvisOut = ProjectWorldToScreen(vPelvis);

						Vector3 vLeftUpperArm = GetBoneWithRotation(CurrentActorMesh, 9);
						vUpperArmLeftOut = ProjectWorldToScreen(vLeftUpperArm);
						Vector3 vLeftlowerarm = GetBoneWithRotation(CurrentActorMesh, 10);
						vlowerarmLeftOut = ProjectWorldToScreen(vLeftlowerarm);
						Vector3 vLeftHand = GetBoneWithRotation(CurrentActorMesh, 11);
						vLeftHandOut = ProjectWorldToScreen(vLeftHand);

						Vector3 vRightUpperArm = GetBoneWithRotation(CurrentActorMesh, 38);
						vUpperArmRightOut = ProjectWorldToScreen(vRightUpperArm);
						Vector3 vRightLowerarm = GetBoneWithRotation(CurrentActorMesh, 39);
						vLowerarmRightOut = ProjectWorldToScreen(vRightLowerarm);
						Vector3 vRightHand = GetBoneWithRotation(CurrentActorMesh, 40);
						vRightHandOut = ProjectWorldToScreen(vRightHand);

						Vector3 vLeftThigh = GetBoneWithRotation(CurrentActorMesh, 69);
						vLeftThighOut = ProjectWorldToScreen(vLeftThigh);
						Vector3 vLeftCalf = GetBoneWithRotation(CurrentActorMesh, 70);
						vLeftCalfOut = ProjectWorldToScreen(vLeftCalf);
						Vector3 vLeftFoot = GetBoneWithRotation(CurrentActorMesh, 73);
						vLeftFootOut = ProjectWorldToScreen(vLeftFoot);
						Vector3 vLeftLball = GetBoneWithRotation(CurrentActorMesh, 74);
						vLeftLballOut = ProjectWorldToScreen(vLeftLball);

						Vector3 vRightThigh = GetBoneWithRotation(CurrentActorMesh, 76);
						vRightThighOut = ProjectWorldToScreen(vRightThigh);
						Vector3 vRightCalf = GetBoneWithRotation(CurrentActorMesh, 77);
						vRightCalfOut = ProjectWorldToScreen(vRightCalf);
						Vector3 vRightFoot = GetBoneWithRotation(CurrentActorMesh, 80);
						vRightFootOut = ProjectWorldToScreen(vRightFoot);
						Vector3 vRightLball = GetBoneWithRotation(CurrentActorMesh, 81);
						vRightLballut = ProjectWorldToScreen(vRightLball);

						DrawLine(vNeckOut.x, vNeckOut.y, vHeadBoneOut.x, vHeadBoneOut.y, &Col.white, 1.5);
						DrawLine(vPelvisOut.x, vPelvisOut.y, vNeckOut.x, vNeckOut.y, &Col.white, 1.5);

						DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.white, 1.5);
						DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.white, 1.5);

						DrawLine(vlowerarmLeftOut.x, vlowerarmLeftOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.white, 1.5);
						DrawLine(vLowerarmRightOut.x, vLowerarmRightOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.white, 1.5);

						DrawLine(vLeftHandOut.x, vLeftHandOut.y, vlowerarmLeftOut.x, vlowerarmLeftOut.y, &Col.white, 1.5);
						DrawLine(vRightHandOut.x, vRightHandOut.y, vLowerarmRightOut.x, vLowerarmRightOut.y, &Col.white, 1.5);

						DrawLine(vLeftThighOut.x, vLeftThighOut.y, vPelvisOut.x, vPelvisOut.y, &Col.white, 1.5);
						DrawLine(vRightThighOut.x, vRightThighOut.y, vPelvisOut.x, vPelvisOut.y, &Col.white, 1.5);

						DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.white, 1.5);
						DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.white, 1.5);

						DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &Col.white, 1.5);
						DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &Col.white, 1.5);

						DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftLballOut.x, vLeftLballOut.y, &Col.white, 1.5);
						DrawLine(vRightFootOut.x, vRightFootOut.y, vRightLballut.x, vRightLballut.y, &Col.white, 1.5);
					}
				}

			}
		}

		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);

		if (dist < AimFOV && dist < closestDistance) {
			closestDistance = dist;
			closestPawn = CurrentActor;
		}

	}

	if (Aimbot)
	{
	     if (Aimbot && closestPawn && GetAsyncKeyState(aimkey) < 0 or Controller::IsPressingLeftTrigger()) {
			AimAt(closestPawn);
		}
	}
	Sleep(1);
}

void render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ShowMenu)
	{
		XorS(menu, "Syphen Private");
		XorS(enable_esp, "Enable Esp");
		XorS(box_esp, "Box Esp");
		XorS(fullboxesp, "Full Box ESP");
		XorS(snapline, "Snap line");
		XorS(espdis, "Esp Distance");
		XorS(espvis, "Esp Visable");
		XorS(es5, "Max Visuals Distance");
		XorS(espskel, "ESP Skeleton");

		XorS(aim1, "Aimbot");
		XorS(aim2, "Aimbot Key");
		XorS(aim3, "Aimbot Bone");
		XorS(aim7, "Enable FOV");
		XorS(aim6, "Aimbot FOV");


		ImGui::Begin(menu.decrypt(), &ShowMenu, ImVec2(700, 350), 1.0f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		static int switchTabs = 5;


		if (ImGui::Button("Aimbot", ImVec2(120 - 15, 39)))
			switchTabs = 0;
		ImGui::SameLine();
		if (ImGui::Button("Visuals", ImVec2(120 - 15, 39)))
			switchTabs = 1;
		ImGui::SameLine();
		if (ImGui::Button("Misc & Exploits", ImVec2(120 - 15, 39)))
			switchTabs = 2;
		ImGui::SameLine();
		if (ImGui::Button("Color", ImVec2(120 - 15, 39)))
			switchTabs = 3;
		ImGui::SameLine();
		if (ImGui::Button("Keybinds", ImVec2(120 - 15, 39)))
			switchTabs = 4;


	

	static bool fovCirclePurple = true;
	static bool fovCircleGreen = false;
	static bool fovCircleBlue = false;
	static bool fovCircleLightBlue = false;

	static bool fovCircleOrange = false;
	static bool fovCircleRed = false;
	static bool fovCircleDarkGreen = false;
	static bool fovCircleScarlet = false;

	static bool snapLineColBlue = true;
	static bool snapLineColPurple = false;
	static bool snapLineColRed = false;
	static bool snapLineColOrange = false;
	static bool snapLineColScarlet = false;
	static bool snapLineColGreen = false;

	static bool fullBoxColRed = true;
	static bool fullBoxColPurple = false;
	static bool fullBoxColBlue = false;

	static bool fullBoxColGreen = false;
	static bool fullBoxColOrange = false;
	static bool fullBoxColDarkGreen = false;

	switch (switchTabs) {
	 case 0:
		ImGui::Checkbox(aim1.decrypt(), &Aimbot);
		ImGui::Spacing();
		ImGui::Checkbox(aim7.decrypt(), &fovCircle);
		ImGui::Spacing();
		ImGui::SliderFloat(aim6.decrypt(), &AimFOV, 25.f, 1500.f);
		ImGui::Spacing();
		ImGui::SliderFloat("Aimbot Smooth", &bA1mb0tSm00th1ngV4lue, 2.5f, 15.f);
		ImGui::Spacing();
		ImGui::Combo(aim3.decrypt(), &hitboxpos, hitboxes, sizeof(hitboxes) / sizeof(*hitboxes));
		ImGui::Spacing();
		ImGui::Combo(aim2.decrypt(), &aimkeypos, aimkeys, sizeof(aimkeys) / sizeof(*aimkeys));
		break;
	 case 1:
		ImGui::Checkbox(enable_esp.decrypt(), &Esp);
		ImGui::Spacing();
		ImGui::Checkbox(box_esp.decrypt(), &Esp_box);
		ImGui::Spacing();
		ImGui::Checkbox(fullboxesp.decrypt(), &Esp_fbox);
		ImGui::Spacing();
		ImGui::Checkbox(espdis.decrypt(), &Esp_Distance);
		ImGui::Spacing();
		ImGui::Checkbox(snapline.decrypt(), &Esp_line);
		ImGui::Spacing();
		ImGui::Checkbox(espskel.decrypt(), &Esp_Skeleton);
		ImGui::Spacing();
		ImGui::Text(es5.decrypt());
		ImGui::SliderInt("", &VisDist, 50, 500);
		break;
	 case 2:
		ImGui::Text("This Has More Of A Risk An Can Get You Banned!");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Insta Revive", &InstaRes);
		ImGui::Spacing();
		ImGui::Checkbox("Air Stuck", &AirStuck);
		ImGui::Spacing();
		ImGui::Checkbox("Aim While Jumping", &AimWhileJumping);
		ImGui::Spacing();
		ImGui::Checkbox("Spinbot", &spinbot);
		ImGui::Spacing();
		ImGui::Checkbox("Car Fly", &carFly);
		break;
	 case 3:
		ImGui::Text("Select A Color!");
		ImGui::Checkbox("Fov Circle - Purple", &fovCirclePurple);
		ImGui::SameLine();
		ImGui::Checkbox("Fov Circle - Green", &fovCircleGreen);
		ImGui::Checkbox("Fov Circle - Blue", &fovCircleBlue);
		ImGui::SameLine();
		ImGui::Checkbox("Fov Circle - Light Blue", &fovCircleLightBlue);
		ImGui::Checkbox("Fov Circle - Orange", &fovCircleOrange);
		ImGui::SameLine();
		ImGui::Checkbox("Fov Circle - Red", &fovCircleRed);
		ImGui::Checkbox("Fov Circle - Dark Green", &fovCircleDarkGreen);
		ImGui::SameLine();
		ImGui::Checkbox("Fov Circle - Scarlet", &fovCircleScarlet);
		ImGui::SameLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Checkbox("Snapline - Blue", &snapLineColBlue);
		ImGui::SameLine();
		ImGui::Checkbox("Snapline - Purple", &snapLineColPurple);
		ImGui::Checkbox("Snapline - Red", &snapLineColRed);
		ImGui::SameLine();
		ImGui::Checkbox("Snapline - Orange", &snapLineColOrange);
		ImGui::Checkbox("Snapline - Scarlet", &snapLineColScarlet);
		ImGui::SameLine();
		ImGui::Checkbox("Snapline - Green", &snapLineColGreen);
		ImGui::SameLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Checkbox("Box - Red", &fullBoxColRed);
		ImGui::SameLine();
		ImGui::Checkbox("Box - Purple", &fullBoxColPurple);
		ImGui::Checkbox("Box - Blue", &fullBoxColBlue);
		ImGui::SameLine();
		ImGui::Checkbox("Box - Green", &fullBoxColGreen);
		ImGui::Checkbox("Box - Orange", &fullBoxColOrange);
		ImGui::SameLine();
		ImGui::Checkbox("Box - Dark Green", &fullBoxColDarkGreen);

		break;
	case 4:
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Controller (Xbox only): ");
		ImGui::Spacing();
		ImGui::Text("Aimbot - Left Controller Bind");
		ImGui::Text("Spin Bot - Left Controller Bind");
		ImGui::Text("Insta Res - No Binds Set To It");
		ImGui::Text("Car Fly - Does Not Support Controller");
		ImGui::Text("The Rest Dont Have A Bind - No Binds Set To It");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("PC: ");
		ImGui::Spacing();
		ImGui::Text("Aimbot - Any Bind You Want");
		ImGui::Text("Spin Bot - Right Mouse Button");
		ImGui::Text("Insta Res - No Binds Set To It");
		ImGui::Text("Car Fly - Space Key");
		ImGui::Text("The Rest Dont Have A Bind - No Binds Set To It");
	}

	if (fovCirclePurple)
	{
		FovColor = Col.purple;
	}
	if (fovCircleBlue)
	{
		FovColor = Col.blue;
	}
	if (fovCircleLightBlue)
	{
		FovColor = Col.lightblue;
	}
	if (fovCircleOrange)
	{
		FovColor = Col.orange;
	}
	if (fovCircleRed)
	{
		FovColor = Col.red;
	}
	if (fovCircleDarkGreen)
	{
		FovColor = Col.darkgreen;
	}
	if (fovCircleGreen)
	{
		FovColor = Col.green;
	}
	if (fovCircleScarlet)
	{
		FovColor = Col.Scarlet;
	}


	if (snapLineColBlue)
	{
		SnapLineColor = Col.blue;
	}
	if (snapLineColPurple)
	{
		SnapLineColor = Col.purple;
	}
	if (snapLineColRed)
	{
		SnapLineColor = Col.red;
	}
	if (snapLineColOrange)
	{
		SnapLineColor = Col.orange;
	}
	if (snapLineColScarlet)
	{
		SnapLineColor = Col.Scarlet;
	}
	if (snapLineColGreen)
	{
		SnapLineColor = Col.green;
	}


	if (fullBoxColRed)
	{
		BoxColor = Col.red;
	}
	if (fullBoxColPurple)
	{
		BoxColor = Col.purple;
	}
	if (fullBoxColBlue)
	{
		BoxColor = Col.blue;
	}
	if (fullBoxColGreen)
	{
		BoxColor = Col.green;
	}
	if (fullBoxColOrange)
	{
		BoxColor = Col.orange;
	}
	if (fullBoxColDarkGreen)
	{
		BoxColor = Col.darkgreen;
	}
	ImGui::End();
	};
	
	DrawESP();

	ImGui::EndFrame();
	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}
	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		D3dDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}


MSG Message = { NULL };

void xMainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == hwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			d3dpp.BackBufferWidth = Width;
			d3dpp.BackBufferHeight = Height;
			SetWindowPos(Window, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			D3dDevice->Reset(&d3dpp);
		}
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(Window);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		xShutdown();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = D3dDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void xShutdown()
{
	TriBuf->Release();
	D3dDevice->Release();
	p_Object->Release();

	DestroyWindow(Window);
	UnregisterClass("fgers", NULL);
} 
