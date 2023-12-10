#include "ManipulatedSprite.h"

void ManipulatedSprite::ScaleSprite(olc::Sprite* orgSprite, olc::Sprite** activeSpr, int nScale)
{
	if (*activeSpr != nullptr)
		delete* activeSpr;

	*activeSpr = new olc::Sprite(orgSprite->width * nScale, orgSprite->height * nScale);

    for (int y = 0; y < orgSprite->height; y++) {
        for (int x = 0; x < orgSprite->width; x++) {
            olc::Pixel p = orgSprite->GetPixel(x, y);
            for (int i = 0; i < nScale; i++) {
                for (int j = 0; j < nScale; j++) {
                    (*activeSpr)->SetPixel(x * nScale + j, y * nScale + i, p);
                }
            }
        }
    }
}

void ManipulatedSprite::Setup(Body* body)
{
    bodies.push_back(body);

	sprOrg = new olc::Sprite("crate.png");
	nScaleSprite = 1;
	scaleFactor = { 1.0f,1.0f };

	

	ScaleSprite(sprOrg, &activeSpr, nScaleSprite);

}

void ManipulatedSprite::Render(olc::PixelGameEngine* pge, Body* body)
{
	BoxShape* boxShape = (BoxShape*)body->shape;
	std::array<Vec2f, 4> points;
	
	
	points[0] = boxShape->worldvertices[3];
	points[1] = boxShape->worldvertices[2];
	points[2] = boxShape->worldvertices[1];
	points[3] = boxShape->worldvertices[0];
	
	olc::Sprite* pWarpedSprite = new olc::Sprite(body->sprite->width, body->sprite->height);
	//pge->SetDrawTarget(pWarpedSprite);

	//pge->Clear(olc::BLANK);

	DrawWarpedSprite(pge, points,body->sprite);

	//pge->SetDrawTarget(nullptr);

}

void ManipulatedSprite::mousecontrol(olc::PixelGameEngine* pge, Body* body,int& count)
{
	static Vec2f originalcoords;
	static Vec2f* offsetVertices;
	static Vec2f diffVertices;
	static float rotAngle;
		
		if (body->shape->GetType() == BOX)
		{

			BoxShape* boxShape = (BoxShape*)body->shape;





			Vec2f mouse = { (float)pge->GetMouseX(), (float)pge->GetMouseY() };

			if (pge->GetMouse(1).bPressed)
			{
				pSelected = nullptr;
				for (int i = 0; i < boxShape->worldvertices.size();i++)
				{
					if ((boxShape->worldvertices[i] - mouse).Magnitude() < 7)
					{
						pSelected = &boxShape->worldvertices[i];
						offsetVertices = &boxShape->offsetverts[i];
						originalcoords = mouse;

						//index = boxShape->bodyindex;
						boxShape->verticindex = i;
						break;
					}


				}
			}

			if (pge->GetMouse(1).bReleased)
			{
				
				pSelected = nullptr;
				
			}

			if (pSelected != nullptr)
			{
				Vec2f p = { pSelected->x, pSelected->y };
				*pSelected = mouse;
				diffVertices.x = mouse.x - originalcoords.x;
				diffVertices.y = mouse.y - originalcoords.y;
				offsetVertices->x = diffVertices.x;
				offsetVertices->y = diffVertices.y;

				
				
				
				
				pge->DrawString(50, 10, "index: " + std::to_string(index));
				
			}
			else
			{
				count++;
			}
			
			

			
		}
	
}

void ManipulatedSprite::GetQuadBoundingBoxD(std::array<Vec2d, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight)
{
	UpLeft = { INT_MAX, INT_MAX };
	LwRight = { INT_MIN,INT_MIN };
	
	for (int i = 0; i < vertices.size(); i++)
	{
		UpLeft = UpLeft.min(vertices[i]);
		LwRight = LwRight.max(vertices[i]);
	}

}

void ManipulatedSprite::GetQuadBoundingBoxF(std::array<Vec2f, 4>& vertices, Vec2i& UpLeft, Vec2i& LwRight)
{
	UpLeft = { INT_MAX, INT_MAX };
	LwRight = { INT_MIN,INT_MIN };

	for (int i = 0; i < vertices.size(); i++)
	{
		UpLeft = UpLeft.min(vertices[i]);
		LwRight = LwRight.max(vertices[i]);
	}
}

void ManipulatedSprite::DrawWarpedSprite(olc::PixelGameEngine* pge, std::array<Vec2f, 4> points, olc::Sprite* sprite)
{
	auto Get_q = [=](const  std::array<Vec2d, 4>& cPts, const Vec2d& curVert) -> Vec2d { return curVert - cPts[0];                     };
	auto Get_b1 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[1] - cPts[0];                     };
	auto Get_b2 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[2] - cPts[0];                     };
	auto Get_b3 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d 
		{ 
			return cPts[0] - cPts[1] - cPts[2] + cPts[3]; 
		};

	// note that the corner points are passed in order: ul, ll, lr, ur, but the WarpedSample() algorithm
	// assumes the order ll, lr, ul, ur. This rearrangement is done here
	std::array<Vec2d, 4> localCornerPoints;
	
	

	localCornerPoints[0] = points[1];
	localCornerPoints[1] = points[2];
	localCornerPoints[2] = points[0];
	localCornerPoints[3] = points[3];
	
	// get b1-b3 values from the quad corner points
	// NOTE: the q value is associated per pixel and is obtained in the nested loop below
	Vec2d b1 = Get_b1(localCornerPoints);
	Vec2d b2 = Get_b2(localCornerPoints);
	Vec2d b3 = Get_b3(localCornerPoints);

	// determine the bounding box around the quad
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBoxD(localCornerPoints, UpperLeft, LowerRight);

	// iterate all pixels within the bounding box of the quad...
	for (int y = UpperLeft.y; y <= LowerRight.y; y++) {
		for (int x = UpperLeft.x; x <= LowerRight.x; x++) {
			// ... and render them if sampling produces valid pixel
			olc::Pixel pix2render;
			Vec2d q = Get_q(localCornerPoints, { (double)x, (double)y });

			if (WarpedSample(q, b1, b2, b3, sprite, pix2render)) {
				if (pix2render != olc::MAGENTA)
				{
					
					pge->Draw(x, y, pix2render);

				}
			}
		}
	}
}

bool ManipulatedSprite::WarpedSample(Vec2d q, Vec2d b1, Vec2d b2, Vec2d b3, olc::Sprite* psprite, olc::Pixel& color)
{
	auto wedge_2d = [=](Vec2d v, Vec2d w) {
		return v.x * w.y - v.y * w.x;
		};

	// Set up quadratic formula
	double A = wedge_2d(b2, b3);
	double B = wedge_2d(b3, q) - wedge_2d(b1, b2);
	double C = wedge_2d(b1, q);

	// Solve for v
	Vec2d uv = { 0.0, 0.0 };
	if (fabs(A) < NEAR_ZERO) {
		// Linear form
		if (fabs(B) < NEAR_ZERO) {
			return false;
		}
		uv.y = -C / B;
	}
	else {
		// Quadratic form: Take positive root for CCW winding with V-up
		double D = B * B - 4 * A * C;
		if (D <= 0.0) {         // if discriminant <= 0, then the point is not inside the quad
			return false;
		}
		uv.y = 0.5 * (-B + sqrt(D)) / A;
	}
	// Solve for u, using largest magnitude component
	Vec2d denom = b1 + b3 * uv.y;
	if (fabs(denom.x) > fabs(denom.y)) {
		if (fabs(denom.x) < NEAR_ZERO) {
			return false;
		}
		uv.x = (q.x - b2.x * uv.y) / denom.x;
	}
	else {
		if (fabs(denom.y) < NEAR_ZERO) {
			return false;
		}
		uv.x = (q.y - b2.y * uv.y) / denom.y;
	}
	// Note that vertical texel coord is mirrored because the algorithm assumes positive y to go up
	color = psprite->Sample(uv.x, 1.0 - uv.y);

	// return whether sampling produced a valid pixel
	return (uv.x >= 0.0 && uv.x <= 1.0 &&
		uv.y >= 0.0 && uv.y <= 1.0);
}

Vec2i ManipulatedSprite::GetQuadCenterpoint(std::array<Vec2f, 4> points)
{
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBoxF(points, UpperLeft, LowerRight);
	// then return the points where the diagonals intersect
	return UpperLeft + (LowerRight - UpperLeft) / 2.0;
}

void ManipulatedSprite::DrawRotatedWarpedSprite(olc::PixelGameEngine* pge, const std::array<Vec2f, 4>& cornerPoints, float fAngle, Vec2f centerPoint)
{
	std::array<Vec2d, 4> rotatedPoints;
	for (int i = 0; i < 4; i++) {
		rotatedPoints[i] = cornerPoints[i];
	}
	// convert float parameters to doubles
	double dAngle = double(fAngle);
	Vec2d dCenterPoint(centerPoint);

	// rotate them around center point
	RotateQuadPoints(rotatedPoints, dAngle, dCenterPoint);
	// convert back to correct type
	std::array<Vec2f, 4> localPoints;
	for (int i = 0; i < 4; i++) {
		localPoints[i] = rotatedPoints[i];
	}
	

}

void ManipulatedSprite::RotateQuadPoints(std::array<Vec2d, 4> points, float rotation, Vec2f centerpt)
{
	double dCosTheta = cos(rotation);
	double dSinTheta = sin(rotation);
	// iterate quad points
	for (int i = 0; i < 4; i++) {
		// first translate point so that center point aligns with origin
		points[i] -= centerpt;
		// rotate point - because x component is altered and used for calculating y component
		// a cache is applied
		Vec2d cachePoint = points[i];
		points[i].x = cachePoint.x * dCosTheta - cachePoint.y * dSinTheta;
		points[i].y = cachePoint.x * dSinTheta + cachePoint.y * dCosTheta;
		// translate back
		points[i] += centerpt;
	}
}

void ManipulatedSprite::Update(olc::PixelGameEngine* pge, Body* body, std::vector<LineData>& linedata, int& count, std::vector<Body*>& bodies)
{
	BoxShape* boxShape = (BoxShape*)body->shape;
	int i = linedata.size() - 1;
	Vec2i mousepos = { linedata[i].x, linedata[i].y };
	Vec2i mouseorgin = { linedata[0].x, linedata[0].y };
	olc::Pixel p = olc::BLUE;

	Vec2i ending = Vec2i(begining.x + boxShape->width, 0);
	for (int i = 0; i < boxShape->worldvertices.size(); i++)
	{
		float newpoint;

		if (boxShape->isinsidebox(i, mousepos, newpoint)) // < then 0
		{
			p = olc::WHITE;
			count++;
		}
		else
		{
			p = olc::GREEN;



		}

	}
	if (p == olc::GREEN)
	{
		begining = Vec2i(pge->GetMouseX(), pge->GetMouseY());
	}
	int sizeX = int((body->position.x - boxShape->width / 2) + boxShape->width);
	if (pge->GetMouseX() > sizeX)
	{
		p = olc::BLUE;
	}

	
	pge->DrawString(30, 10, "beginning.x: " + std::to_string(begining.x) + "beginning.y: " + std::to_string(begining.y));
	pge->DrawLine(pge->ScreenWidth() / 2, pge->ScreenHeight() / 2, mousepos.x + 10, mousepos.y, p);

	std::vector<Body*> tempbody;
	for (int i = 0; i < (int)bodies.size(); i++)
	{
		Body* body = bodies[i];
		if (pge->GetKey(olc::Key::SPACE).bPressed)
		{
			olc::Sprite* curSprite = body->sprite;
			olc::Sprite* newSprite1 = nullptr;
			olc::Sprite* newSprite2 = nullptr;

			Vec2i vOffset1, vOffset2;
			int upperleftY, upperrightY, lowerleftY, lowerrightY;
			SplitSprite(curSprite, body->position, mouseorgin, mousepos, &newSprite1, &newSprite2, vOffset1, vOffset2,upperleftY,upperrightY,lowerleftY,lowerrightY);

			Body* body1 = new Body(BoxShape(newSprite1->width, newSprite1->height), 0,
				body->position.x - vOffset1.x, body->position.y - vOffset1.y, 1.0);
			BoxShape* boxShape1 = (BoxShape*)body1->shape;
			//boxShape1->worldvertices[2] = Vec2()

			body1->sprite = newSprite1;
			Body* body2 = new Body(BoxShape(newSprite2->width, newSprite2->height), 0,
				body->position.x - vOffset2.x, body->position.y - vOffset2.y, 1.0);
			body2->sprite = newSprite2;
			BoxShape* boxShape2 = (BoxShape*)body2->shape;



			if (body1->sprite->width > 0 && body1->sprite->height > 0)
			{
				tempbody.push_back(body1);
			}
			else
			{
				delete body1->sprite;
			}

			if (body2->sprite->width > 0 && body2->sprite->height > 0)
			{
				tempbody.push_back(body2);
			}
			else
			{
				delete body2->sprite;
			}
			delete curSprite;
			
		}
		else
		{
			tempbody.push_back(bodies[i]);
		}
	
	}
	bodies.clear();
	
	for (auto& obj : tempbody)
	{
		sabermarks(obj->sprite);
	}

	bodies = tempbody;

	//for (int i = 0; i < (int)bodies.size(); i++)
	//{
	//	auto rand_range3 = [=]() {
	//		switch (rand() % 50) {   // don't move too often
	//		case 0: return -1;
	//		case 1: return +1;
	//		}
	//		return 0;
	//		};
	//	bodies[i]->position += Vec2f(float(rand_range3()), float(rand_range3()));
	//}
	
	
}

bool ManipulatedSprite::IsLeft(Vec2i line1, Vec2i line2, Vec2i point)
{
	return (line2.x - line1.x) * (point.y - line1.y) - (line2.y - line1.y) * (point.x - line1.x) < 0;
}

olc::Sprite* ManipulatedSprite::TrimToMinimal(olc::Sprite* inputSpritePtr, Vec2i& offset)
{
	olc::Sprite* resultPtr = nullptr;
	if (inputSpritePtr == nullptr)
	{
		std::cout << "error: cannot trim nullptr input" << std::endl;
	}
	else
	{
		int lowX = inputSpritePtr->width, HighX = -1, lowY = inputSpritePtr->height, HighY = -1;

		for (int y = 0; y < inputSpritePtr->height; y++)
		{
			for (int x = 0; x < inputSpritePtr->width; x++)
			{
				if (inputSpritePtr->GetPixel(x, y) != olc::BLANK)
				{
					lowX = std::min(x, lowX);
					lowY = std::min(y, lowY);
					HighX = std::max(x, HighX);
					HighY = std::max(y, HighY);


				}
			}
		}
		offset = { lowX,lowY };

		resultPtr = new olc::Sprite(HighX - lowX + 1, HighY - lowY + 1);
		for (int y = lowY; y <= HighY; y++)
		{
			for (int x = lowX; x <= HighX; x++)
			{
				olc::Pixel aux = inputSpritePtr->GetPixel(x, y);
				resultPtr->SetPixel(x - lowX, y - lowY, aux);
			}
		}
	}
	return resultPtr;
}

void ManipulatedSprite::SplitSprite(olc::Sprite* org, Vec2i spriteOrigin, Vec2i split1, Vec2i split2, olc::Sprite** newHalf1, olc::Sprite** newHalf2, Vec2i& OffsetHalf1, Vec2i& OffsetHalf2,
	int& upperleftY,
	int& uppoerrightY,
	int& lowerleftY,
	int& lowerrightY)
{
	Vec2i localSprite1 = split1 - spriteOrigin;
	Vec2i localSprite2 = split2 - spriteOrigin;

	*newHalf1 = new olc::Sprite(org->width, org->height);
	*newHalf2 = new olc::Sprite(org->width, org->height);

	for (int y = 0; y < org->height; y++)
	{
		
		for (int x = 0; x < org->width; x++)
		{
			Vec2i pixPos = { x,y };
			olc::Pixel curPix = org->GetPixel(x, y);
		
			if (IsLeft(localSprite1, localSprite2, pixPos))
			{

				(*newHalf1)->SetPixel(x, y, curPix);
				(*newHalf2)->SetPixel(x, y, olc::BLANK);

			}
			else
			{
				(*newHalf1)->SetPixel(x, y, olc::BLANK);
				(*newHalf2)->SetPixel(x, y, curPix);
			}
		}
	}

	olc::Sprite* pTmpPtr1 = TrimToMinimal(*newHalf1, OffsetHalf1);
	delete* newHalf1;
	*newHalf1 = pTmpPtr1;

	olc::Sprite* pTmpPtr2 = TrimToMinimal(*newHalf2, OffsetHalf2);
	delete* newHalf2;
	*newHalf2 = pTmpPtr2;
}

void ManipulatedSprite::sabermarks(olc::Sprite* sprite)
{
	olc::Pixel p1 = olc::DARK_RED;
	olc::Pixel p = olc::DARK_YELLOW;

	for (int y = 0; y < sprite->height; y++)
	{
		for (int x = 0; x < sprite->width; x++)
		{

			if (sprite->GetPixel(x, y) == olc::BLANK)
			{

				if (sprite->GetPixel(x - 1, y) != olc::BLANK && sprite->GetPixel(x - 1, y) != olc::MAGENTA)
				{
					int i = 0;
					sprite->SetPixel(x - 1, y, p);
					sprite->SetPixel(x - 2, y, p1);
				}
				if (sprite->GetPixel(x, y - 1) != olc::BLANK && sprite->GetPixel(x, y - 1) != olc::MAGENTA)
				{
					int  i = 0;
					sprite->SetPixel(x, y - 1, p);
					sprite->SetPixel(x, y - 2, p1);
				}
				if (sprite->GetPixel(x, y + 1) != olc::BLANK && sprite->GetPixel(x, y + 1) != olc::MAGENTA)
				{
					int  i = 0;
					sprite->SetPixel(x, y + 1, p);
					sprite->SetPixel(x, y + 2, p1);
				}
				if (sprite->GetPixel(x + 1, y) != olc::BLANK && sprite->GetPixel(x + 1, y) != olc::MAGENTA)
				{
					int  i = 0;
					sprite->SetPixel(x + 1, y, p);
					sprite->SetPixel(x + 2, y, p1);
				}
			}

		}
	}
}



