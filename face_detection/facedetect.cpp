#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
FILE *fp, *fp1, *fp2,* fp3;
char MotorA[3][50];
char MotorB[3][50];
void motor_control(int controlTime, int motorNumber, int orient);
void motor_init();
using namespace std;
using namespace cv;

void motor_init(){
    int count=0;
    strcpy(MotorA[0], "/sys/class/gpio/gpio134/direction");
    strcpy(MotorA[1], "/sys/class/gpio/gpio135/direction");
    strcpy(MotorA[2], "/sys/class/gpio/gpio136/direction");
    strcpy(MotorB[0], "/sys/class/gpio/gpio137/direction");
    strcpy(MotorB[1], "/sys/class/gpio/gpio138/direction");
    strcpy(MotorB[2], "/sys/class/gpio/gpio139/direction");

    

    for(count=0;count<3;count++){
        /*MOTOR A*/
        if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL){
        printf("Cannot open export file.\n");
        exit(1);
        }
        fprintf( fp, "%d", (134+count) );
        fclose(fp);
        if ((fp = fopen(MotorA[count], "rb+")) == NULL){
            printf("Cannot open direction file.\n");
            exit(1);
        }
        /*MOTOR B*/
        if ((fp = fopen("/sys/class/gpio/export", "w")) == NULL){
        printf("Cannot open export file.\n");
        exit(1);
        }
        fprintf( fp, "%d", (137+count) );
        fclose(fp);
        if ((fp = fopen(MotorB[count], "rb+")) == NULL){
            printf("Cannot open direction file.\n");
            exit(1);
        }
    }

}

/*   controlTime = 0~1000
 *   motorNumber = 1 or 2   (A & B)
 *   orient = 1 or 2 (forward or backward)
 */
void motor_control(int controlTime, int motorNumber, int orient){
    while(controlTime > 0){
        if (motorNumber == 1 && orient == 1)
        {
            fp1 = fopen(MotorA[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[1], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorA[2], "rb+");
            fprintf(fp3, "low");
            printf("1 out\n");
            }
        else if (motorNumber == 1 && orient == 2)
        {
            fp1 = fopen(MotorA[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorA[2], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorA[1], "rb+");
            fprintf(fp3, "low");
            printf("1 in\n");
        }
        else if (motorNumber == 2 && orient == 1)
        {
            fp1 = fopen(MotorB[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[1], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorB[2], "rb+");
            fprintf(fp3, "low");
            printf("2 out\n");
        }
        else if (motorNumber == 2 && orient == 2)
        {
            fp1 = fopen(MotorB[0], "rb+");
            fprintf(fp1, "high");
            fp2 = fopen(MotorB[2], "rb+");
            fprintf(fp2, "high");
            fp3 = fopen(MotorB[1], "rb+");
            fprintf(fp3, "low");
            printf("2 in\n");
        }
        else{

        }
        sleep(1);
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        controlTime--;
    }
}

static void help()
{
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many ~rigid objects, it's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
               "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye.xml\" --scale=1.3 \n"
            "Hit any key to quit.\n"
            "Using OpenCV version " << CV_VERSION << "\n" << endl;
}

IplImage * detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale);

String cascadeName = "../../data/haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

/*Load image file used*/
IplImage *cloth_img = NULL;
IplImage *cloth_output_frame = NULL;
char cloth_img_R[150][150];
char cloth_img_G[150][150];
char cloth_img_B[150][150];

int main( int argc, const char** argv )
{
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const String nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
    String inputName;
        int frame_index;
    help();

    CascadeClassifier cascade, nestedCascade;
    double scale = 1;

        /*load cloth image*/
        cloth_img = cvLoadImage("cloth1.jpg",1);
                cv::imshow("cloth img",cloth_img);
        if(!cloth_img) {
                printf("No such image file\n");                
                return 0;
        }
    for( int i = 1; i < argc; i++ )
    {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
        {
            cascadeName.assign( argv[i] + cascadeOptLen );
            cout << "  from which we have cascadeName= " << cascadeName << endl;
        }
        else if( nestedCascadeOpt.compare( 0, nestedCascadeOptLen, argv[i], nestedCascadeOptLen ) == 0 )
        {
            if( argv[i][nestedCascadeOpt.length()] == '=' )
                nestedCascadeName.assign( argv[i] + nestedCascadeOpt.length() + 1 );
            if( !nestedCascade.load( nestedCascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        }
        else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
        {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        }
        else if( argv[i][0] == '-' )
        {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        }
        else
            inputName.assign( argv[i] );
    }

    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
            "   [--nested-cascade[=nested_cascade_path]]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" << endl ;
        return -1;
    }

    if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
    {
        capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
                cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);                      
                cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240);        
        int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
        if(!capture) cout << "Capture from CAM " <<  c << " didn't work" << endl;
    }
    else if( inputName.size() )
    {
        image = imread( inputName, 1 );
        if( image.empty() )
        {
            capture = cvCaptureFromAVI( inputName.c_str() );
            if(!capture) cout << "Capture from AVI didn't work" << endl;
        }
    }
    else
    {
        image = imread( "lena.jpg", 1 );
        if(image.empty()) cout << "Couldn't read lena.jpg" << endl;
    }

    cvNamedWindow( "result", 1 );

    if( capture )
    {
        cout << "In capture ..." << endl;

                for(frame_index=0 ;;frame_index++)
                {
                                IplImage* iplImg = cvQueryFrame( capture );
                                frame = iplImg;
                                printf("%d %d \n" , frame.cols,frame.rows);
                                if( frame.empty() )
                                                break;


                                /*in our project , we capture 100 frame , but only detect face 1 frame */
                                if(( (frame_index%1) == 0)&&(frame_index!=0)){ 
                                //if(1){ 
                                                if( iplImg->origin == IPL_ORIGIN_TL )
                                                                frame.copyTo( frameCopy );
                                                else
                                                                flip( frame, frameCopy, 0 );

                                                cloth_output_frame = detectAndDraw( frameCopy, cascade, nestedCascade, scale );
                                }
                                else{
                                                cv::imshow( "result", frame );
                                }
                                if( waitKey( 10 ) >= 0 )
                                                goto _cleanup_;
                }


        waitKey(0);

_cleanup_:
        cvReleaseCapture( &capture );
    }
    else
    {
        cout << "In image read" << endl;
        if( !image.empty() )
        {
            detectAndDraw( image, cascade, nestedCascade, scale );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
            list of the image filenames to be processed - one per line */
            FILE* f = fopen( inputName.c_str(), "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf), c;
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
                        cloth_output_frame = detectAndDraw( image, cascade, nestedCascade, scale );
                        c = waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                    else
                    {
                            cerr << "Aw snap, couldn't read image " << buf << endl;
                    }
                }
                fclose(f);
            }
        }
    }
    cvDestroyWindow("result");
    cvReleaseImage(&cloth_img);
    return 0;
}

IplImage * detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale)
{
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,255,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius,rec_width,rec_height,i,j,k;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        /*Draw circle around face*/
                 //circle( img, center, radius, color, 3, 8, 0 );             
        /*Rectangle*/
        CvPoint point1, point2;                
        point1.x = r->x - r->width*0.9;
        point2.x = r->x + r->width*1.8;
        point1.y = r->y + r->height*0.9;
        point2.y = r->y + r->height*4.1;
        //calculate rect. width&height
        rec_width = point2.x-point1.x;
        rec_height = point2.y-point1.y;
        /*Draw a rectangle around body*/
               // cv::rectangle(img, point1, point2, CV_RGB(255,0,0), 3, 8, 0); 
        printf("rec_height=%d , rec_width=%d\n" , rec_height, rec_width);
        printf("Point1 is (%d,%d)\n",point1.x,point1.y);
        printf("Point2 is (%d,%d)\n",point2.x,point2.y);




                /*Put on clothes function*/
                for(i = point1.y ; i < point1.y+150 ; i++){ /*150  times*/
                                for(j = (point1.x)*3  ; j < (150+point1.x)*3 ; j = j+3){       /*if(signed char "-1" >> white[255])*/
                                                //if(cloth_img->imageData[(i-point1.y)*452+j-point1.x*3]==255&&cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+1]==255&&cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+2]==255){/*do not copy from cloth_image*/}
                                                if( (cloth_img->imageData[(i-point1.y)*452+j-point1.x*3] + cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+1] + cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+2])/3 >200 ){/*do not copy from cloth_image*/}
                                                else{
                                                                img.data[i*( img.cols * 3)+j] = cloth_img->imageData[(i-point1.y)*452+j-point1.x*3];            
                                                                img.data[i*( img.cols * 3)+j+1] = cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+1];
                                                                img.data[i*( img.cols * 3)+j+2] = cloth_img->imageData[(i-point1.y)*452+j-point1.x*3+2];
                                                }
                                }
                }
#if 0
                for(i = 0 ; i < 150 ; i++){ /*150  times*/
                                for(j =0  ; j <450 ; j=j+3){   
                                                img.data[i*960+j] =(unsigned char) cloth_img->imageData[i*450+j];            
                                                img.data[i*960+j+1] = (unsigned char) cloth_img->imageData[i*450+j+1];
                                                img.data[i*960+j+2] = (unsigned char) cloth_img->imageData[i*450+j+2];

                                }
                }
#endif
#if 0
                for(i = 0 ; i < 150 ; i++){ /*150  times*/
                                for(j =0  ; j <450 ; j+=3){   
                                                cloth_img_B[i][j/3]        = cloth_img->imageData[i*452+j];
                                                cloth_img_G[i][j/3] =  cloth_img->imageData[i*452+j+1];
                                                cloth_img_R[i][j/3] =  cloth_img->imageData[i*452+j+2];
                                }
                }
                for(i = 0 ; i < 150 ; i++){ /*150  times*/
                                for(j =0  ; j <450 ; j=j+3){   
                                                img.data[i*960+j] = cloth_img_B[i][j/3];            
                                                img.data[i*960+j+1] = cloth_img_G[i][j/3];
                                                img.data[i*960+j+2] = cloth_img_R[i][j/3];

                                }
                }
#endif
                /*printf format by img*/
                printf("data = %d\n",cloth_img->imageData[(i-point1.y)*450+j-point1.x*3]);
                printf("imgCols = %d imgCows = %d \n",img.cols , img.rows);
                

                /*unable to useing the nested detection*/
                #if 0
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }                                            
                #endif
    }
    cv::imshow( "result", img );
        return cloth_img;

}