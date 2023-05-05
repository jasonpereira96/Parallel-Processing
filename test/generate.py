import os
import subprocess

# run chmod +x generate.py before running this
# generates most of the test cases

count = 0 

os.system("rm A_vec*")
os.system("rm pivot*")

for N in [100000, 250000, 500000, 1000000]:
	for P in [ 2, 4, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64]:
		s_values = [P, P/2, P/4, 8, 4, 2]
		for s in s_values:
			_s = int(s)
			cmd_str = f'./input_gen {N} {P} {_s}'
			if _s > 1:
				count = count + 1 
				print(cmd_str)
				os.system(cmd_str)


print(f'count: {count}')	
