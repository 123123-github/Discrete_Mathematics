%��ȡͼ��
test=imread('test3_1.jpg');

%��test3_1.jpgͼ������Ӿ�ֵΪ0������Ϊ0.02�ĸ�˹����
t1=imnoise(test,'gaussian',0,0.02);

%��������Ⱦ���ͼ����nlfilter���� 5*5 ����ƽ��
t2=nlfilter(t1,[5 5],@mean2);
t2=uint8(t2);

%��ʾ�����Ľ��
figure(1);
imshow(test);
title('�����ͼ�� test3_1');
figure(2);
imshow(t1);
title('��Ӹ�˹�������ͼ��');
figure(3);
imshow(t2);
title('����ƽ�����ͼ��');