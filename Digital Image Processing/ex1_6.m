% �� rice.png ͼ��ʵʩ�Ҷ���Ƭ
rice=imread('rice.png');
r=im2double(rice);
s=[256,256];
for i = 1 : 256
    for j = 1 : 256
        if 0.2<=r(i,j) && r(i,j)<=0.4
            s(i,j)=0.6;
        else
            s(i,j)=r(i,j);
        end
    end
end
s=im2uint8(s);
imwrite(s,'images_ex\ex1_6.jpg');

%��ʾͼ��ͻҶȱ任��������
figure(1);
imshow(s);

figure(2);
x=0:0.01:1;
y=x.*(x<0.2 | x>0.4)+0.6*(0.2<=x & x<=0.4);
plot(x,y);
xlabel('r');
ylabel('s');
title('Gray-level slicing');