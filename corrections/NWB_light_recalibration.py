import warnings
warnings.filterwarnings('ignore')

import numpy as np
import pandas as pd
from scipy.optimize import curve_fit

def get_old_pulse_height_calibration(
    url='https://raw.githubusercontent.com/nscl-hira/E15190-Unified-Analysis-Framework/d8e18dc5a962b7b5b9d358ee876ef777ff062917/calibrations/NWB_PulseHeightCalibration.dat',
):
    df = pd.read_csv(url, sep='\s+', skiprows=1, header=None).loc[:, :5]
    df.columns = ['bar', 'a', 'b', 'c', 'd', 'e']
    df['bar'] = df['bar'].str.extract('(\d+)', expand=False).astype(int)
    return df.set_index('bar')
df_old = get_old_pulse_height_calibration()

def get_ambe_compton_edges(
    url='https://raw.githubusercontent.com/fanurs/data-analysis-e15190-e14030/main/database/neutron_wall/ambe/compton_edges_3072-3073.csv',
):
    return pd.read_csv(url).set_index('bar')
df_ambe = get_ambe_compton_edges()

# functions to fit
# https://github.com/nscl-hira/E15190-Unified-Analysis-Framework/blob/4521b84863cf8d56124e30f2e9ef2a0627e11711/NWPulseHeightCalibration.cpp
def get_light(chn, x, a, b, c, d, e):
    result = chn / (a + b * x + c * x ** 2)
    result = d + result * (4.196 * e)
    return result

def get_adc(light, x, a, b, c, d, e):
    result = (light - d) / (4.196 * e)
    result = result * (a + b * x + c * x ** 2)
    return result

# updated the calibration points (MeVee)
# using Geant4 simulation, with the correct NW bar dimensions
old_calib_lights = {
    'pedestal': 0.0, 'AmBe': 4.196,
    '90 deg': 11.02, '44.4 deg': 15.6538, '56.3 deg': 20.3242,
}
new_calib_lights = {
    'pedestal': 0.0, 'AmBe': 4.196,
    '90 deg': 11.96, '44.4 deg': 17.0536, '56.3 deg': 22.1550,
}

# a draw routine for every bar
def routine(bar, ax):
    global df_old
    df_bar = df_old.loc[bar]
    old_data = pd.DataFrame(
        [
            [old_calib_lights['pedestal'], 0.0],
            [old_calib_lights['AmBe'], get_adc(old_calib_lights['AmBe'], 0, *df_bar)],
            [old_calib_lights['90 deg'], get_adc(old_calib_lights['90 deg'], 0, *df_bar)],
            [old_calib_lights['44.4 deg'], get_adc(old_calib_lights['44.4 deg'], 0, *df_bar)],
            [old_calib_lights['56.3 deg'], get_adc(old_calib_lights['56.3 deg'], 0, *df_bar)],
        ],
        columns=['light', 'adc'],
    )
    old_data.index = list(old_calib_lights.keys())

    new_data = pd.DataFrame(
        [
            [new_calib_lights['pedestal'], 0.0],
            [new_calib_lights['AmBe'], df_ambe.loc[bar, 'p0']],
            [new_calib_lights['90 deg'], old_data.loc['90 deg', 'adc']],
            [new_calib_lights['44.4 deg'], old_data.loc['44.4 deg', 'adc']],
            [new_calib_lights['56.3 deg'], old_data.loc['56.3 deg', 'adc']],
        ],
        columns=['light', 'adc'],
    )
    new_data.index = list(new_calib_lights.keys())

    # the quadratic part (position dependence) would be from new AmBe fit
    # the linear part (calibration) would be re-fitted with the new data
    df_ambe_bar = df_ambe.loc[bar]
    x0 = (df_ambe_bar.p0, df_ambe_bar.p1, df_ambe_bar.p2, df_bar.d, df_bar.e)
    par = curve_fit(
        lambda light, *args: get_adc(light, 0, *args),
        new_data['light'], new_data['adc'],
        p0=x0,
        bounds=(
            (x0[0] - 5e-2, x0[1] - 5e-3, x0[2] - 5e-5, x0[3] - 0.25, x0[4] - 0.25),
            (x0[0] + 5e-2, x0[1] + 5e-3, x0[2] + 5e-5, x0[3] + 0.25, x0[4] + 0.25),
        ),
    )[0]

    light = np.linspace(-1, 100, 200) # MeVee
    ax.plot(light, get_adc(light, 0, *df_bar), color='black', linewidth=0.3)
    ax.plot(light, get_adc(light, -100, *df_bar), color='gray', linewidth=0.3, linestyle='dashed')
    ax.plot(light, get_adc(light, 100, *df_bar), color='gray', linewidth=0.3, linestyle='dashed')
    ax.plot(light, get_adc(light, 0, *par), color='red', linewidth=0.3)
    ax.plot(light, get_adc(light, -100, *par), color='pink', linewidth=0.3, linestyle='dashed')
    ax.plot(light, get_adc(light, 100, *par), color='pink', linewidth=0.3, linestyle='dashed')
    ax.errorbar(old_data['light'], old_data['adc'], label='old', fmt='s', markersize=1, zorder=100, color='black')
    ax.errorbar(new_data['light'], new_data['adc'], label='new', fmt='s', markersize=1, zorder=100, color='red')
    return par


# draw summary plot
import matplotlib as mpl
import matplotlib.pyplot as plt
mpl.rcParams.update({
    'font.family': 'serif',
    'xtick.top': True,
    'xtick.direction': 'in',
    'xtick.minor.visible': True,
    'ytick.right': True,
    'ytick.direction': 'in',
    'ytick.minor.visible': True,
})

fig, ax = plt.subplots(ncols=6, nrows=4, figsize=(12, 9), dpi=300, constrained_layout=True, facecolor='white')
df_new = []
for bar in range(1, 25):
    rc = (int((bar - 1) / 6), (bar - 1) % 6)
    ax[rc].set_title(f'bar-{bar:02d}')
    ax[rc].set_xlim(-1, 80)
    ax[rc].set_ylim(-100, 4100)
    ax[rc].set_xticks([0, 20, 40, 60, 80])
    ax[rc].set_yticks([0, 1000, 2000, 3000, 4000])
    ax[rc].grid(linestyle='dashed', linewidth=0.1, color='gray', zorder=1)
    par = routine(bar, ax[rc])
    df_new.append(par)
    if rc[0] == 3:
        ax[rc].set_xlabel('Light (MeVee)')
    else:
        ax[rc].set_xticklabels([])
    if rc[1] == 0:
        ax[rc].set_ylabel('GeoMean ADC')
    else:
        ax[rc].set_yticklabels([])
ax[0, 0].legend(loc='upper left', handletextpad=0.05)
plt.savefig('output.png', dpi=600, bbox_inches='tight')
plt.draw()
# plt.show()

df_new = pd.DataFrame(df_new, columns=['a', 'b', 'c', 'd', 'e'])
df_new.insert(0, '*numbar', [f'NWBbar{bar:02d}' for bar in range(1, 25)])
try:
    import tables
    tables.to_fwf(df_new, 'output.dat')
except ImportError:
    df_new.to_csv('output.dat', index=False, sep='\t')

print('Done')
