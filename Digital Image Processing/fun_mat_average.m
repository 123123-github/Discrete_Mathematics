function [ c ] = fun_mat_average( m )
%该函数实现计算 M*N 矩阵各元素的平均值
% c 为输出的矩阵平均值
% m 为输入矩阵
[M,N]=size(m);
m=double(m);
sum=0;
for i=1:M
    for j=1:N
        sum=sum+m(i,j);
    end
end
c=sum/(M*N);

end

