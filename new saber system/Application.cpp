#include "Application.h"

void Application::Setup()
{
	Body* floor = new Body(BoxShape(800 - 50, 50), 0, 400, 500, 0.0f);
	floor->restitution = 0.5f;
	floor->movementstatic = true;
	floor->rotationstatic = true;
	floor->SetTexture("crate.png");
	Bodies.push_back(floor);
	Body* Trooper1 = new Body(BoxShape(162, 273) ,0, 200, 300, 1.0f);
	Trooper1->SetTexture("trooper.png");
	Bodies.push_back(Trooper1);
	Body* Trooper2 = new Body(BoxShape(162, 273), 1, 700, 300, 1.0f);
	Trooper2->SetTexture("trooper.png");
	Bodies.push_back(Trooper2);

	

	Body* Saber = new Body(BoxShape(32, 402), 3, 400, 300, 0.0f);
	Saber->movementstatic = true;
	
	Saber->SetTexture("saber.png");
	//Bodies.push_back(Saber);
	
	//saberindex = Bodies.size() - 1;
	for (int i = 0; i < Bodies.size(); i++)
	{
		MS.Setup(Bodies[i]);
	}
	
}

void Application::Input(olc::PixelGameEngine* ptr)
{
	if (ptr->GetKey(olc::W).bHeld)
		Bodies[1]->velocity.y = -50;
	if (ptr->GetKey(olc::S).bHeld)
		Bodies[1]->velocity.y = +50;
	if (ptr->GetKey(olc::A).bHeld)
		Bodies[1]->velocity.x = -50;
	if (ptr->GetKey(olc::D).bHeld)
		Bodies[1]->velocity.x = +50;

	if (ptr->GetKey(olc::W).bReleased)
		Bodies[1]->velocity.y = 0;
	if (ptr->GetKey(olc::S).bReleased)
		Bodies[1]->velocity.y = 0;
	if (ptr->GetKey(olc::A).bReleased)
		Bodies[1]->velocity.x = 0;
	if (ptr->GetKey(olc::D).bReleased)
		Bodies[1]->velocity.x = 0;
    
	if (ptr->GetKey(olc::LEFT).bHeld)  
		Bodies[saberindex]->angularvelocity = -1.0;
	if (ptr->GetKey(olc::RIGHT).bHeld) 
		Bodies[saberindex]->angularvelocity = 1.0;

	
	if (ptr->GetKey(olc::LEFT).bReleased)  Bodies[saberindex]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::RIGHT).bReleased) Bodies[saberindex]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::D).bPressed) debug = !debug;
	if (ptr->GetMouse(0).bPressed)
	{
		int x = ptr->GetMouseX();
		int y = ptr->GetMouseY();
		std::vector<Vec2f> vertices =
		{
			Vec2f(20,60),
			Vec2f(-40,20),
			Vec2f(-20,-60),
			Vec2f(20, -60),
			Vec2f(40,20)
		};
		Body* poly = new Body(PolygonShape(vertices),1, x, y, 2.0);
		//poly->restitution = 0.1f;
		poly->friction = 0.7f;
		
		Bodies.push_back(poly);
	}
	
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr) 
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}
	LineData.clear();
	BoxShape* boxShape = (BoxShape*)Bodies[saberindex]->shape;
	//Bodies[saberindex]->position = Vec2f(ptr->GetMouseX(), ptr->GetMouseY());
	//Graphics::DrawLineWithData(ptr,ptr->ScreenWidth() / 2, ptr->ScreenHeight() / 2 , Bodies[saberindex]->position.x, Bodies[saberindex]->position.y, 0xffffffff, LineData);
	Graphics::DrawLineWithData(ptr, ptr->ScreenWidth() / 2, ptr->ScreenHeight() / 2, ptr->GetMouseX(), ptr->GetMouseY(), 0xffffffff, LineData);
	
	//ptr->DrawRe

	for (int i = 0; i < Bodies.size(); i++)
	{
		Body* body = Bodies[i];
		
			Vec2f drag = Force::GenerateDragForce(*body, 0.002f);
			//body->AddForce(drag);

			Vec2f weight = Vec2f(0.0f, body->mass * 9.8f * PIXELS_PER_METER);
			body->AddForce(weight);

			float torque = 200;
			//body->AddTorque(torque);

			Vec2f wind = Vec2f(2.0f * PIXELS_PER_METER, 0.0f);
			//body->AddForce(wind);
		
	}

	
	
	MS.mousecontrol(ptr, Bodies[mousenumber],mousenumber);
	if (mousenumber > Bodies.size() - 1)
	{
		mousenumber = 0;
	}

	for (int i = 0; i < Bodies.size(); i++)
	{
		Body* body = Bodies[i];
		
		
		body->Update(deltatime, index);
		//MS.Update(ptr, body, LineData, bodynumber);
		
	
	}
	
	MS.Update(ptr, Bodies[bodynumber], LineData, bodynumber,Bodies);
	if (bodynumber > Bodies.size() - 1)
	{
		bodynumber = 0;
	}
	

	for (int i = 0; i <= Bodies.size() - 1; i++)
	{
		for (int j = i + 1; j < Bodies.size(); j++)
		{
			//if (i != saberindex && j != saberindex)
			{
				Body* a = Bodies[i];
				Body* b = Bodies[j];
				Contact contact;
				a->isColliding = false;
				b->isColliding = false;
				if (CollisionDetection::IsColliding(a, b, contact))
				{
					contact.ResolveCollision();
					if (debug)
					{
						ptr->DrawCircle(contact.start.x, contact.start.y, 3, olc::DARK_MAGENTA);
						ptr->DrawCircle(contact.end.x, contact.end.y, 3, olc::DARK_MAGENTA);
						ptr->DrawLine(contact.start.x, contact.start.y, contact.end.x, contact.end.y, olc::DARK_MAGENTA);
						a->isColliding = true;
						b->isColliding = true;
					}


				}
			}
		}
	}

	
}

void Application::Render(olc::PixelGameEngine* ptr)
{

	
	for (int j = 0; j < Bodies.size(); j++)
	{
		Body* body = Bodies[j];
		if (body->shape->GetType() == CIRCLE)
		{
			
			CircleShape* circleshape = (CircleShape*)body->shape;
			DraweWireFrameModel(ptr, circleshape->vertices, body->position.x, body->position.y,body->rotation, circleshape->radius, 0xff00ff00);
			//ptr->FillCircle(body->position.x, body->position.y, circleshape->radius, color);
		}
		if(body->shape->GetType() == BOX)
		{
			
			BoxShape* boxShape = (BoxShape*)body->shape;
			
			
				
			
			
			

			MS.Render(ptr,body);
			for (int i = 0; i < boxShape->worldvertices.size(); i++)
			{
			
				
			
					
					ptr->FillCircle({ int(boxShape->worldvertices[i].x),int(boxShape->worldvertices[i].y) }, 7, olc::CYAN);
			}
			
			
			Graphics::DrawPolygon(ptr, body->position.x, body->position.y, boxShape->worldvertices, 0xff00ff00);
			
			
		}
		if (body->shape->GetType() == POLYGON)
		{
			PolygonShape* polygonShape = (PolygonShape*)body->shape;
			Graphics::DrawPolygon(ptr, body->position.x, body->position.y, polygonShape->worldvertices, 0xff00ff00);
		}
	}
}

void Application::Destroy()
{
	for (auto body : Bodies)
	{
		delete body;
	}
}

void Application::DraweWireFrameModel(olc::PixelGameEngine* pge, const std::vector<Vec2f> vecmodelcoordinates, float x, float y, float r, float s, olc::Pixel p)
{
	std::vector<Vec2f> vecTransformedCoordinates;
	int verts = vecmodelcoordinates.size();
	vecTransformedCoordinates.resize(verts);

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecmodelcoordinates[i].x * cosf(r) - vecmodelcoordinates[i].y * sinf(r);
		vecTransformedCoordinates[i].y = vecmodelcoordinates[i].x * sinf(r) + vecmodelcoordinates[i].y * cosf(r);
	}

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecTransformedCoordinates[i].x * s;
		vecTransformedCoordinates[i].y = vecTransformedCoordinates[i].y * s;
 	}

	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].x = vecTransformedCoordinates[i].x + x;
		vecTransformedCoordinates[i].y = vecTransformedCoordinates[i].y + y;
	}

	for (int i = 0; i < verts + 1; i++)
	{
		int j = (i + 1);
		pge->DrawLine(vecTransformedCoordinates[i % verts].x, vecTransformedCoordinates[i % verts].y,
			vecTransformedCoordinates[j % verts].x, vecTransformedCoordinates[j % verts].y, p);
	}

}
