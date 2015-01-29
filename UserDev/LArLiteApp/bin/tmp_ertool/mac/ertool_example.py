import sys

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import ertool
ertool.Manager()
from ROOT import larlite as fmwk
fmwk.geo.PlaneID


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ertool_hist.root")

# Create an empty (baseclass) algorithm
my_algo = ertool.AlgoBase()

# Create an empty (baseclass) filter
my_filter = ertool.FilterBase()

# Create an analysis unit ... ExampleERSelection is LArLite interface
my_ana = fmwk.ExampleERSelection()

# Set Producers ... First Argument: True = MC, False = Reco
my_ana.SetShowerProducer(True,"mcreco");
my_ana.SetTrackProducer(True,"mcreco");
my_ana.SetVtxProducer(True,"generator");

# Attach algorithm & filter to ERTool manager 
my_ana._mgr.SetAlgo(my_algo)
my_ana._mgr.SetFilter(my_filter)

my_proc.add_process(my_ana)
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

