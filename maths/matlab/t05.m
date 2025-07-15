clear;
clc;
hf=figure;
%建立窗口并返回句柄hf
set(hf,'menubar','none')
%去除系统菜单
hm=uimenu(hf,'label','我的菜单');
hm_exgrid = uimenu(hm,'label','调用内联函数','callback','alnn');
%建立子菜单’调用回调函数’，并调用函数alnn（自己定义）

alnn=inline('msgbox(''你要显示的内容'', ''对话框标题'')');