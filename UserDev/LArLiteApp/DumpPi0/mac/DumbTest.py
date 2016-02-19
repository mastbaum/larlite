import sys, os
from ROOT import larlite as fmwk
from seltool import ertool


if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

# Create algorithm

#====Pi0=====
algo_pi0 = ertool.ERAlgoDumbPi0()

# Create MC filter
my_anaunit = fmwk.Pi0Reco()#ExampleERSelection()
my_anaunit.SetMinEDep(50)# Sets min shower to be seen in an event
my_proc = fmwk.ana_processor()
my_proc.enable_filter(True)
my_proc.enable_event_alignment(False)


#Set up the configure params for the type of reco/mc
my_anaunit._mgr.ClearCfgFile()
my_anaunit._mgr.AddCfgFile('/home/ryan/LARLITE/trunk/UserDev/SelectionTool/ERTool/dat/ertool_default_mc.cfg')

# Set up ana for 1pi0 selection
my_ana = ertool.EREffCorrectAna()

my_anaunit._mgr.AddAlgo(algo_pi0)
my_anaunit._mgr.AddAna(my_ana)
my_anaunit._mgr._training_mode =False


# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("Dumb_Pi0.root")



# here set E-cut for Helper & Ana modules
my_anaunit._mgr._mc_for_ana = True

# ***************  Set Producers  ****************
# First Argument: True = MC, False = Reco
my_anaunit.SetShowerProducer(True,"mcreco");
my_anaunit.SetTrackProducer(True,"mcreco");
# ************************************************

my_proc.add_process(my_anaunit)


my_proc.run()


# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

