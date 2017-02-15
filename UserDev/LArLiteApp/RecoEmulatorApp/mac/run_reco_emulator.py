import sys

if len(sys.argv) < 3:
    msg  = '\n'
    msg += "Usage 1: %s $CONFIG_FILE $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-3):
    my_proc.add_input_file(sys.argv[x+2])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kBOTH)

# Specify output root file name
my_proc.set_ana_output_file(sys.argv[3] + "ana.root")

my_proc.set_output_file(sys.argv[3] + "out.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.EmuDriver()
my_unit.set_config(sys.argv[1])
my_proc.add_process(my_unit)

my_proc.set_data_to_read(fmwk.data.kMCTrack,my_unit.mctrack_producer())
my_proc.set_data_to_read(fmwk.data.kMCShower,my_unit.mcshower_producer())

my_proc.set_data_to_write(fmwk.data.kTrack,"recoemu")
my_proc.set_data_to_write(fmwk.data.kShower,"recoemu")

print
print  "Finished configuring ana_processor. Start event loop!"
print

# Let's run it.
my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

sys.exit(0)
