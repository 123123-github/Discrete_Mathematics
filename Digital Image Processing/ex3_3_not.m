%读取图像
test=imread('test3_1.jpg');

%在test3_1.jpg图像中添加均值为0，方差为0.02的高斯噪声
t1=imnoise(test,'gaussian',0,0.02);

%对噪声污染后的图像用nlfilter进行 5*5 邻域平均
t2=nlfilter(t1,[5 5],@mean2);
t2=uint8(t2);

%显示处理后的结果
figure(1);
imshow(test);
title('读入的图像 test3_1');
figure(2);
imshow(t1);
title('添加高斯噪声后的图像');
figure(3);
imshow(t2);
title('邻域平均后的图像');