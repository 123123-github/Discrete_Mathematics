%将test3_2.jpg图像文件读入Matlab
test3_2=imread('images\test3_2.jpg');
t=im2double(test3_2);
%用fspecial函数生成一 5*5 邻域平均窗函数
h=fspecial('average',[5 5]);

%再用filter2函数求邻域平均
t=filter2(h,t);
t=im2uint8(t);

%   比较与用nlfilter函数求邻域平均的速度
%   使用 nlfilter函数 较慢

%显示处理后的结果
figure(1);
imshow(test3_2);
title('处理前图像');
figure(2);
imshow(t);
title('处理后图像');