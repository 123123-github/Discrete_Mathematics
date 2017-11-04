% �� circuit.tif ͼ��ʵʩ���任
circuit=imread('circuit.tif');

r=im2double(circuit);
s=1-r;
s=im2uint8(s);
%���洦����ͼ��
imwrite(s,'images_ex\ex1_5.jpg');

%��ʾͼ��ͻҶȱ任����
figure(1);
imshow(s);

x=0:0.01:1;
y=1-x;
figure(2);
plot(x,y);
xlabel('r');
ylabel('s');
title('Negative Transformation');