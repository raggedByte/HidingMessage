## HidingMessage - a program for hiding text in a picture.

To embed text in an image use -embed command:
> hiding.exe -embed image.bmp secret.txt 


Note: processed image will saved instead original!
The program only works with 24-bit bmp images.

To retrieve secret text use -retrieve command:
> hiding.exe -retrieve imageWithSecret.bmp retrievedSecret.txt

### Example
Original image:

 ![original image](/test/origImage.bmp)  
 
 
Image with embedded secret file (secret.txt):

 ![processed image](/test/imageWithSecret.bmp)
