function [ s ] = my_medfilt2( r )
%����ʵ�ֶ�ͼ�� r ���� 5*5 ��ֵ�˲�
% r Ϊ����ͼ�� ���� r Ϊ uint8 ����
% s Ϊ���ͼ�� ���� s Ϊ uint8 ����
[M,N]=size(r);
s=uint8(zeros(M,N));
m=uint8(zeros(5,5));
%�������ص�
for i=3:M-2
    for j=3:N-2
        %�Ա����������ص㣬���� 5*5 �����ֵ�������� m 
        for ii=1:5
            for jj=1:5
                m(ii,jj)=r(i-3+ii,j-3+jj);
                %���� sort() �������򣬽��м�ֵ�������� s ����ӦԪ��
                temp=sort(m(:));
                s(i,j)=temp(13,1);
            end
        end
        %�Ա����������ص㣬���� 5*5 �����ֵ�������� m ����
    end
end

end