#include <stdio.h>
#include <stdlib.h>
#include <hpdf.h>

int main()
{
    // Create PDF document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf)
    {
        fprintf(stderr, "Error: Cannot create PDF document\n");
        return EXIT_FAILURE;
    }

    // Add a new page
    HPDF_Page page = HPDF_AddPage(pdf);
    if (!page)
    {
        fprintf(stderr, "Error: Cannot add page\n");
        HPDF_Free(pdf);
        return EXIT_FAILURE;
    }

    // Set font
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Add text to the page
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, 750, "Hello, this is some text for my PDF.");
    HPDF_Page_EndText(page);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, 737, "Hello, this is some text for my PDF.");
    HPDF_Page_EndText(page);
    HPDF_Page_BeginText(page);
    HPDF_Page_TextOut(page, 50, 724, "Hi, this is some text for my PDF.");
    HPDF_Page_EndText(page);

    // Load and embed an image
    HPDF_Image image = HPDF_LoadJpegImageFromFile(pdf, "/home/muneeb/Projects/KHIWeather/tempfiles/Precipitation.jpg");
    if (image)
    {
        HPDF_REAL imageWidth = HPDF_Image_GetWidth(image);
        HPDF_REAL imageHeight = HPDF_Image_GetHeight(image);
        HPDF_REAL desiredWidth = 400.0;
        HPDF_REAL scaleFactor = desiredWidth / imageWidth;
        HPDF_Page_DrawImage(page, image, 50, 200, imageWidth * scaleFactor, imageHeight * scaleFactor);
    }
    else
    {
        fprintf(stderr, "Error: Cannot load image\n");
    }

    // Save the PDF to a file
    HPDF_SaveToFile(pdf, "output.pdf");

    // Clean up
    HPDF_Free(pdf);

    return EXIT_SUCCESS;
}
