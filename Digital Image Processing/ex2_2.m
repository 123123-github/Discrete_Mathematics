%��space.jpgͼ����ֱ��ͼƥ������
space=imread('space.jpg');

%ָ��ֱ��ͼ
x=0:1:255;
y=(1400*x).*(x<=5)...
+(7000-310*x).*(5<x & x<=20)...
+(900-5*x).*(20<x & x<=180)...
+(-1440+8*x).*(180<x & x<=225)...
+(3060-12*x).*(255<x & x<=255);

figure(1);
plot(x,y);
title('ָ��ֱ��ͼ');

process=y';
s=histeq(space,process);

%��ʾ����ǰ���ͼ���ֱ��ͼ
figure(2);
imhist(space);
title('����ǰ-ֱ��ͼ');
figure(3);
imhist(s);
title('�����-ֱ��ͼ');

%������ͼ��
figure(4);
imshow(space);
title('ԭͼ��');
figure(5);
imshow(s);
title('�����ͼ��');

%�Ҷȱ任����




