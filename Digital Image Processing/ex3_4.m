%编写中值滤波程序（函数）
%程序编写在文件 my_medfilt2.m 中

test=imread('images\test3_2.jpg');

%对test3_2.jpg图像进行中值滤波
%函数效率好像有问题，运行较慢 大概6~8s
t=my_medfilt2(test);

%显示处理结果
figure(1);
imshow(test);
title('原图像');
figure(2);
imshow(t);
title('自编函数处理后的图像');