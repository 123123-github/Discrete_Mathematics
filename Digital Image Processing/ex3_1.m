%��test3_1.jpgͼ���ļ�����Matlab
test3_1=imread('images\test3_1.jpg');

%��nlfilter�������5*5����ƽ��
t1=nlfilter(test3_1,[5 5],@fun_mat_average);
t1=uint8(t1);
t2=nlfilter(test3_1,[5 5],@mean2);
t2=uint8(t2);
%���������׼��
t3=nlfilter(test3_1,[5 5],@std2);
t3=uint8(t3);

%��ʾ�����Ľ��
%��ʾԭͼ�������ɵ�ͼ��_5*5����ƽ��
figure(1);
imshow(test3_1);
title('ԭͼ��');
figure(2);
imshow(t1);
title('5*5����ƽ����ͼ��\_��д�ĺ���');
figure(3);
imshow(t2);
title('5*5����ƽ����ͼ��\_mean2����');
%��ʾ�����׼��ͼ��
figure(4);
imshow(t3);
title('�����׼��ͼ��');

