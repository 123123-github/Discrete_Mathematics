%获取实验用图像：rice.png.
rice=imread('rice.png');

%灰度变换函数T1
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

%保存处理后的新图像
imwrite(s,'images_ex\ex1_T1.jpg');

%显示图像
figure(1);
imshow(s);
title('处理后的图像-T1')
%显示灰度变换函数曲线
figure(2);
x=0:0.01:1;
y = (0.3*x).*(x<0.35)+(0.105 + 2.6333*(x-0.35)).*(0.35<=x & x<=0.65)+((1+0.3*(x-1))).*(x>0.65);
plot(x,y);
xlabel('r');
ylabel('s');
title('灰度变换函数曲线-T1');