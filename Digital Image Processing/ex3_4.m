%��д��ֵ�˲����򣨺�����
%�����д���ļ� my_medfilt2.m ��

test=imread('images\test3_2.jpg');

%��test3_2.jpgͼ�������ֵ�˲�
%����Ч�ʺ��������⣬���н��� ���6~8s
t=my_medfilt2(test);

%��ʾ������
figure(1);
imshow(test);
title('ԭͼ��');
figure(2);
imshow(t);
title('�Աຯ��������ͼ��');