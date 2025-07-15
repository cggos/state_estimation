%坐标图
title('test')
xlabel('x1')
ylabel('y1')
text(3,5,'sin({/omega}t+{/beta})') % 'the useful {/bf MATLAB}'
legend('','');

axis([1 10 0 10])    %xmin xmax ymin ymax zmin zmax
axis equal %纵横坐标轴采用等长刻度
axis square %产生正方形坐标系（默认为矩形）
axis auto %使用默认设置
% axis off %取消坐标轴
axis on %显示坐标轴

% subplot(2,3,4)
grid on
box on

% % 在线性直角坐标中，条形图、阶梯图、杆图和填充图所采用的函数分别为：
% bar(x,y,'g')
% stairs(x,y,'')
% stem(x,y,'')
% fill(x,y,'')
% 
% %极坐标
% polar() 
% 
% %对数坐标
% semilogx()
% semilogy()
% loglog()
% 
% %二维绘图
% plot()
% 
% %三维绘图
% plot3()
% 
% %平面网格坐标矩阵的生成
% %利用矩阵运算生成。
% x=a:dx:b;
% y=(c:dy:d)';
% X=ones(size(y))*x;
% Y=y*ones(size(x));
% %利用meshgrid函数生成；
% x=a:dx:b;
% y=c:dy:d;
% [X,Y]=meshgrid(x,y);
% 
% %mesh函数用来绘制三维网格图，而surf用来绘制三维曲面图，各线条之间的补面用颜色填充
% mesh(x,y,z,c)
% surf(x,y,z,c)


%变量
% save 文件名.mat
% load 文件名.mat
clear 

%向量
    %列向量：分号
    %行向量：逗号或空格
%等间距向量
    %行向量：冒号或linspace()
    %列向量：行向量+撇号
%矩阵
    %方括号 或 函数
%级联：方括号
    %纵级联：分号
    %横级联：逗号
    
%帮助
    %命令：doc

%索引从1开始

