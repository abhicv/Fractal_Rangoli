#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//types used
#include "types.h"
#include "render.c"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

f32 r_min = -1;
f32 r_max = 1;
u32  max_iteration = 10;

//creating a PPM file to output the created design
void CreatePPMImageFile(u32 *pixels, u32 width, u32 height)
{
	FILE *imageFile = fopen("design.ppm","w");
	fprintf(imageFile,"P6 %d %d 255 ",width,height);
    
	for(u32 i = 0; i < width*height; i++)
	{
        u8 r = (pixels[i] & 0x000000FF);
        u8 g = (pixels[i] & 0x0000FF00) >> 8;
        u8 b = (pixels[i] & 0x00FF0000) >> 16;
        fprintf(imageFile,"%c%c%c", r, g, b);
	}
    
	fclose(imageFile);
}

//mapping function
f32 map(f32 x,f32  in_min, f32 in_max, f32 out_min, f32 out_max)
{
    return (x-in_min)*(out_max-out_min)/(in_max-in_min) + out_min;
}

//creating Mandlebrot fractal rendering to buffer
void RenderMandleBrotAngled(Buffer *buffer, Rect rect, f32 rMin, f32 rMax, Color color, f32 angle, f32 power)
{
    f32 aspect = (f32)rect.width / (f32)rect.height;
    
    Buffer tBuffer;
    tBuffer.data = (u32*)malloc(sizeof(u32) * rect.width * rect.height);
    tBuffer.width = rect.width;
    tBuffer.height = rect.height;
    
    //computing elements in Mandlebrot Set
    for(u32 y = 0; y < rect.height; y++) 
    {
        for(u32 x = 0; x < rect.width; x++) 
        {
			f32 a = map((f32)x, (f32)0, (f32)rect.width, rMin * aspect, rMax * aspect);
            f32 b = map((f32)y, (f32)0, (f32)rect.height, rMin, rMax);
            
            f32 ca = a;
            f32 cb = b;
            
            u32 n = 0;
            while(n < max_iteration) 
            {
                f32 r = sqrt(a*a + b*b);
                f32 arg = atan(b / a);
                f32 aa = pow(r, power) * cos(arg * power);
                f32 bb = pow(r, power) * sin(arg * power);
                
                a = aa + ca;
                b = bb + cb;
                
                if(abs(a+b) > 16)
                {
                    break;
                }
                
                n++;
            }
            
            if(n == max_iteration) 
            {
                tBuffer.data[x + y * tBuffer.width] = PACK_RGBA_INTO_U32(color);
            }
            else
            {
                Color c = {0, 0, 0, 0};
                tBuffer.data[x + y * tBuffer.width] = PACK_RGBA_INTO_U32(c);
            }
		}
	}
    
    angle = angle * 3.14f / 180.0f;
    
    //rotating the fractal
    for(i32 y = 0; y < rect.height; y++) 
    {
        for(i32 x = 0; x < rect.width; x++) 
        {
            i32 newX = x - rect.width / 2;
            i32 newY = y - rect.height / 2;
            
            i32 rotX = (newX * cos(angle)) - (newY * sin(angle));
            i32 rotY = (newX * sin(angle)) + (newY * cos(angle));
            
            rotX = rotX + rect.x;
            rotY = rotY + rect.y;
            
            u32 a = (tBuffer.data[x + y * rect.width] & 0xFF000000) >> 24;
            if(a)
            {
                buffer->data[rotX + rotY * buffer->width] = tBuffer.data[x + y * rect.width];
            }
        }
    }
    
    free(tBuffer.data);
}

int main(int argc, char **argv)
{
    Buffer renderBuffer = {0};
    renderBuffer.data = (u32*)malloc(sizeof(u32) * SCREEN_WIDTH * SCREEN_HEIGHT);
    renderBuffer.width = SCREEN_WIDTH;
    renderBuffer.height = SCREEN_HEIGHT;
    
    u32 count = 0;
    f32 r = 300;
    
    Rect rect = {0};
    rect.width = 300 * 0.9;
    rect.height = 200 * 0.9;
    
    //Rendering code for drawing various fractions
    DrawCircle(&renderBuffer, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 400, (Color){255, 255, 0, 255});
    for(int n = 0; n <= 360; n += 15)
    {
        rect.x = SCREEN_WIDTH / 2 - r * cos(n * 3.14 / 180);
        rect.y = SCREEN_HEIGHT / 2 - r * sin(n * 3.14 / 180);
        DrawCircleWire(&renderBuffer, rect.x, rect.y, 100, (Color){128, 0, 128, 255}, 5);
    }
    
    DrawCircle(&renderBuffer, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 350, (Color){255, 0, 0, 255});
    DrawCircle(&renderBuffer, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 100, (Color){255, 255, 255, 255});
    
    r = 330;
    for(int n = 0; n <= 360; n += 3)
    {
        rect.x = SCREEN_WIDTH / 2 - r * cos(n * 3.14 / 180);
        rect.y = SCREEN_HEIGHT / 2 - r * sin(n * 3.14 / 180);
        DrawCircleWire(&renderBuffer, rect.x, rect.y, 15, (Color){255, 255, 255, 255}, 2);
    }
    
    r = 120;
    for(int n = 0; n <= 360; n += 15)
    {
        rect.x = SCREEN_WIDTH / 2 - r * cos(n * 3.14 / 180);
        rect.y = SCREEN_HEIGHT / 2 - r * sin(n * 3.14 / 180);
        DrawCircleWire(&renderBuffer, rect.x, rect.y, 18, (Color){255, 0, 255, 255}, 5);
        DrawCircleWire(&renderBuffer, rect.x, rect.y, 10, (Color){255, 255, 0, 255}, 5);
    }
    
    r = 220;
    for(int n = 0; n <= 360; n += 45)
    {
        rect.x = SCREEN_WIDTH / 2 - r * cos(n * 3.14 / 180);
        rect.y = SCREEN_HEIGHT / 2 - r * sin(n * 3.14 / 180);
        
        if(count % 2)
        {
            RenderMandleBrotAngled(&renderBuffer, rect, -1.1, 1.1, (Color){255, 255, 0, 255}, n, 6.0);
        }
        else
        {
            RenderMandleBrotAngled(&renderBuffer, rect, -1.1, 1.1, (Color){0, 255, 255, 255}, n, 6.0);
        }
        
        DrawCircle(&renderBuffer, rect.x, rect.y, 40, (Color){255, 255, 255, 255});
        DrawCircle(&renderBuffer, rect.x, rect.y, 30, (Color){0, 0, 255, 255});
        
        count++;
    }
    
    rect.x = SCREEN_WIDTH / 2;
    rect.y = SCREEN_HEIGHT / 2;
    rect.width = 150 * 1.4;
    rect.height = 100 * 1.4;
    RenderMandleBrotAngled(&renderBuffer, rect, -1.2, 1.2, (Color){128, 0, 128, 255}, 0.0, 4.0);
    
    //writing to PPM image file
    CreatePPMImageFile(renderBuffer.data, renderBuffer.width, renderBuffer.height);
    
    free(renderBuffer.data);
    return 0;
}
