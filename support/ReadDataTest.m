clear all;
close all;
clc;

testFile = "data1.txt"
testData = ES410ReadData(testFile);

P = testData(:,"P");
V = testData(:,"V");
A = testData(:,"A");
PRaw = testData(:,"PRaw");
ARaw = testData(:,"ARaw");

figure(1);
hold on;
plot(table2array(P));
plot(table2array(V));
plot(table2array(A));
legend({"P" "V" "A"})

figure(2);
hold on;
plot(table2array(ARaw));
plot(table2array(A));
legend({"ARaw" "A"})


figure(3);
hold on;
plot(table2array(PRaw)*(-1));
plot(table2array(P));
legend({"PRaw" "P"})