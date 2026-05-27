import os
import sys
import pandas as pd
import subprocess
import argparse

masterList = os.environ.get("basePath") + "/e15190/data/runList.csv"
if (os.path.isfile(masterList) == False):
	raise Exception(f"\033[1;31mMaster run list ({masterList}) does not exist\033[0m\n")

def GetRuns(df: pd.DataFrame, **kwargs) -> list:
	filtered = df
	for col, val in kwargs.items():
		if col not in df.columns:
			raise KeyError(f"Column \"{col}\" is not in dataframe. Available: {df.columns.tolist()}")
		filtered = filtered[filtered[col] == val]
	return filtered["Runs"].tolist()

def GetSubmissionArgs(runs: list, projectile: str, target: str, energy: int) -> list:
	systemDir = projectile + target + "_" + str(energy) # template for system storage on HPC and wally
	paths = [""]*len(runs)
	for i in range(len(runs)):
		paths[i] = f"https://data.heavyion.cc/{systemDir}/CalibratedData_{runs[i]}.root"
	return [paths, systemDir]

if __name__ == "__main__":
	df = pd.read_csv(masterList)
	parser = argparse.ArgumentParser(description="Submit runs for a given system")
	parser.add_argument("--projectile", type=str, default="", help="Projectile (e.g. ca40, ca48)")
	parser.add_argument("--target",     type=str, default="", help="Target (e.g. ni64, sn112)")
	parser.add_argument("--energy",     type=int, default=0,  help="Beam energy in MeV/u (e.g. 56, 140)")
	args = parser.parse_args()
	runs = GetRuns(df, Projectile=args.projectile, Target=args.target, Energy=args.energy)
	paths, systemDir = GetSubmissionArgs(runs, args.projectile, args.target, args.energy)
	
	outDir = f"/scratch/e15190/runs/{systemDir}"
	print(f"Processing {len(runs)} runs for {systemDir}", flush=True)
	env = os.environ.copy()
	for i in range(len(runs)):
		print(f"\n----------- [{i+1}/{len(runs)}] run {runs[i]} ----------\n", flush=True)
		env["RUN_NUMBER"]     = str(runs[i])
		env["RUN_INPUT_PATH"] = paths[i]
		env["RUN_OUTPUT_DIR"] = outDir
		try:
			subprocess.run(["./bin/MultCent_Network"], env=env, check=True, timeout=900)
		except subprocess.CalledProcessError as e:
			print(f"! run {runs[i]} failed (exit {e.returncode}), continuing", flush=True)
		except subprocess.TimeoutExpired:
			print(f"! run {runs[i]} timed out after 15min, continuing", flush=True)
