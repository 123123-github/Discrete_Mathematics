%获取实验用图像：rice.png.
rice=imread('rice.png');

%灰度变换函数T2
r=im2double(rice);
s=[256,256];
for i = 1 : 256
    for j = 1 : 256
        if r(i,j) <= 0.5
            s(i,j)=15.9744*r(i,j)^5;
        else
            s(i,j)=power((r(i,j)-0.5),0.2)+ 0.12;
        end
    end
end
s=im2uint8(s);

%保存处理后的新图像
imwrite(s,'images_ex\ex1_T2.jpg');

%显示图像
figure(1);
imshow(s);
title('处理后的图像-T2')
%显示灰度变换函数曲线
figure(2);
x=0:0.01:1;
y = (15.9744*power(x,5)).*(x<=0.5)+(power((x-0.5),0.2)+0.12).*(x>0.5);
plot(x,y);
xlabel('r');
ylabel('s');
title('灰度变换函数曲线-T2');