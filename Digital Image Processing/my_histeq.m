function [s]=my_histeq(I,Scale)
% I 为输入的原始图像
% Scale 为输入的灰度等级
% s 为输出的直方图均衡化后的图像

%M,N记录图像矩阵的行列信息
[M,N]=size(I);
%hist记录I的灰度分布情况
hist=imhist(I)';
%n为I像素点总个数 m为I的灰度等级
n=M*N;
m=length(hist);
phist=hist/n;

if m==1
    return;
end

for i=2:m
    phist(1,i)=phist(1,i)+phist(1,i-1);
end

% y 为均衡化的灰度变换函数 Y为y的取整
y=phist*Scale;
Y=round(y);

s=I;
for i=0:255
    s(I==i) = Y(1,i+1);
end

end







