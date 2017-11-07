%将test3_1.jpg图像文件读入Matlab
test3_1=imread('images\test3_1.jpg');

%用nlfilter对其进行5*5邻域平均
t1=nlfilter(test3_1,[5 5],@fun_mat_average);
t1=uint8(t1);
t2=nlfilter(test3_1,[5 5],@mean2);
t2=uint8(t2);
%计算邻域标准差
t3=nlfilter(test3_1,[5 5],@std2);
t3=uint8(t3);

%显示处理后的结果
%显示原图像及新生成的图像_5*5邻域平均
figure(1);
imshow(test3_1);
title('原图像');
figure(2);
imshow(t1);
title('5*5邻域平均后图像\_自写的函数');
figure(3);
imshow(t2);
title('5*5邻域平均后图像\_mean2函数');
%显示邻域标准差图像
figure(4);
imshow(t3);
title('邻域标准差图像');

