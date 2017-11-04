%��ȡʵ����ͼ��rice.png.
rice=imread('rice.png');

%�Ҷȱ任����T1
r=im2double(rice);
s=[256,256];
for i = 1 : 256
    for j = 1 : 256
        if r(i,j) < 0.35
            s(i,j)=0.3*r(i,j);
        elseif 0.35 <= r(i,j) && r(i,j) <= 0.65
            s(i,j)=0.105 + 2.6333*(r(i,j) - 0.35);
        else
            s(i,j)=1 + 0.3*(r(i,j)-1);
        end
    end
end
s=im2uint8(s);

%���洦������ͼ��
imwrite(s,'images_ex\ex1_T1.jpg');

%��ʾͼ��
figure(1);
imshow(s);
title('������ͼ��-T1')
%��ʾ�Ҷȱ任��������
figure(2);
x=0:0.01:1;
y = (0.3*x).*(x<0.35)+(0.105 + 2.6333*(x-0.35)).*(0.35<=x & x<=0.65)+((1+0.3*(x-1))).*(x>0.65);
plot(x,y);
xlabel('r');
ylabel('s');
title('�Ҷȱ任��������-T1');