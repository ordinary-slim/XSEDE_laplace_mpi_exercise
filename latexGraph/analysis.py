import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import scipy as sp
from scipy.ndimage import uniform_filter1d
import seaborn as sns
sns.set_style("darkgrid", {"axes.facecolor": ".9"})

# Read csv into dataframe
df = pd.read_csv("strongScaling.csv")

nprocs  = np.array( df["nprocs[]"])
cputime = np.array( df["time[s]"] ) #[s]
smooth_cputime = uniform_filter1d( cputime, size = 3)

# find local minima
minima = np.r_[True, smooth_cputime[1:] < smooth_cputime[:-1]] & np.r_[smooth_cputime[:-1] < smooth_cputime[1:], True]
idx_minima = [i for i, l in enumerate( minima ) if l]
plt_idx_minima = [val for i, val in enumerate(idx_minima[:-1]) if ((idx_minima[i+1] - idx_minima[i]) > 4)]
plt_idx_minima.append( idx_minima[-2] )

oneOverX = np.array( [(cputime[0] / (idx+1)) for idx in range( len(nprocs) ) ] )

plt.figure( figsize=(8,6), dpi=300 )
plt.scatter( nprocs, cputime, facecolors='none', edgecolors='darkslateblue', label='Execution time' )
plt.plot( nprocs,  oneOverX, '--', color='darkolivegreen', label="$1/x$ scaling")
plt.legend()

yticks = []
yticks.extend( cputime[0:4] )
yticks.extend( [cputime[7], cputime[15]] )
yticks.append( round(min(cputime), 2) )
plt.yticks( yticks )

plt.xticks( nprocs[plt_idx_minima] )

plt.xlim( [0, nprocs[-1]])

plt.xlabel( "# processors []" )
plt.ylabel( "CPU time [s]" )

plt.rcParams["font.serif"] = "Times"

# Save figure
plt.savefig("strongScaling.png")
