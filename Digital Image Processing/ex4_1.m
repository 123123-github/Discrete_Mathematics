%��cameraman.tifͼ���ļ�����Matlab
man=imread('images\cameraman.tif');

%ʹ��imfilter�����ֱ���ò�ͬ�����Ӷ�����������
%�������뷽��:
%(1)��fspecial��������
%�۲��м�ֵ��֪��fspecial������ˮƽsobel����
h1=fspecial('sobel');
s1=imfilter(man,h1);
%ת�ÿɵô�ֱsobel����
h2=h1';
s2=imfilter(man,h2);
%laplacian����
h3=fspecial('laplacian',0);
s3=imfilter(man,h3);
%��ʾ����ǰ���ͼ��
figure(1);
imshow(s1);
title('ˮƽsobel����');
figure(2);
imshow(s2);
title('��ֱsobel����');
figure(3);
imshow(s3);
title('laplacian����');
figure(4);
imshow(man);
title('����ǰͼ��');

%(2)ֱ������
%sobel���� ˮƽ
m1=[1 2 1;0 0 0;-1 -2 -1];
s4=imfilter(man,m1);
%sobel���� ��ֱ
m2=[1 0 -1;2 0 -2;1 0 -1];
s5=imfilter(man,m2);
%laplacian����
m3=[0 -1 0;-1 4 -1;0 -1 0];
s6=imfilter(man,m3);
%��ʾ����ǰ���ͼ��
figure(5);
imshow(s4);
title('ˮƽsobel����\_ֱ������');
figure(6);
imshow(s5);
title('��ֱsobel����\_ֱ������');
figure(7);
imshow(s6);
title('laplacian����\_ֱ������');