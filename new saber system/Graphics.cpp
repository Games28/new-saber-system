#include "Graphics.h"
#define NEAR_ZERO   0.000001
void Graphics::DrawLine(olc::PixelGameEngine* pge, int x0, int y0, int x1, int y1, uint32_t color)
{
	int delta_x(x1 - x0);
	int delta_y(y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / float(longest_side_length);
	float y_inc = delta_y / float(longest_side_length);

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length; i++)
	{
		pge->Draw(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;

	}
}

void Graphics::DrawLineWithData(olc::PixelGameEngine* pge, int x0, int y0, int x1, int y1, uint32_t color, std::vector<LineData>& linedata)
{
	int delta_x(x1 - x0);
	int delta_y(y1 - y0);

	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / float(longest_side_length);
	float y_inc = delta_y / float(longest_side_length);

	float current_x = x0;
	float current_y = y0;



	for (int i = 0; i <= longest_side_length; i++)
	{
		LineData data = { (int)current_x,(int)current_y,olc::BLUE };
		linedata.push_back(data);
		//pge->Draw(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;

	}
}

void Graphics::DrawCircle(olc::PixelGameEngine* pge, int x, int y, int radius, float angle, uint32_t color)
{

}

void Graphics::DrawFillCircle(olc::PixelGameEngine* pge, int x, int y, int radius, uint32_t color)
{
}

void Graphics::DrawRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int current_x = x + i;
			int current_y = y + j;
			pge->Draw(current_x, current_y, color);
		}
	}
}

void Graphics::DrawFillRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color)
{
}

void Graphics::DrawPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2f>& vertices, uint32_t color)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		int currIndex = i;
		int nextIndex = (i + 1) % vertices.size();
		DrawLine(pge, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
	}

	//pge->DrawCircle(x, y, 4, color);
}

void Graphics::DrawPolygon(olc::PixelGameEngine* pge, int x, int y, std::array<Vec2f, 4>& vertices, uint32_t color)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		int currIndex = i;
		int nextIndex = (i + 1) % vertices.size();
		DrawLine(pge, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
	}

	//pge->DrawCircle(x, y, 4, color);
}

void Graphics::DrawFillPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2f>& vertices, uint32_t color)
{
}

void Graphics::GetQuadBoundingBoxD(std::array<Vec2d, 4> vertices, Vec2i& upleft, Vec2i& lwright)
{
	upleft = { INT_MAX,INT_MAX };
	lwright = { INT_MIN, INT_MIN };
	for (int i = 0; i < 4; i++)
	{
		upleft = upleft.min(vertices[i]);
		lwright = lwright.max(vertices[i]);
	}
}

void Graphics::GetQuadBoundingBoxF(std::array<Vec2f, 4> vertices, Vec2i& upleft, Vec2i& lwright)
{
	upleft = { INT_MAX,INT_MAX };
	lwright = { INT_MIN, INT_MIN };
	for (int i = 0; i < 4; i++)
	{
		upleft = upleft.min(vertices[i]);
		lwright = lwright.max(vertices[i]);
	}
}

bool Graphics::wrapsample(Vec2f q, Vec2f b1, Vec2f b2, Vec2f b3, olc::Sprite* sprite, olc::Pixel& color)
{
	auto wedge_2d = [=](Vec2d v, Vec2d w)
		{
			return v.x * w.y - v.y * w.x;
		};

	double A = wedge_2d(b2, b3);
	double B = wedge_2d(b3, q) - wedge_2d(b1, b2);
	double C = wedge_2d(b1, q);

	Vec2f uv = { 0.0f,0.0f };
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

	Vec2f denom = b1 + b3 * uv.y;
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
	color = sprite->Sample(uv.x, 1.0 - uv.y);

	return (uv.x >= 0.0 && uv.x < 1.0 &&
		uv.y >  0.0 && uv.y <= 1.0);

	
}

void Graphics::Drawwrapsprite(olc::PixelGameEngine* pge, std::array<Vec2f, 4>& vertices, olc::Sprite* sprite)
{
	// These lambdas return respectively the values q, b1 - b3 from the bilinear interpolation analysis
   // The b1 - b3 values can be determined once per quad. The q value is associated per pixel
	auto Get_q = [=](const  std::array<Vec2d, 4>& cPts, const Vec2d& curVert) -> Vec2d { return curVert - cPts[0];                     };
	auto Get_b1 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[1] - cPts[0];                     };
	auto Get_b2 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[2] - cPts[0];                     };
	auto Get_b3 = [=](const std::array<Vec2d, 4>& cPts) -> Vec2d { return cPts[0] - cPts[1] - cPts[2] + cPts[3]; };

	// note that the corner points are passed in order: ul, ll, lr, ur, but the WarpedSample() algorithm
	// assumes the order ll, lr, ul, ur. This rearrangement is done here
	std::array<Vec2d, 4> localCornerPoints;
	localCornerPoints[0] = vertices[1];
	localCornerPoints[1] = vertices[2];
	localCornerPoints[2] = vertices[0];
	localCornerPoints[3] = vertices[3];

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
			
			Vec2f q = Get_q(localCornerPoints, { double(x), double(y) });

			if (wrapsample(q, b1, b2, b3, sprite, pix2render)) {
				
				pge->Draw(x, y, pix2render);
			}
		}
	}
}



Vec2f Graphics::GetQuadCenterPoint(std::array<Vec2f, 4>& vertices)
{
	Vec2i UpperLeft, LowerRight;
	GetQuadBoundingBoxF(vertices, UpperLeft, LowerRight);
	// then return the points where the diagonals intersect
	return UpperLeft + (LowerRight - UpperLeft) / 2.0;
}


