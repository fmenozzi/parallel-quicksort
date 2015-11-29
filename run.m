times_seq = dlmread('running-times/seq-times.txt');
times_par = dlmread('running-times/par-times.txt');

m = 7;

n = logspace(1,m,m);

semilogx(n, times_seq(1:m), 'r-', n, times_par(1:m), 'g-');

grid;
xlabel('N');
ylabel('Running Time (s)');
title('Running Times of Sequential vs Parallel Quicksort');
legend('Sequential', 'Parallel');

hold on;
