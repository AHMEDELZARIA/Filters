### Bitmaps

Perhaps the simplest way to represent an image is with a grid of pixels (i.e., dots), each of which can be of a different colour. For black-and-white images, I thus need 1 bit per pixel, as 0 could represent black and 1 could represent white, as in the below.

![1](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/67693812-674c-44d2-bb4e-09d8b6689cb8)

In this sense, then, is an image just a bitmap (i.e., a map of bits). For more colourful images, you simply need more bits per pixel. A file format (like [BMP](https://en.wikipedia.org/wiki/BMP_file_format), [JPEG](https://en.wikipedia.org/wiki/JPEG), or [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics)) that supports “24-bit colour” uses 24 bits per pixel. (BMP actually supports 1-, 4-, 8-, 16-, 24-, and 32-bit colour.)

A 24-bit BMP uses 8 bits to signify the amount of red in a pixel’s color, 8 bits to signify the amount of green in a pixel’s color, and 8 bits to signify the amount of blue in a pixel’s color. If you’ve ever heard of RGB color, well, there you have it: red, green, blue.

If the R, G, and B values of some pixel in a BMP are, say, `0xff`, `0x00`, and `0x00` in hexadecimal, that pixel is purely red, as `0xff` (otherwise known as `255` in decimal) implies “a lot of red,” while `0x00` and `0x00` imply “no green” and “no blue,” respectively.

### A Bit(map) More Technical

Recalling that a file is just a sequence of bits, arranged in some fashion. A 24-bit BMP file, then, is essentially just a sequence of bits, (almost) every 24 of which happen to represent some pixel’s colour. But a BMP file also contains some “metadata,” information like an image’s height and width. That metadata is stored at the beginning of the file in the form of two data structures generally referred to as “headers,” not to be confused with C’s header files. (Incidentally, these headers have evolved over time. This project uses the latest version of Microsoft’s BMP format, 4.0, which debuted with Windows 95.)

The first of these headers, called `BITMAPFILEHEADER`, is 14 bytes long. The second of these headers, called `BITMAPINFOHEADER`, is 40 bytes long. Immediately following these headers is the actual bitmap: an array of bytes, triples of which represent a pixel’s colour. However, BMP stores these triples backwards (i.e., as BGR), with 8 bits for blue, followed by 8 bits for green, followed by 8 bits for red. (Some BMPs also store the entire bitmap backwards, with an image’s top row at the end of the BMP file. But for this project, it set’s BMPs as described herein, with each bitmap’s top row first and bottom row last.) In other words, were I to convert the 1-bit smiley above to a 24-bit smiley, substituting red for black, a 24-bit BMP would store this bitmap as follows, where `0000ff` signifies red and `ffffff` signifies white; which is highlighted in red all instances of `0000ff`.

![2](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/fe637da7-c879-4e57-91f9-1dd391013a88)

Because these bits are presented from left to right, top to bottom, in 8 columns, you can actually see the red smiley if you take a step back.

Notice that you could represent a bitmap as a 2-dimensional array of pixels: where the image is an array of rows, each row is an array of pixels. 

### Image Filtering

What does it even mean to filter an image? You can think of filtering an image as taking the pixels of some original image, and modifying each pixel in such a way that a particular effect is apparent in the resulting image.

*The original image before applying filters:*

![3](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/7a454dc1-bd8e-42fa-abc2-348dbe5c0bd5)

**Grayscale**

One common filter is the “grayscale” filter, where I take an image and want to convert it to black-and-white. How does that work?

Recall that if the red, green, and blue values are all set to `0x00` (hexadecimal for `0`), then the pixel is black. And if all values are set to `0xff` (hexadecimal for `255`), then the pixel is white. So long as the red, green, and blue values are all equal, the result will be varying shades of gray along the black-white spectrum, with higher values meaning lighter shades (closer to white) and lower values meaning darker shades (closer to black).

So to convert a pixel to grayscale, I just need to make sure the red, green, and blue values are all the same value. But how do I know what value to make them? Well, it’s probably reasonable to expect that if the original red, green, and blue values were all pretty high, then the new value should also be pretty high. And if the original values were all low, then the new value should also be low.

In fact, to ensure each pixel of the new image still has the same general brightness or darkness as the old image, I can take the average of the red, green, and blue values to determine what shade of grey to make the new pixel.

If you apply that to each pixel in the image, the result will be an image converted to grayscale.

*Code for grayscale filter:*
```C
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // Iterates through each row
    {
        for (int j = 0; j < width; j++) // Iterates through each column (each pixel)
        {
            double average = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00; // Calculates average rbg value of pixel
            int avg = round(average); // Converts average into an int for rgb compatability
            
            // Set average value equal to all colour value for greyscale effect
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}
```
*Image with applied grayscale filter:*

![4](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/943aa5b4-6cf4-4a5a-952b-e539110e580c)

**Reflection**

Some filters might also move pixels around. Reflecting an image, for example, is a filter where the resulting image is what you would get by placing the original image in front of a mirror. So any pixels on the left side of the image should end up on the right, and vice versa.

Note that all of the original pixels of the original image will still be present in the reflected image, it’s just that those pixels may have rearranged to be in a different place in the image.

*Code for reflect filter:*
```C
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) // Iterate through each row
    {
        for (int j = 0; j < width/2; j++) // Iterate through each column (each pixel)
        {
           RGBTRIPLE reflection = image[i][j]; // Set current pixel as a variable (for swapping later)
           image[i][j] = image[i][width - j - 1]; // Change current pixel into direct opposite pixel
           image[i][width - j - 1] = reflection; // Change direct opposite pixel into variable
        }
    }
    return;
}
```
*Image with applied reflection filter:*

![5](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/2da63991-1147-4995-b8e9-f178c6c4b445)

**Blur**

There are a number of ways to create the effect of blurring or softening an image. For this problem, I will use the “box blur,” which works by taking each pixel and, for each colour value, giving it a new value by averaging the colour values of neighbouring pixels.

Consider the following grid of pixels, where we’ve numbered each pixel.

![6](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/64279b67-55fa-410d-95b2-dc3a78d36810)

The new value of each pixel would be the average of the values of all of the pixels that are within 1 row and column of the original pixel (forming a 3x3 box). For example, each of the colour values for pixel 6 would be obtained by averaging the original colour values of pixels 1, 2, 3, 5, 6, 7, 9, 10, and 11 (note that pixel 6 itself is included in the average). Likewise, the colour values for pixel 11 would be be obtained by averaging the colour values of pixels 6, 7, 8, 10, 11, 12, 14, 15 and 16.

For a pixel along the edge or corner, like pixel 15, I would still look for all pixels within 1 row and column: in this case, pixels 10, 11, 12, 14, 15, and 16.

*Code for blur filter:*
```C
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width]; // 2D array that will store blurred pixels
    
    for (int i = 0; i < height; i++) // Iterate through rows of pixels
    {
        for (int j = 0; j < width; j++) // Iterate through columns of pixels (each pixel)
        {
            int numValidPixels = 0, sumRed = 0, sumGreen = 0, sumBlue = 0;
            
            for (int x = -1; x < 2; x++) // Iterate through rows of neighgbouring pixels
            {
                for (int y = -1; y < 2; y++) // Iterate through columns of neighbouring pixels (each neighbouring pixel)
                {
                    int currentX = i + x; // Current X position of neighbouring pixel
                    int currentY = j + y; // Current Y position of neighbouring pixel
                    
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1)) // If invalid pixels (outside of image)
                    {
                        continue; // Don't count them
                    }
                    else // If valid
                    {
                        /* Add to sum of rgb colour values and increment # of valid pixels 
                           by 1 in order to calculate a correct average later */
                        sumRed += image[currentX][currentY].rgbtRed;
                        sumGreen += image[currentX][currentY].rgbtGreen;
                        sumBlue += image[currentX][currentY].rgbtBlue;
                        numValidPixels++;
                    }
                }
            }
            // Calculate new RGB values for blurred pixel
            int avgRed = round( sumRed / (float) numValidPixels );
            int avgGreen = round( sumGreen / (float) numValidPixels );
            int avgBlue = round( sumBlue / (float) numValidPixels );
            
            // Transfer new blurred pixel to copy
            blurred[i][j].rgbtRed = avgRed;
            blurred[i][j].rgbtGreen = avgGreen;
            blurred[i][j].rgbtBlue = avgBlue;
        }
    }
    
    // Changing original image to blurred copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blurred[i][j].rgbtRed;
            image[i][j].rgbtGreen = blurred[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blurred[i][j].rgbtBlue;
        }
    }
    return;
}
```
*Image with applied blur filter:*

![7](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/5f066f28-77c2-42ba-85bb-64c4e3811d31)

**Edges**

In artificial intelligence algorithms for image processing, it is often useful to detect edges in an image: lines in the image that create a boundary between one object and another. One way to achieve this effect is by applying the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator) to the image.

Like image blurring, edge detection also works by taking each pixel, and modifying it based on the 3x3 grid of pixels that surrounds that pixel. But instead of just taking the average of the nine pixels, the Sobel operator computes the new value of each pixel by taking a weighted sum of the values for the surrounding pixels. And since edges between objects could take place in both a vertical and a horizontal direction, I will actually compute two weighted sums: one for detecting edges in the x direction, and one for detecting edges in the y direction. In particular, I will use the following two “kernels”:

![8](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/552e17dc-4774-43a7-a918-285976efd827)

How to interpret these kernels? In short, for each of the three colour values for each pixel, we’ll compute two values `Gx` and `Gy`. To compute `Gx` for the red channel value of a pixel, for instance, I will take the original red values for the nine pixels that form a 3x3 box around the pixel, multiply them each by the corresponding value in the `Gx` kernel, and take the sum of the resulting values.

Why these particular values for the kernel? In the `Gx` direction, for instance, I am multiplying the pixels to the right of the target pixel by a positive number, and multiplying the pixels to the left of the target pixel by a negative number. When I take the sum, if the pixels on the right are a similar colour to the pixels on the left, the result will be close to 0 (the numbers cancel out). But if the pixels on the right are very different from the pixels on the left, then the resulting value will be very positive or very negative, indicating a change in colour that likely is the result of a boundary between objects. And a similar argument holds true for calculating edges in the `y` direction.

Using these kernels, I can generate a `Gx` and `Gy` value for each of the red, green, and blue channels for a pixel. But each channel can only take on one value, not two: so I need some way to combine `Gx` and `Gy` into a single value. The Sobel filter algorithm combines `Gx` and `Gy` into a final value by calculating the square root of `Gx^2 + Gy^2`. And since channel values can only take on integer values from 0 to 255, I must be sure the resulting value is rounded to the nearest integer and capped at 255!

And what about handling pixels at the edge, or in the corner of the image? There are many ways to handle pixels at the edge, but for the purposes of this problem, I will treat the image as if there was 1 pixel solid black border around the edge of the image: therefore, trying to access a pixel past the edge of the image should be treated as a solid black pixel (values of 0 for each of red, green, and blue). This will effectively ignore those pixels from our calculations of `Gx`and `Gy`.

*Code for edges filter:*
```C
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edged[height][width]; // 2D array that will store new pixels
    
    int Gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} }; // Gx matrix
    int Gy[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} }; // Gy matrix
    
    for (int i = 0; i < height; i++) // Iterates through rows of pixels
    {
        for (int j = 0; j < width; j++) // Iterates through each column of pixels (each pixel)z
        {
            int sumGxRed = 0, sumGxGreen = 0, sumGxBlue = 0, sumGyRed = 0, sumGyGreen = 0, sumGyBlue = 0;
            int counter1 = 0; // Keeps track of current row in matrices
            
            for (int x = -1; x < 2; x++) // Iterate through each row of neighbouring pixels
            {
                int counter2 = 0; // Keep track of current column in matrices
                
                for (int y = -1; y < 2; y++) // Iterates through each column of neighboring pixels (each neighbouring pixel)
                {
                    int currentX = i + x; // Obtains x coordinate of current neighbouring pixel
                    int currentY = j + y; // Obtains y coordinate of current neighbouring pixel
                    
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1)) // If invalid pixel (outside of image), don't do anything, increment counter2 to update current column in matrices correctly
                    {
                        counter2++;
                        continue;
                    }
                    else // Calculate Gx and Gy values for all three colour values and store them in their respective variables
                    {
                        sumGxRed += (image[currentX][currentY].rgbtRed * Gx[counter1][counter2]);
                        sumGxGreen += (image[currentX][currentY].rgbtGreen * Gx[counter1][counter2]);
                        sumGxBlue += (image[currentX][currentY].rgbtBlue * Gx[counter1][counter2]);
                        sumGyRed += (image[currentX][currentY].rgbtRed * Gy[counter1][counter2]);
                        sumGyGreen += (image[currentX][currentY].rgbtGreen * Gy[counter1][counter2]);
                        sumGyBlue += (image[currentX][currentY].rgbtBlue * Gy[counter1][counter2]);
                    }
                    counter2++; // Update current column of matrices
                }
                counter1++; // Update current row of matrices
            }
            
            // Calculate new rgb values for pixel
            int newRed = round(sqrt((sumGxRed * sumGxRed) + (sumGyRed * sumGyRed)));
            int newGreen = round(sqrt((sumGxGreen * sumGxGreen) + (sumGyGreen * sumGyGreen)));
            int newBlue = round(sqrt((sumGxBlue * sumGxBlue) + (sumGyBlue * sumGyBlue)));
            
            // Check if new values are over the cap of 255, if they are, cap them at 255
            if (newRed > 255)
            {
                newRed = 255;
            }
            if (newBlue > 255)
            {
                newBlue = 255;
            }
            if (newGreen > 255)
            {
                newGreen = 255;
            }
            
            // Place new pixel into 2D array edged
            edged[i][j].rgbtRed = newRed;
            edged[i][j].rgbtGreen = newGreen;
            edged[i][j].rgbtBlue = newBlue;
        }   
    }
    
    // Change image into new edged image
    for (int i = 0; i < height; i++) // Iterate through rows of pixels
    {
        for (int j = 0; j < width; j++) // Iterate through columns of pixels (each pixel)
        {
            image[i][j].rgbtRed = edged[i][j].rgbtRed;
            image[i][j].rgbtGreen = edged[i][j].rgbtGreen;
            image[i][j].rgbtBlue = edged[i][j].rgbtBlue;
        }
    }
    
    return;
}
```
*Image with applied edges filter:*

![9](https://github.com/AHMEDELZARIA/Recover-JPEG/assets/93144563/04371480-c604-4dfc-8a54-de3487f40fc1)
