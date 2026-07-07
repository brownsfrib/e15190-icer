import os
import pandas as pd

def openRunList() -> pd.DataFrame:
	pd
	return

def openYCMList() -> pd.DataFrame:
	return

if __name__ == "__main__":
	runList = pd.read_csv("./runList.csv")
	y_cmList = pd.read_csv("./y_cm_lookup.csv")

	result = runList.merge(y_cmList, on=['Projectile', 'Target', 'Energy'], how='left')

	result.to_csv("result.csv", index=False)




