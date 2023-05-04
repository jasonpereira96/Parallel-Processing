import os
import subprocess

for N in [100000, 250000, 500000, 1000000]:
	for P in [ 2, 4, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64]:
		s_values = [P, P/2, P/4, 8, 4, 2]
		for s in s_values:
			cmd_str = f'./input_gen {N} {P} {int(s)}'
			print(cmd_str)
			os.system(cmd_str)
		