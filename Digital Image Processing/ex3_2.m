%��test3_2.jpgͼ���ļ�����Matlab
test3_2=imread('images\test3_2.jpg');
t=im2double(test3_2);
%��fspecial��������һ 5*5 ����ƽ��������
h=fspecial('average',[5 5]);

%����filter2����������ƽ��
t=filter2(h,t);
t=im2uint8(t);

%   �Ƚ�����nlfilter����������ƽ�����ٶ�
%   ʹ�� nlfilter���� ����

%��ʾ�����Ľ��
figure(1);
imshow(test3_2);
title('����ǰͼ��');
figure(2);
imshow(t);
title('�����ͼ��');