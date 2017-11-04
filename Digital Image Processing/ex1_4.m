%获取实验用图像：kids.tiff
kids=imread('kids.tif');

%灰度变换 S1 S2 S3
s1=imadjust(kids,[],[],0.6);
s2=imadjust(kids,[],[],0.4);
s3=imadjust(kids,[],[],0.3);

%保存图像
imwrite(s1,'images_ex\ex1_4_S1.jpg');
imwrite(s2,'images_ex\ex1_4_S2.jpg');
imwrite(s3,'images_ex\ex1_4_S3.jpg');

%显示图像
figure(1);
imshow(s1);
title('s=r^0.6 变换S1');
figure(2);
imshow(s2);
title('s=r^0.4 变换S2');
figure(3);
imshow(s3);
title('s=r^0.3 变换S3');

%显示灰度变换函数曲线
x=0:0.01:1;

m1=0.6;
y=x.^m1;
figure(4);
plot(x,y);
xlabel('r');
ylabel('s');
title('灰度变换函数曲线-S1');

m2=0.4;
y=x.^m2;
figure(5);
plot(x,y);
xlabel('r');
ylabel('s');
title('灰度变换函数曲线-S2');

m3=0.3;
y=x.^m3;
figure(6);
plot(x,y);
xlabel('r');
ylabel('s');
title('灰度变换函数曲线-S3');