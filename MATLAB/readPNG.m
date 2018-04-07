
clear

[A,map,alpha] = imread('企鹅.png');                 % (ICO, CUR, and PNG only)
out = fopen('penguin.txt','w');
for i=1:64
    for j=1:64
        fprintf(out,'%d,%d,%d,%d,',A(i,j,1),A(i,j,2),A(i,j,3),alpha(i,j));
    end
end
fclose(out);

clear

[A,map,alpha] = imread('窗口.png');                 % (ICO, CUR, and PNG only)
out = fopen('window.txt','w');
for i=1:64
    for j=1:64
        fprintf(out,'%d,%d,%d,%d,',A(i,j,1),A(i,j,2),A(i,j,3),alpha(i,j));
    end
end
fclose(out);
clear

[A,map,alpha] = imread('向日葵.png');                 % (ICO, CUR, and PNG only)
out = fopen('flower.txt','w');
for i=1:64
    for j=1:64
        fprintf(out,'%d,%d,%d,%d,',A(i,j,1),A(i,j,2),A(i,j,3),alpha(i,j));
    end
end
fclose(out);
