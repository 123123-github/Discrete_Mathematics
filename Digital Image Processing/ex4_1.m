%将cameraman.tif图像文件读入Matlab
man=imread('images\cameraman.tif');

%使用imfilter函数分别采用不同的算子对其作锐化运算
%算子输入方法:
%(1)用fspecial函数产生
%观察中间值得知，fspecial产生了水平sobel算子
h1=fspecial('sobel');
s1=imfilter(man,h1);
%转置可得垂直sobel算子
h2=h1';
s2=imfilter(man,h2);
%laplacian算子
h3=fspecial('laplacian',0);
s3=imfilter(man,h3);
%显示运算前后的图像
figure(1);
imshow(s1);
title('水平sobel算子');
figure(2);
imshow(s2);
title('垂直sobel算子');
figure(3);
imshow(s3);
title('laplacian算子');
figure(4);
imshow(man);
title('运算前图像');

%(2)直接输入
%sobel算子 水平
m1=[1 2 1;0 0 0;-1 -2 -1];
s4=imfilter(man,m1);
%sobel算子 垂直
m2=[1 0 -1;2 0 -2;1 0 -1];
s5=imfilter(man,m2);
%laplacian算子
m3=[0 -1 0;-1 4 -1;0 -1 0];
s6=imfilter(man,m3);
%显示运算前后的图像
figure(5);
imshow(s4);
title('水平sobel算子\_直接输入');
figure(6);
imshow(s5);
title('垂直sobel算子\_直接输入');
figure(7);
imshow(s6);
title('laplacian算子\_直接输入');