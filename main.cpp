#include <iostream>
#include "Image.h"
using namespace std;

int main()
{
    ///Copying an image
    Image test("castle.jpg");
    test.write("new_castle.jpg");
    Image copy = test;
    for(int i = 0; i < copy.w * copy.ch; i++){
        copy.data[i] = 255;
    }
    copy.write("copy_castle.jpg");
    Image blank(100, 100, 3);
    blank.write("blank.jpg");

    ///Greyscale
    Image test2("test1.jpg");

    Image grey_avg = test2;
    grey_avg.greyscale_avg();
    grey_avg.write("grey_avg.jpg");

    ///Imporoved greyscale
    Image test3("test2.jpg");

    Image grey_lum = test3;
    grey_lum.greyscale_lum();
    grey_lum.write("grey_lum.jpg");

    ///Color channel modification
    Image test4("test4.jpg");
    test4.colorMask(100, 1, 1);///Here you can put any color combination (100, 1, 1)
    test4.write("filter1.jpg");
    test4.colorMask(142, 104, 240);
    test4.write("filter2.jpg");
    test4.colorMask(78, 64, 33);
    test4.write("filter3.jpg");
    test4.colorMask(160, 198, 59);
    test4.write("filter4.jpg");

    ///Steganography - encrypting images/hiding messages inside images :)
    Image test5 ("test5.jpg");
    test5.cryptImg("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum");
    test5.write("SecretMessage.jpg");
    char buffer[5000] = {0};
    size_t len = 0;
    test5.decryptImg(buffer, &len);
    printf("Message: %s(%zu)\n", buffer, len);

    return 0;
}
