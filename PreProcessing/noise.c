#include "noise.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"

void array_swap(int array[], size_t i, size_t j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void array_select_sort(int array[], size_t len)
{
    for(int i = 0; (size_t)i < len; i++)
    {
        size_t min_index = 0;
        for(int j = i + 1; (size_t)j < len; j++)
        {
            if(array[j] < array[min_index])
            {
                min_index = j;
            }
        }
        array_swap(array, min_index, i);
    }
}

void noiseReduction(SDL_Surface *img)
{
  int w;
  int h;
  int base_value;
  w = img -> w;
  h = img -> h;
  int pixelTable[5];

 for(int i = 0; i < w; i++)
 {
   for(int j = 0; j < h; j++)
   {
     for(int k = j; k <= j + 4 && k < h; k++)
     {
        base_value = get_pixel(img, i, k);
        pixelTable[0] = base_value;
        if(i == 0)
        {
            if(k == 0)
            {
                pixelTable[1] = base_value;
                pixelTable[2] = base_value;
                pixelTable[3] = get_pixel(img, i, k + 1);
                pixelTable[4] = get_pixel(img, i + 1, k);
                break;
            }
         if(k == h - 1)
         {
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = base_value;
            pixelTable[3] = base_value;
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
         else
         {
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = base_value;
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
         }
       }
       if(i == w - 1)
       {
          if(k == 0)
          {
                pixelTable[1] = base_value;
                pixelTable[2] = get_pixel(img, i - 1, k);
                pixelTable[3] = get_pixel(img, i, k + 1);
                pixelTable[4] = base_value;
                break;
          }
          if(k == h - 1)
          {
                pixelTable[1] = get_pixel(img, i, k - 1);
                pixelTable[2] = get_pixel(img, i - 1, k);
                pixelTable[3] = base_value;
                pixelTable[4] = base_value;
                break;
          }
          else
          {
                pixelTable[1] = get_pixel(img, i, k - 1);
                pixelTable[2] = get_pixel(img, i - 1, k);
                pixelTable[3] = get_pixel(img, i, k + 1);
                pixelTable[4] = base_value;
                break;
          }
       }
       if(k == 0)
       {
              pixelTable[1] = base_value;
              pixelTable[2] = get_pixel(img, i - 1, k);
              pixelTable[3] = get_pixel(img, i, k + 1);
              pixelTable[4] = get_pixel(img, i + 1, k);
              break;
       }
       if(k == h - 1)
       {
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = base_value;
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
       }
       else
       {
            pixelTable[1] = get_pixel(img, i, k - 1);
            pixelTable[2] = get_pixel(img, i - 1, k);
            pixelTable[3] = get_pixel(img, i, k + 1);
            pixelTable[4] = get_pixel(img, i + 1, k);
            break;
       }
     }
      array_select_sort(pixelTable, 5);
      int med = pixelTable[2];
      put_pixel(img, i, j, med);
   }
 }
}
