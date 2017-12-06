%对space.jpg图像做直方图匹配运算
space=imread('space.jpg');

%指定直方图
x=0:1:255;
y=(1400*x).*(x<=5)...
+(7000-310*x).*(5<x & x<=20)...
+(900-5*x).*(20<x & x<=180)...
+(-1440+8*x).*(180<x & x<=225)...
+(3060-12*x).*(255<x & x<=255);

figure(1);
plot(x,y);
title('指定直方图');

process=y';
s=histeq(space,process);

%显示运算前后该图像的直方图
figure(2);
imhist(space);
title('运算前-直方图');
figure(3);
imhist(s);
title('运算后-直方图');

%处理后的图像
figure(4);
imshow(space);
title('原图像');
figure(5);
imshow(s);
title('处理后图像');

%灰度变换函数




