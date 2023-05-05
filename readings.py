import os
import time

# run chmod +x generate.py before running this

count = 0 
SLEEP_TIME = 0.5

def getCombo(P):
	combos = {
		2: {'nodes': 1, 'ppn': 2},
		4: {'nodes': 1, 'ppn': 4},
		8: {'nodes': 1, 'ppn': 8},
		12: {'nodes': 1, 'ppn': 12},
		16: {'nodes': 1, 'ppn': 16},
		20: {'nodes': 1, 'ppn': 20},
		24: {'nodes': 2, 'ppn': 12},
		32: {'nodes': 2, 'ppn': 16},
		40: {'nodes': 2, 'ppn': 20},
		48: {'nodes': 4, 'ppn': 12},
		56: {'nodes': 4, 'ppn': 14},
		64: {'nodes': 4, 'ppn': 16}
	}
	return combos[P]

done = set()


# for N in [100000, 250000, 500000, 1000000]:
for N in [100000]:
	for P in [ 2, 4, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64]:
		s_values = list(map(lambda x: int(x), [P, P/2, P/4, 8, 4, 2]))
		for s in s_values:

			if s > 1 and s <= P:

				key = f'{N}_{P}_{s}' # to prevent duplicate jobs

				if key in done:
					continue

				count = count + 2
				afile = f'test/A_vector_{N}_{P}_{s}.txt'
				bfile = f'test/pivot_vector_{N}_{P}_{s}.txt'

				combo = getCombo(P)
				nodes = combo['nodes']
				ppn = combo['ppn']

				# with load balancing
				command1 = f'qsub -l nodes={nodes}:ppn={ppn} -v afile="{afile}",bfile="{bfile}",lb="1",s="{s}" pbs_readings.sh' 
				print(command1)
				os.system(command1)

				time.sleep(SLEEP_TIME)

				# without load balancing
				command2 = f'qsub -l nodes={nodes}:ppn={ppn} -v afile="{afile}",bfile="{bfile}",lb="0",s="{s}" pbs_readings.sh' 
				print(command2)
				os.system(command2)

				time.sleep(SLEEP_TIME)

				done.add(key)

print(f'{count} jobs scheduled.')

# qsub -l nodes=1:ppn=$P -v exp="$exp",P="1",ppn="$P",id="$i",groupId="$exp-1-P" run.sh
