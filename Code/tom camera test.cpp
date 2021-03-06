#include <stdio.h>
#include <time.h>

extern "C" int init(int d_level);
extern "C" int sleep(int sec, int usec);
extern "C" int set_motor(int motor, int value);
extern "C" int take_picture();
extern "C" int get_pixel(int row, int col, int colour);
extern "C" int open_screen_stream();
extern "C" int close_screen_stream();
extern "C" int update_screen();
using namespace std;

int main()
{
  init(0);
  bool flip = false;
        open_screen_stream();
        int pixels [320];
        while (true) {
            take_picture();
            update_screen();
            for (int i = 0;i<320;i++) {
                int whiteness = get_pixel(i, 120, 3);//gets a line of pixels halfway down the image
                if (whiteness>140) { //Changeable, 140 seems to be best number. Might have to be changed depending on lighting
                    whiteness = 1;
                }
                else {
                    whiteness = 0;
                }
                pixels[i] = whiteness;
            }
            int num = -160;
            for (int i=0; i<320;i++){        //this loop goes through the pixel array and multiplies each value by num, num starts at -160 and increases by 1 each loop       
                if(num==0){num++;}           //addded an "if" to skip 0 so that it goes ....-2,-1,1,2....
                pixels[i]=pixels[i]*num;
                num++;
            }
            int sum = 0;
            for (int i=0; i<320;i++){     //this loop simply adds up the array          
                sum = sum + pixels[i];
            }
            
            if (sum>0){flip=true;}//if sum = 0 then flip will not be changed and stay whatever it was right before the sum was 0
            if(sum<0){flip=false;}
            
            //The following code will make the robot go backward if it loses the white line havent quite tested but seems to work
            if(sum==0 && flip == false){
              set_motor(1, -40);
              set_motor(2, -50);
              printf("moving back");
            }
            else if(sum==0 && flip == true){
              set_motor(1, -50);
              set_motor(2, -40);
              printf("moving back");
            }
            /*if there are lots of white pixels on the left, sum should be very negative, lots on the right and sum should be very positive, 
              when relativly even, the sum should be very small. The sum is proportional to how uneven the white pixels are. Maximum sum is about 12200*/
              
            double turnRate = 0.004;//how sensitive robot is to corners
            UI.printf("\nThe sum is %d",sum);
            set_motor(1 , 45+turnRate*sum);//45 is the base speed which is the speed when the white line is perfectly centred
            set_motor(2 , 45-turnRate*sum);
        }
  }
  
     return 0;
}
