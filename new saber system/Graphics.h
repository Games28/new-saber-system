#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "olcPixelGameEngine.h"
#include "Vec2.h"
#include <vector>

struct Graphics
{
    static const int width = 1200;
    static const int height = 1300;
    static void DrawLine(olc::PixelGameEngine* pge,int x0, int y0, int x1, int y1, uint32_t color);
    static void DrawLineWithData(olc::PixelGameEngine* pge, int x0, int y0, int x1, int y1, uint32_t color, std::vector<LineData>& linedata);
    static void DrawCircle(olc::PixelGameEngine* pge, int x, int y, int radius, float angle, uint32_t color);
    static void DrawFillCircle(olc::PixelGameEngine* pge, int x, int y, int radius, uint32_t color);
    static void DrawRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color);
    static void DrawFillRect(olc::PixelGameEngine* pge, int x, int y, int width, int height, uint32_t color);
    static void DrawPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2f>& vertices, uint32_t color);
    static void DrawPolygon(olc::PixelGameEngine* pge, int x, int y, std::array<Vec2f, 4>& vertices, uint32_t color);
    static void DrawFillPolygon(olc::PixelGameEngine* pge, int x, int y, const std::vector<Vec2f>& vertices, uint32_t color);
    static void GetQuadBoundingBoxD(std::array<Vec2d, 4> vertices, Vec2i& upleft, Vec2i& lwright);
    static void GetQuadBoundingBoxF(std::array<Vec2f, 4> vertices, Vec2i& upleft, Vec2i& lwright);
    static bool wrapsample(Vec2f q, Vec2f b1, Vec2f b2, Vec2f b3, olc::Sprite* sprite, olc::Pixel& color);
    
    static void Drawwrapsprite(olc::PixelGameEngine* pge, std::array<Vec2f, 4>& vertices, olc::Sprite* sprite);
   
    static Vec2f GetQuadCenterPoint(std::array<Vec2f, 4>& vertices);
    
};


#endif // !GRAPHICS_H



