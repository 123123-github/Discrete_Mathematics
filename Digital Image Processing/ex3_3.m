%读取图像
test=imread('test3_2.jpg');

%用medfilt2函数对test3_2.jpg图像进行中值滤波
t=medfilt2(test,[5 5]);

%正确显示处理结果
figure(1);
imshow(test);
title('原图像');
figure(2);
imshow(t);
title('5*5中值滤波后的图像');