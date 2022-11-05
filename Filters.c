// 4 different filters that can be applied to a photo, grayscale filter, mirror filter, blur filter using box blur algorithim, and edges filter using Sobel operator

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
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

// Reflect image horizontally
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

// Blur image
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
                        // Add to sum of rgb colour values and increment # of valid pixels by 1 in order to calculate a correct average later
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

// Detect edges
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
