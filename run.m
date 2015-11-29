times_seq = dlmread('running-times/seq-times.txt');
times_par = dlmread('running-times/par-times.txt');

n = [10 100 1000 10000 100000 1000000 10000000 100000000 1000000000];


loglog(n(1:8), times_seq(1:8), 'r-', n(1:8), times_par(1:8), 'g-');

grid;
xlabel('log N');
ylabel('Log Running Time (s)');
title('Running Times of Sequential vs Parallel Quicksort');
legend('Sequential', 'Parallel');

hold on;
