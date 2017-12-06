%将space.jpg图像文件读入Matlab，对其作直方图均衡化
space=imread('space.jpg');
s=histeq(space,256);

%+显示运算前后该图像的直方图
figure(1);
imhist(space);
title('运算前-图像直方图');

figure(2);
imhist(s);
title('运算后-图像直方图');

%处理后的图像
figure(3);
imshow(space);
title('原图像');

figure(4);
imshow(s);
title('处理后的图像');

%灰度变换函数






