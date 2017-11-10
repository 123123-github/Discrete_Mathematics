function [ s ] = my_medfilt2( r )
%函数实现对图像 r 进行 5*5 中值滤波
% r 为输入图像 输入 r 为 uint8 类型
% s 为输出图像 输入 s 为 uint8 类型
[M,N]=size(r);
s=uint8(zeros(M,N));
m=uint8(zeros(5,5));
%遍历像素点
for i=3:M-2
    for j=3:N-2
        %对遍历到的像素点，将其 5*5 邻域的值赋给矩阵 m 
        for ii=1:5
            for jj=1:5
                m(ii,jj)=r(i-3+ii,j-3+jj);
                %利用 sort() 函数排序，将中间值赋给矩阵 s 的相应元素
                temp=sort(m(:));
                s(i,j)=temp(13,1);
            end
        end
        %对遍历到的像素点，将其 5*5 邻域的值赋给矩阵 m 以上
    end
end

end