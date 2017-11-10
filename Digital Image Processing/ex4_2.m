%将skeleton.jpg图像文件读入Matlab
skeleton=imread('images\skeleton.jpg');
figure(1);
subplot(2,4,1);
imshow(skeleton);
title('原图像');

%1）	用带对角线的Laplacian对其处理
h1=[-1 -1 -1;-1 8 -1;-1 -1 -1];
s1=imfilter(skeleton,h1);
subplot(2,4,2);
imshow(s1);
title('带对角线的Laplacian');

%2）	将1）结果叠加到原始图像上 噪声增强了,应想办法降低
s2=imadd(s1,skeleton);
subplot(2,4,3);
imshow(s2);
title('叠加-带对角线的Laplacian-后的图像');

%3）	获取Sobel图像并用imfilter对其进行5×5邻域平均，以减少噪声
h1=fspecial('sobel');
s3=imfilter(skeleton,h1);
subplot(2,4,4);
imshow(s3);
title('sobel算子');

h2=fspecial('average',[5 5]);
s4=imfilter(s3,h2);
subplot(2,4,5);
imshow(s4);
title('sobel算子-5*5邻域平均后');

%4）	获取2)和3)相乘图像，噪声得以减少
s5=im2double(s2).*im2double(s4);
s5=im2uint8(s5);
subplot(2,4,6);
imshow(s5);
title('获取2)和3)相乘后图像');

%5）	将4）结果叠加到原始图像上
s=s5+skeleton;
subplot(2,4,7);
imshow(s);
title('叠加后');

%6）	最后用imadjust函数对5)结果做power-law灰度变换
%采用 γ=0.5 的变换
T=im2double(s);
T=imadjust(T,[],[],0.5);
subplot(2,4,8);
imshow(T);
title('灰度变换处理后');

%7）显示处理结果
figure(2);
imshow(skeleton);
title('原图像');
figure(3);
imshow(s);
title('最后叠加结果');
figure(4);
imshow(T);
title('灰度变换处理后');