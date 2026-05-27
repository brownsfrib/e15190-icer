import os
import sys
import pandas as pd

def ExplodeRuns(run_str) -> list:
	explosion = []
	for series in run_str.replace(" ", "").split(":"):
		if not series:
			continue
		if "-" in series:
			start, end = series.split("-")
			explosion.extend(range(int(start), int(end)+1))
		else:
			explosion.append(int(series))
	return explosion

def ConstructFrame(filePath) -> pd.DataFrame:
	trueCSV 	= pd.read_csv(filePath)
	trueCSV["Runs"] = trueCSV["Runs"].apply(ExplodeRuns)	
	df = trueCSV.explode("Runs", ignore_index=True)
	df["Runs"] = df["Runs"].astype(int)
	return df

# Loading in existing runs file for filtering
def LoadExistingRuns(existing_runs) -> set:
	with open(existing_runs) as file:
		return {int(line.strip()) for line in file if line.strip()}

def FilterFrame(df, existing_run_path) -> pd.DataFrame:
	existing_runs = LoadExistingRuns(existing_run_path)
	dfSize = len(df)
	filtered = df[df["Runs"].isin(existing_runs)].reset_index(drop=True)
	return filtered


if __name__ == "__main__":
	runIndex = os.environ.get("basePath") + "/e15190/data/runIndex.txt"
	if (os.path.isfile(runIndex) == False):
		raise Exception(f"\033[1;31mrunIndex file ({runIndex}) does not exist\033[0m\n")

	runList = os.environ.get("basePath") + "/e15190/data/runList.txt"
	if (os.path.isfile(runList) == False):
		raise Exception(f"\033[1;31mrunList file ({runList}) does not exist\033[0m\n")

	df = FilterFrame(ConstructFrame(runIndex), runList)

	df.to_csv("output.csv", index=False)


