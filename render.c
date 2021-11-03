#include "render.h"

void ClearBuffer(Buffer *buffer, Color color)
{
    if(buffer->data != NULL)
    {
        for(u32 n = 0; n < (buffer->height * buffer->width); n++)
        {
            buffer->data[n] = PACK_RGBA_INTO_U32(color);
        }
    }
}

//TODO(abhicv): rect clipping
void DrawRect(Buffer *buffer, Rect *rect, Color color)
{
    if(buffer->data != NULL)
    {
        for(u32 y = rect->y; y < (rect->y + rect->height); y++)
        {
            for(u32 x = rect->x; x < (rect->x + rect->width); x++)
            {
                if(x >= 0 && y >= 0 && x < buffer->width && y < buffer->height)
                {
                    buffer->data[x + y * buffer->width] = PACK_RGBA_INTO_U32(color);
                }
                
            }
        }
    }
}

void DrawRectWire(Buffer *buffer, Rect *rect, Color color)
{
    if(buffer->data != NULL)
    {
        for(u32 y = rect->y; y < (rect->y + rect->height); y++)
        {
            for(u32 x = rect->x; x < (rect->x + rect->width); x++)
            {
                if(x >= 0 && y >= 0 && x < buffer->width && y < buffer->height)
                {
                    if(x == rect->x || y == rect->y || x == rect->x + rect->width - 1 || y == rect->y + rect->height - 1)
                    {
                        buffer->data[x + y * buffer->width] = PACK_RGBA_INTO_U32(color);
                    }
                }
            }
        }
    }
}

void DrawCircle(Buffer *buffer, i32 xPos, i32 yPos, u32 radius, Color color)
{
    if(buffer->data != NULL)
    {
        for(u32 y = yPos - radius; y < yPos + radius; y++)
        {
            for(u32 x = xPos - radius; x < xPos + radius; x++)
            {
                u32 distSqrt = (x - xPos) * (x - xPos) + (y - yPos) * (y - yPos);
                
                if(x >= 0 && y >= 0 && x < buffer->width && y < buffer->height)
                {
                    if(distSqrt <= (radius * radius))
                    {
                        buffer->data[x + y * buffer->width] = PACK_RGBA_INTO_U32(color);
                    }
                }
            }
        }
    }
}

void DrawCircleWire(Buffer *buffer, i32 xPos, i32 yPos, u32 radius, Color color, f32 thickness)
{
    if(buffer->data != NULL)
    {
        for(u32 y = yPos - radius; y < yPos + radius; y++)
        {
            for(u32 x = xPos - radius; x < xPos + radius; x++)
            {
                u32 distSqrt = (x - xPos) * (x - xPos) + (y - yPos) * (y - yPos);
                
                if(x >= 0 && y >= 0 && x < buffer->width && y < buffer->height)
                {
                    if(distSqrt <= (radius * radius) && 
                       distSqrt > ((radius - thickness) * (radius - thickness)))
                    {
                        buffer->data[x + y * buffer->width] = PACK_RGBA_INTO_U32(color);
                    }
                }
            }
        }
    }
}

void DrawRectRounded(Buffer *buffer, Rect *rect, u32 roundness, Color color)
{
    Rect vRect = {0};
    vRect.x = rect->x + roundness;
    vRect.y = rect->y;
    vRect.width = rect->width - (2 * roundness);
    vRect.height = rect->height;
    
    Rect hRect = {0};
    hRect.x = rect->x;
    hRect.y = rect->y + roundness;
    hRect.width = rect->width;
    hRect.height = rect->height - (2 * roundness);
    
    DrawRect(buffer, &hRect, color);
    DrawRect(buffer, &vRect, color);
    
    DrawCircle(buffer, rect->x + roundness, rect->y + roundness, roundness, color);
    DrawCircle(buffer, rect->x + rect->width - roundness, rect->y + roundness, roundness, color);
    DrawCircle(buffer, rect->x + roundness, rect->y + rect->height - roundness, roundness, color);
    DrawCircle(buffer, rect->x + rect->width - roundness, rect->y + rect->height - roundness, roundness, color);
}
