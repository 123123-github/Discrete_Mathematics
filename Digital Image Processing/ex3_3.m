%��ȡͼ��
test=imread('test3_2.jpg');

%��medfilt2������test3_2.jpgͼ�������ֵ�˲�
t=medfilt2(test,[5 5]);

%��ȷ��ʾ������
figure(1);
imshow(test);
title('ԭͼ��');
figure(2);
imshow(t);
title('5*5��ֵ�˲����ͼ��');