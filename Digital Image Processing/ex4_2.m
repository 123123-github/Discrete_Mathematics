%��skeleton.jpgͼ���ļ�����Matlab
skeleton=imread('images\skeleton.jpg');
figure(1);
subplot(2,4,1);
imshow(skeleton);
title('ԭͼ��');

%1��	�ô��Խ��ߵ�Laplacian���䴦��
h1=[-1 -1 -1;-1 8 -1;-1 -1 -1];
s1=imfilter(skeleton,h1);
subplot(2,4,2);
imshow(s1);
title('���Խ��ߵ�Laplacian');

%2��	��1��������ӵ�ԭʼͼ���� ������ǿ��,Ӧ��취����
s2=imadd(s1,skeleton);
subplot(2,4,3);
imshow(s2);
title('����-���Խ��ߵ�Laplacian-���ͼ��');

%3��	��ȡSobelͼ����imfilter�������5��5����ƽ�����Լ�������
h1=fspecial('sobel');
s3=imfilter(skeleton,h1);
subplot(2,4,4);
imshow(s3);
title('sobel����');

h2=fspecial('average',[5 5]);
s4=imfilter(s3,h2);
subplot(2,4,5);
imshow(s4);
title('sobel����-5*5����ƽ����');

%4��	��ȡ2)��3)���ͼ���������Լ���
s5=im2double(s2).*im2double(s4);
s5=im2uint8(s5);
subplot(2,4,6);
imshow(s5);
title('��ȡ2)��3)��˺�ͼ��');

%5��	��4��������ӵ�ԭʼͼ����
s=s5+skeleton;
subplot(2,4,7);
imshow(s);
title('���Ӻ�');

%6��	�����imadjust������5)�����power-law�Ҷȱ任
%���� ��=0.5 �ı任
T=im2double(s);
T=imadjust(T,[],[],0.5);
subplot(2,4,8);
imshow(T);
title('�Ҷȱ任�����');

%7����ʾ������
figure(2);
imshow(skeleton);
title('ԭͼ��');
figure(3);
imshow(s);
title('�����ӽ��');
figure(4);
imshow(T);
title('�Ҷȱ任�����');