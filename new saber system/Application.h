#ifndef APPLICATION_H
#define APPLICAITON_H
#include "olcPixelGameEngine.h"
#include "Body.h"
#include "Constants.h"
#include "Force.h"
#include "Graphics.h"
#include "CollisionDetection.h"
#include "Contact.h"
#include "ManipulatedSprite.h"



class Application
{
public:
	Application() = default;
	~Application() = default;

public:
	std::vector<Body*> Bodies;
	Vec2f pushForce = Vec2f(0,0);
	Vec2f* pSelected = nullptr;
	int pIndex = -1;
	float k = 500;
	float restLength = 200.0f;
	const int NUM_PARTICLES = 4;
	bool debug = false;
	Vec2f mousepressedlocation;
	Vec2f mousereleasedlocation;
	olc::Sprite* sprite;
	olc::Decal* Dsprite;
	ManipulatedSprite MS;
	int index = 0;
	int saberindex;
	std::vector<LineData> LineData;
	int bodynumber = 0;
	int mousenumber = 0;
public:

	void Setup();
	void Input(olc::PixelGameEngine* ptr);
	void Update(float deltatime, olc::PixelGameEngine* ptr);
	void Render(olc::PixelGameEngine* ptr);
	void Destroy();
	void DraweWireFrameModel(olc::PixelGameEngine* pge,
		const std::vector<Vec2f> vecmodelcoordinates,
		float x, float y,
		float r = 0.0f, float s = 1.0f,
		olc::Pixel p = olc::WHITE);
};



#endif // !APPLICATION_H


