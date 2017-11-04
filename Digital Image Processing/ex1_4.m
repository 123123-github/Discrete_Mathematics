%��ȡʵ����ͼ��kids.tiff
kids=imread('kids.tif');

%�Ҷȱ任 S1 S2 S3
s1=imadjust(kids,[],[],0.6);
s2=imadjust(kids,[],[],0.4);
s3=imadjust(kids,[],[],0.3);

%����ͼ��
imwrite(s1,'images_ex\ex1_4_S1.jpg');
imwrite(s2,'images_ex\ex1_4_S2.jpg');
imwrite(s3,'images_ex\ex1_4_S3.jpg');

%��ʾͼ��
figure(1);
imshow(s1);
title('s=r^0.6 �任S1');
figure(2);
imshow(s2);
title('s=r^0.4 �任S2');
figure(3);
imshow(s3);
title('s=r^0.3 �任S3');

%��ʾ�Ҷȱ任��������
x=0:0.01:1;

m1=0.6;
y=x.^m1;
figure(4);
plot(x,y);
xlabel('r');
ylabel('s');
title('�Ҷȱ任��������-S1');

m2=0.4;
y=x.^m2;
figure(5);
plot(x,y);
xlabel('r');
ylabel('s');
title('�Ҷȱ任��������-S2');

m3=0.3;
y=x.^m3;
figure(6);
plot(x,y);
xlabel('r');
ylabel('s');
title('�Ҷȱ任��������-S3');