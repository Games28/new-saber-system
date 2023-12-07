#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Application.h"


class physics_system : public olc::PixelGameEngine
{
public:
	physics_system()
	{
		sAppName = "physics system";

	}

private:

public:
	Application app;
 
public:

	bool OnUserCreate() override
	{
		app.Setup();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);

		app.Input(this);
		app.Update(fElapsedTime,this);
		app.Render(this);


		return true;
	}

	bool OnUserDestroy() override
	{
		app.Destroy();
		return true;
	}
};


int main()
{
	physics_system demo;
	if (demo.Construct(1200, 800, 1, 1))
		demo.Start();

	return 0;
}