%�����Լ���д��ֱ��ͼ���⻯����

%��ȡͼƬ
space=imread('space.jpg');
%ֱ��ͼ���⻯
s1=my_histeq(space,256);
s2=histeq(space,256);

%��ʾԭͼƬ��������ͼƬ
figure(1);
imshow(space);
title('ԭͼƬ')
figure(2);
imshow(s1);
title('my\_histeq�����');
figure(3);
imshow(s2);
title('histeq�����');