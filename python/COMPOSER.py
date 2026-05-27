import os
import sys
import subprocess
from multiprocessing import Pool, current_process, cpu_count

def AppendRuns(runs, low, high) -> list:
	for i in range(low, high+1):
		runs.append(i)
	return runs

runs = []


# ------------------------------------------------ 

# Ca40 + Ni64 @ 56.0 MeV block - complete!
AppendRuns(runs, 2512, 2523) 

# Ca40 + Ni64 @ 140.0 MeV block - complete!
AppendRuns(runs, 2265, 2265)	
AppendRuns(runs, 2723, 2744)
#
# Ca48 + Ni64 @ 56.0 MeV block
AppendRuns(runs, 4383, 4414)
AppendRuns(runs, 4466, 4493)
AppendRuns(runs, 4543, 4573)
AppendRuns(runs, 4642, 4661)

# Ca48 + Ni64 @ 140.0 MeV block
AppendRuns(runs, 4023, 4032)
AppendRuns(runs, 4082, 4123)
AppendRuns(runs, 4224, 4245)
AppendRuns(runs, 4315, 4327)

# ------------------------------------------------ 

# Ca40 + Ni58 @ 56.0 MeV block - ALL
AppendRuns(runs, 2333, 2354)
AppendRuns(runs, 2378, 2394)
AppendRuns(runs, 2485, 2489)
AppendRuns(runs, 2491, 2494)
AppendRuns(runs, 2495, 2495)
AppendRuns(runs, 2497, 2510)

# Ca40 + Ni58 @ 140.0 MeV block  - ALL
AppendRuns(runs, 2134, 2135) 
AppendRuns(runs, 2142, 2152)
AppendRuns(runs, 2155, 2200)
AppendRuns(runs, 2266, 2270)
AppendRuns(runs, 2272, 2280)	
AppendRuns(runs, 2565, 2611)	
AppendRuns(runs, 2632, 2656)	
AppendRuns(runs, 2848, 2870)

# Ca48 + Ni58 @ 56.0 MeV block
AppendRuns(runs, 4495, 4509)
AppendRuns(runs, 4610, 4619)

# Ca48 + Ni58 @ 140.0 MeV block
AppendRuns(runs, 4136, 4140)
AppendRuns(runs, 4212, 4219)
AppendRuns(runs, 4255, 4264)
AppendRuns(runs, 4308, 4313)

# ------------------------------------------------ 

# Ca40 + Sn112 @ 56.0 MeV block
AppendRuns(runs, 2289, 2311) 
AppendRuns(runs, 2316, 2331)
AppendRuns(runs, 2357, 2376)

# Ca40 + Sn112 @ 140.0 MeV block
AppendRuns(runs, 2204, 2221) # should be here in just a second
AppendRuns(runs, 2222, 2233)
AppendRuns(runs, 2237, 2255)
AppendRuns(runs, 2259, 2264)
AppendRuns(runs, 2614, 2630)
AppendRuns(runs, 2830, 2846)

# Ca48 + Sn112 @ 56.0 MeV block
AppendRuns(runs, 4448, 4464)
AppendRuns(runs, 4593, 4605)

# Ca48 + Sn112 @ 140.0 MeV block
AppendRuns(runs, 4125, 4132)
AppendRuns(runs, 4203, 4208)
AppendRuns(runs, 4247, 4253)
AppendRuns(runs, 4302, 4306)

# ------------------------------------------------ 

# Ca40 + Sn124 @ 56.0 MeV block
AppendRuns(runs, 2529, 2545)

# Ca40 + Sn124 @ 140.0 MeV block
AppendRuns(runs, 2077, 2078)
AppendRuns(runs, 2710, 2721)

# Ca48 + Sn124 @ 56.0 MeV block
AppendRuns(runs, 4333, 4380)
AppendRuns(runs, 4511, 4541)
AppendRuns(runs, 4621, 4640)

# Ca48 + Sn124 @ 140.0 MeV block
AppendRuns(runs, 4007, 4007)
AppendRuns(runs, 4042, 4058)
AppendRuns(runs, 4060, 4078)
AppendRuns(runs, 4142, 4150)
AppendRuns(runs, 4190, 4192)
AppendRuns(runs, 4194, 4195)
AppendRuns(runs, 4196, 4201)
AppendRuns(runs, 4266, 4300)

# ------------------------------------------------ 

def getWorkerID():
	name = current_process().name
	return int(name.split("-")[-1]) - 1


def run_correlation(run):
	workID = getWorkerID()
	prefix = "\t"*3*workID
	sys.stdout.write(f"{prefix}\033[1;33mStarting run {run}\033[0m\n")
	sys.stdout.flush()

	cmd = ["./exec_Correlation", str(run), "input.env"]
	result = subprocess.run(cmd, capture_output=True)
	if result.returncode != 0:
		sys.stdout.write(f"{prefix}\033[1;31m{run} FAILED\033[0m\n")
	else:
		sys.stdout.write(f"{prefix}\033[1;32m{run} COMPLETE\033[0m\n")
	sys.stdout.flush()
	return (run, result.returncode == 0)

if __name__ == "__main__":

	nworkers = 3 # trying to not break fishtank
	sys.stdout.write(f"Running {len(runs)} runs on {nworkers} cores\n")
	sys.stdout.flush()

	with Pool(nworkers) as pool:
		list(pool.imap_unordered(run_correlation, runs))
