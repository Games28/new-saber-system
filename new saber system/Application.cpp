#include "Application.h"

void Application::Setup()
{
	world = new World(-9.8f);

	

	
	Body* bob = new Body(BoxShape(10, 10), Graphics::width / 2.0, Graphics::height / 2.0 - 200, 0.0);
	Body* head = new Body(BoxShape(50, 50), bob->position.x, bob->position.y + 70, 1.0);
	Body* torso = new Body(BoxShape(50, 100), head->position.x, head->position.y + 80, 2.0);
	Body* lowertorso = new Body(BoxShape(50, 30), torso->position.x, torso->position.y + 70, 2.0f);
	Body* leftArm = new Body(BoxShape(25, 70), torso->position.x - 40, torso->position.y - 10, 2.0);
	Body* rightArm = new Body(BoxShape(25, 70), torso->position.x + 40, torso->position.y - 10, 2.0);
	Body* leftLeg = new Body(BoxShape(30, 90), lowertorso->position.x - 20, lowertorso->position.y + 60, 2.0);
	Body* rightLeg = new Body(BoxShape(30, 90), lowertorso->position.x + 20, lowertorso->position.y + 60, 2.0);
	//bob->SetTexture("bob.png");
	
	head->SetTexture("mandohead.png");
	
	torso->SetTexture("mandotorse.png");
	lowertorso->SetTexture("mandolowertorso.png");
	leftArm->SetTexture("mandoleftArm.png");
	rightArm->SetTexture("mandorightArm.png");
	leftLeg->SetTexture("mandoleftLeg.png");
	rightLeg->SetTexture("mandorightLeg.png");
	world->AddBody(bob);
	world->AddBody(head);
	world->AddBody(torso);
	world->AddBody(lowertorso);
	world->AddBody(leftArm);
	world->AddBody(rightArm);
	world->AddBody(leftLeg);
	world->AddBody(rightLeg);
	
	// Add joints between ragdoll parts (distance constraints with one anchor point)
	JointConstraint* string = new JointConstraint(bob, head, bob->position);
	JointConstraint* neck = new JointConstraint(head, torso, head->position + Vec2f(0, 25));
	JointConstraint* leftShoulder = new JointConstraint(torso, leftArm, torso->position + Vec2f(-28, -45));
	JointConstraint* rightShoulder = new JointConstraint(torso, rightArm, torso->position + Vec2f(+28, -45));
	JointConstraint* waist = new JointConstraint(torso, lowertorso, lowertorso->position + Vec2f(0, -20));
	JointConstraint* leftHip = new JointConstraint(lowertorso, leftLeg, lowertorso->position + Vec2f(-10, 0));
	JointConstraint* rightHip = new JointConstraint(lowertorso, rightLeg, lowertorso->position + Vec2f(+10,0));
	world->AddConstraint(string);
	world->AddConstraint(neck);
	
	world->AddConstraint(leftShoulder);
	world->AddConstraint(rightShoulder);
	world->AddConstraint(waist);
	world->AddConstraint(leftHip);
	world->AddConstraint(rightHip);
	
	// Add a floor and walls to contain objects objects
	Body* floor = new Body(BoxShape(Graphics::width - 50, 50), Graphics::width / 2.0, Graphics::height - 50, 0.0);
	Body* leftWall = new Body(BoxShape(50, Graphics::height - 100), 50, Graphics::height / 2.0 - 25, 0.0);
	Body* rightWall = new Body(BoxShape(50, Graphics::height - 100), Graphics::width - 50, Graphics::height / 2.0 - 25, 0.0);
	floor->restitution = 0.7;
	leftWall->restitution = 0.2;
	rightWall->restitution = 0.2;
	floor->SetTexture("crate.png");
	leftWall->SetTexture("head.png");
	world->AddBody(floor);
	world->AddBody(leftWall);
	world->AddBody(rightWall);
	
}

void Application::Input(olc::PixelGameEngine* ptr)
{
	if (ptr->GetKey(olc::W).bHeld)
		world->GetBodies()[bodynumber]->velocity.y = -400;
	if (ptr->GetKey(olc::S).bHeld)
		world->GetBodies()[bodynumber]->velocity.y = +400;
	if (ptr->GetKey(olc::A).bHeld)
		world->GetBodies()[bodynumber]->velocity.x = -400;
	if (ptr->GetKey(olc::D).bHeld)
		world->GetBodies()[bodynumber]->velocity.x = +400;

	if (ptr->GetKey(olc::W).bReleased)
		world->GetBodies()[bodynumber]->velocity.y = 0;
	if (ptr->GetKey(olc::S).bReleased)
		world->GetBodies()[bodynumber]->velocity.y = 0;
	if (ptr->GetKey(olc::A).bReleased)
		world->GetBodies()[bodynumber]->velocity.x = 0;
	if (ptr->GetKey(olc::D).bReleased)
		world->GetBodies()[bodynumber]->velocity.x = 0;
    
	
	
	if (ptr->GetKey(olc::F1).bPressed) bodynumber = 0;
	if (ptr->GetKey(olc::F2).bPressed) bodynumber = 1;
	if (ptr->GetKey(olc::F3).bPressed) bodynumber = 2;
	
	if (ptr->GetKey(olc::LEFT).bReleased)  world->GetBodies()[bodynumber]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::RIGHT).bReleased) world->GetBodies()[bodynumber]->angularvelocity = 0;//pushForce.x = 0;
	if (ptr->GetKey(olc::D).bPressed) debug = !debug;
	if (ptr->GetMouse(0).bPressed)
	{
		int x = ptr->GetMouseX();
		int y = ptr->GetMouseY();
		
		Body* poly = new Body(BoxShape(50,50), x, y, 2.0);
		//poly->restitution = 0.1f;
		poly->friction = 0.7f;
		
		world->AddBody(poly);
	}

	//Vec2f mouse = Vec2f((int)ptr->GetMouseX(), (int)ptr->GetMouseY());
	//Body* bob = world->GetBodies()[0];
	//Vec2f direction = (mouse - bob->position).Normalize();
	//float speed = 5.0f;
	//bob->position += direction * speed;
	
}

void Application::Update(float deltatime,olc::PixelGameEngine* ptr) 
{
	if (deltatime > 0.017)
	{
		deltatime = 0.017f;
	}
	LineData.clear();
	//BoxShape* boxShape = (BoxShape*)Bodies[saberindex]->shape;
	//Bodies[saberindex]->position = Vec2f(ptr->GetMouseX(), ptr->GetMouseY());
	//Graphics::DrawLineWithData(ptr,ptr->ScreenWidth() / 2, ptr->ScreenHeight() / 2 , Bodies[saberindex]->position.x, Bodies[saberindex]->position.y, 0xffffffff, LineData);
	//Graphics::DrawLineWithData(ptr, ptr->ScreenWidth() / 2, ptr->ScreenHeight() / 2, ptr->GetMouseX(), ptr->GetMouseY(), 0xffffffff, LineData);
	//MS.mousecontrol(ptr, Bodies[mousenumber], mousenumber);
	
	world->Update(ptr, deltatime);
	
}

void Application::Render(olc::PixelGameEngine* ptr)
{
	for (auto joint : world->GetConstraints())
	{
		const Vec2f pa = joint->a->LocalSpaceToWorldSpace(joint->aPoint);
		const Vec2f pb = joint->b->LocalSpaceToWorldSpace(joint->bPoint);
		Graphics::DrawLine(ptr, pa.x, pa.y, pb.x, pb.y, 0xff555555);
	
	}
	
	for (auto& body : world->GetBodies())
	{
		
		if (body->shape->GetType() == CIRCLE)
		{
			
			CircleShape* circleshape = (CircleShape*)body->shape;
			DraweWireFrameModel(ptr, circleshape->vertices, body->position.x, body->position.y,body->rotation, circleshape->radius, 0xff00ff00);
			//ptr->FillCircle(body->position.x, body->position.y, circleshape->radius, color);
		}
		if(body->shape->GetType() == BOX)
		{
			
			BoxShape* boxShape = (BoxShape*)body->shape;
			
			
				
			
			
			
			if (body->sprite != nullptr)
			{
				MS.Render(ptr, body);
			}
			//ptr->DrawSprite(body->position.x, body->position.y, body->sprite);
			//for (int i = 0; i < boxShape->worldvertices.size(); i++)
			//{
			//
			//	
			//
			//		
			//		ptr->FillCircle({ int(boxShape->worldvertices[i].x),int(boxShape->worldvertices[i].y) }, 7, olc::CYAN);
			//}
			
			
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
