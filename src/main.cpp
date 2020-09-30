#include "panoManipulation.h" 

void doMagicOneTime(cv::Mat img, float yaw, float pitch, float roll, string outputPath)
{
    pixel input, output;
    cv::Mat mapx;
    cv::Mat mapy;
    cv::Mat out = img.clone();
    mapx.create(cv::Size(img.cols, img.rows), CV_32FC1);
    mapy.create(cv::Size(img.cols, img.rows), CV_32FC1);

    for (int i = 0 ; i < out.rows; i++)
    {
        for (int j = 0 ; j < out.cols; j++)
        {
            input.x = j;
            input.y = i;

            output = computeDewarping(yaw, pitch, roll, input);

            mapx.at<float>(i,j) = output.x;
            mapy.at<float>(i,j) = output.y;
        }
    }

    cv::remap(img, out, mapx, mapy, cv::INTER_CUBIC, BORDER_WRAP);

    // draw Stuff
    for (int j = 0 ; j < out.cols; j++)
    {
        out.at<Vec3b>(1499,j)[0] = 0;   //B
        out.at<Vec3b>(1499,j)[1] = 0;   //G
        out.at<Vec3b>(1499,j)[2] = 255; //R
           
        out.at<Vec3b>(1500,j)[0] = 0;   //B
        out.at<Vec3b>(1500,j)[1] = 0;   //G
        out.at<Vec3b>(1500,j)[2] = 255; //R

        out.at<Vec3b>(1501,j)[0] = 0;   //B
        out.at<Vec3b>(1501,j)[1] = 0;   //G
        out.at<Vec3b>(1501,j)[2] = 255; //R
    }
    for (int j = 0 ; j < out.rows; j++)
    {
        out.at<Vec3b>(j,2999)[0] = 0;   //B
        out.at<Vec3b>(j,2999)[1] = 0;   //G
        out.at<Vec3b>(j,2999)[2] = 255; //R
           
        out.at<Vec3b>(j,3000)[0] = 0;   //B
        out.at<Vec3b>(j,3000)[1] = 0;   //G
        out.at<Vec3b>(j,3000)[2] = 255; //R

        out.at<Vec3b>(j,3001)[0] = 0;   //B
        out.at<Vec3b>(j,3001)[1] = 0;   //G
        out.at<Vec3b>(j,3001)[2] = 255; //R
    }

    cv::imwrite(outputPath, out);
}

void doMagic(cv::Mat img, int count)
{
    float angle = 0;
    float angle_limit = 6.6;
    pixel input, output;
    cv::Mat mapx;
    cv::Mat mapy;
    cv::Mat tmp = img.clone();
    cv::Mat out = img.clone();
    mapx.create(cv::Size(img.cols, img.rows), CV_32FC1);
    mapy.create(cv::Size(img.cols, img.rows), CV_32FC1);

    while (angle <= angle_limit)
    {
        std::cout << "Angle: " << angle << std::endl;

        for (int i = 0 ; i < out.rows; i++)
        {
            for (int j = 0 ; j < out.cols; j++)
            {
                input.x = j;
                input.y = i;

                output = computeDewarping(0, angle, 0, input);

                mapx.at<float>(i,j) = output.x;
                mapy.at<float>(i,j) = output.y;
            }
        }

        cv::remap(tmp, out, mapx, mapy, cv::INTER_CUBIC, BORDER_WRAP);
        
        // draw stuff
        for (int j = 0 ; j < out.cols; j++)
        {
            out.at<Vec3b>(1499,j)[0] = 0;   //B
            out.at<Vec3b>(1499,j)[1] = 0;   //G
            out.at<Vec3b>(1499,j)[2] = 255; //R
               
            out.at<Vec3b>(1500,j)[0] = 0;   //B
            out.at<Vec3b>(1500,j)[1] = 0;   //G
            out.at<Vec3b>(1500,j)[2] = 255; //R

            out.at<Vec3b>(1501,j)[0] = 0;   //B
            out.at<Vec3b>(1501,j)[1] = 0;   //G
            out.at<Vec3b>(1501,j)[2] = 255; //R
        }
        for (int j = 0 ; j < out.rows; j++)
        {
            out.at<Vec3b>(j,2999)[0] = 0;   //B
            out.at<Vec3b>(j,2999)[1] = 0;   //G
            out.at<Vec3b>(j,2999)[2] = 255; //R
               
            out.at<Vec3b>(j,3000)[0] = 0;   //B
            out.at<Vec3b>(j,3000)[1] = 0;   //G
            out.at<Vec3b>(j,3000)[2] = 255; //R

            out.at<Vec3b>(j,3001)[0] = 0;   //B
            out.at<Vec3b>(j,3001)[1] = 0;   //G
            out.at<Vec3b>(j,3001)[2] = 255; //R
        }

        // 0 padding ta mère
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(3) << count;
        cv::imwrite("/tmp/res/" + ss.str() + ".jpg", out);

        angle = angle + 2;
        count = count + 1;
    };  
}

int main(int argc, char* argv[])
{
    clock_t start, end;
        start = clock();
        cv::Mat img = cv::imread("/tmp/pano.jpg");
        doMagic(img, 1);
    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout << "Time taken by program is: " << ConvertSectoDay(time_taken);

    return 0;
}