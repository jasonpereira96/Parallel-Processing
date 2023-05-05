import os

# run chmod +x generate.py before running this

count = 0 


for N in [100000, 250000, 500000, 1000000]:
	for P in [ 2, 4, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64]:
		s_values = list(map(lambda x: int(x), [P, P/2, P/4, 8, 4, 2]))
		for s in s_values:

			if s > 1:
				count = count + 1 
				afile = f'test/A_vector_{N}_{P}_{s}.txt'
				pfile = f'test/pivot_vector_{N}_{P}_{s}.txt'
				command1 = f'qsub -l nodes=1:ppn={P} -v afile="{afile}",pfile="{pfile}",lb="0",s={s} pbs_readings.sh' 
				command2 = f'qsub -l nodes=1:ppn={P} -v afile="{afile}",pfile="{pfile}",lb="1",s={s} pbs_readings.sh' 

				print(command1)
				os.system(command1)

				print(command2)
				os.system(command2)


print(f'{count} jobs scheduled.')

# qsub -l nodes=1:ppn=$P -v exp="$exp",P="1",ppn="$P",id="$i",groupId="$exp-1-P" run.sh