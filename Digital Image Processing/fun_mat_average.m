function [ c ] = fun_mat_average( m )
%�ú���ʵ�ּ��� M*N �����Ԫ�ص�ƽ��ֵ
% c Ϊ����ľ���ƽ��ֵ
% m Ϊ�������
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

