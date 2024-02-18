clear all;
close all;
clc;

testFile = "dataDesk6.txt"
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

arrPRaw = table2array(PRaw);
arrARaw = table2array(ARaw);
PRaw2000 = arrPRaw(1:2000);
ARaw2000 = arrARaw(1:2000);
muP = mean(PRaw2000)
muA = mean(ARaw2000)
sdP = std(PRaw2000)
sdA = std(ARaw2000)

cov(PRaw2000, ARaw2000)

ToFUpdated = table2array(testData(:,"ToFUpdate"));
tToFSample = table2array(testData(:,"tToFSample"));
tIMUSample = table2array(testData(:,"tIMUSample"));
dtToFSample = diff(tToFSample);%(find(ToFUpdated)));
dtIMUSample = diff(tIMUSample);

figure(5);
hold on;
plot(dtToFSample);
plot(dtIMUSample);
legend({"dtToFSample" "dtIMUSample"})


