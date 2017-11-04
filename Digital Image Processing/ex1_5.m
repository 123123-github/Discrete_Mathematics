% 对 circuit.tif 图像实施反变换
circuit=imread('circuit.tif');

r=im2double(circuit);
s=1-r;
s=im2uint8(s);
%保存处理后的图像
imwrite(s,'images_ex\ex1_5.jpg');

%显示图像和灰度变换曲线
figure(1);
imshow(s);

x=0:0.01:1;
y=1-x;
figure(2);
plot(x,y);
xlabel('r');
ylabel('s');
title('Negative Transformation');