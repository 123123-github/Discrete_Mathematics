%��space.jpgͼ���ļ�����Matlab��������ֱ��ͼ���⻯
space=imread('space.jpg');
s=histeq(space,256);

%+��ʾ����ǰ���ͼ���ֱ��ͼ
figure(1);
imhist(space);
title('����ǰ-ͼ��ֱ��ͼ');

figure(2);
imhist(s);
title('�����-ͼ��ֱ��ͼ');

%������ͼ��
figure(3);
imshow(space);
title('ԭͼ��');

figure(4);
imshow(s);
title('������ͼ��');

%�Ҷȱ任����






