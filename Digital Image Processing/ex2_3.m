%测试自己编写的直方图均衡化函数

%读取图片
space=imread('space.jpg');
%直方图均衡化
s1=my_histeq(space,256);
s2=histeq(space,256);

%显示原图片及处理后的图片
figure(1);
imshow(space);
title('原图片')
figure(2);
imshow(s1);
title('my\_histeq处理后');
figure(3);
imshow(s2);
title('histeq处理后');