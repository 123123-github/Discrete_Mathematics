function [s]=my_histeq(I,Scale)
% I Ϊ�����ԭʼͼ��
% Scale Ϊ����ĻҶȵȼ�
% s Ϊ�����ֱ��ͼ���⻯���ͼ��

%M,N��¼ͼ������������Ϣ
[M,N]=size(I);
%hist��¼I�ĻҶȷֲ����
hist=imhist(I)';
%nΪI���ص��ܸ��� mΪI�ĻҶȵȼ�
n=M*N;
m=length(hist);
phist=hist/n;

if m==1
    return;
end

for i=2:m
    phist(1,i)=phist(1,i)+phist(1,i-1);
end

% y Ϊ���⻯�ĻҶȱ任���� YΪy��ȡ��
y=phist*Scale;
Y=round(y);

s=I;
for i=0:255
    s(I==i) = Y(1,i+1);
end

end







