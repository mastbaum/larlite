#ifndef LARLITE_DRAWDrawMCTRUTH_CXX
#define LARLITE_DRAWDrawMCTRUTH_CXX

#include "DrawMCTruth.h"

namespace evd {

bool DrawMCTruth::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D").
    //


    return true;
}

bool DrawMCTruth::analyze(larlite::storage_manager* storage) {

    //
    // Do your event-by-event analysis here. This function is called for
    // each event in the loop. You have "storage" pointer which contains
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    //
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    // For now, only taking one mcinfo per event.  So build it and quit:
    _this_info = MCInfoSummary();

    // Get the Drawmctruth info:
    auto ev_mctruth = storage -> get_data<larlite::event_mctruth>(_producer);


    if (ev_mctruth -> size() == 0)
        return false;

    auto _mctruth = ev_mctruth -> front();

    _this_info._vertex = _mctruth.GetNeutrino().Lepton().Trajectory().front().Position().Vect();

    // std::cout << "Neutrino trajectory is :\n" ;
    // for (auto & point : _mctruth.GetNeutrino().Nu().Trajectory() ) {
    //     std::cout << "   ( " << point.X()
    //               << ", " << point.Y()
    //               << ", " << point.Z()
    //               << ")\n";
    // }

    // std::cout << "Vertex is ( " << _this_info.vertex().X()
    //           << ", " << _this_info.vertex().Y()
    //           << ", " << _this_info.vertex().Z()
    //           << ")\n";

    // Fill out the MCInfoSummary object

    _this_info._isCC = _mctruth.GetNeutrino().CCNC();

    _this_info._incoming_lepton_pdg = _mctruth.GetNeutrino().Nu().PdgCode();
    _this_info._outgoing_lepton_pdg = _mctruth.GetNeutrino().Lepton().PdgCode();
    _this_info._target_pdg = _mctruth.GetNeutrino().Target();

    _this_info._outgoing_pdg_list.reserve(_mctruth.GetParticles().size());
    for (auto & part : _mctruth.GetParticles() ) {
        _this_info._outgoing_pdg_list.push_back(part.PdgCode());
    }

    return true;
}

bool DrawMCTruth::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //

    return true;
}

}
#endif
