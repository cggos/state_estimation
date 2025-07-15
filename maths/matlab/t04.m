function [B,ix]=sort_grade(A1,A2,A3,A4,A5,A6);
%SORT_GRADE sort the grades of six classes

A=[A1;A2;A3;A4;A5;A6];
[B,iy]=sort(A);
a1=mean(A1);
a2=mean(A2);
a3=mean(A3);
a4=mean(A4);
a5=mean(A5);
a6=mean(A6);
[s,ix]=max([a1 a2 a3 a4 a5 a6]);