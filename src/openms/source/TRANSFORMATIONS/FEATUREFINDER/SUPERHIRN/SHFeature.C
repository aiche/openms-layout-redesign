// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Florian Zeller $
// $Authors: Lukas Mueller, Markus Mueller $
// --------------------------------------------------------------------------
//
///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//  Ported to OpenMS by Florian Zeller, florian.zeller@bsse.ethz.ch
//  December 2010
//

#include <iostream>
#include <map>
#include <stdio.h>

//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/MSPeak.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/CentroidPeak.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/CentroidData.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/SuperHirnUtil.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/SuperHirnParameters.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/LCElutionPeak.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/BackgroundIntensityBin.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/BackgroundControl.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/LCMSCData.h>
//#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/ProcessData.h>

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/FeatureLCProfile.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/MS2Info.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/MS2Feature.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/SHFeature.h>

namespace OpenMS
{

  using namespace std;

///////////////////////////////////////////
// atomic mass to compute theo mass from
// AA sequence:
  double SHFeature::_MONO_H = 1.00794;
  double SHFeature::_MONO_O = 15.99943;

// tolerance in m/z and TR:
// double SHFeature::PPM_MZ_TOL;
// double SHFeature::MZ_TOL = 0.01;
// double SHFeature::TR_TOL = 0.5;
// double SHFeature::PEPTIDE_PROBABILITY_THRESHOLD = 0.9;

// option to store all low probability scans in the feature
// bool SHFeature::STORE_ALL_LOW_PROBABILITY_MS2_SCANS = false;
// bool SHFeature::PRINT_ALL_ACs = true;

////////////////////////////////////////////////
// constructor for the object feature:
  SHFeature::SHFeature()
  {

    MONO_MZ = -1;
    MONO_MZ_ORIGINAL = -1;
    TR_APEX = -1;
    feature_ID = -1;
    spectrum_ID = -1;
    MASTER_ID = -1;
    scan_apex = -1;
    scan_end = -1;
    scan_start = -1;
    charge_state = -1;
    total_peak_area = -1;
    apex_peak_intensity = -1;
    TR = -1;
    PEAK_SCORE = -1;
    alignment_error_up = 0;
    alignment_error_down = 0;
    SCORE_HOLDER = 0;
    TR_START = -1;
    TR_END = -1;
    MONO_MZ_START = -1;
    MONO_MZ_END = -1;
    feature_match_status = false;
    LCprofile = NULL;
    MS2TraceFeature = NULL;
    PI = 0;
  }

////////////////////////////////////////////////
// constructor for the object feature:
  SHFeature::SHFeature(double IN_MONO_MZ, double IN_TR, int IN_scan_apex, int IN_scan_start, int IN_scan_end,
                       int IN_charge_state, float IN_total_peak_area, float IN_apex_peak_intensity, float IN_PEAK_SCORE)
  {

    MONO_MZ = IN_MONO_MZ;
    MONO_MZ_ORIGINAL = MONO_MZ;
    TR_APEX = IN_TR;
    feature_ID = -1;
    spectrum_ID = -1;
    MASTER_ID = -1;
    scan_apex = IN_scan_apex;
    scan_end = IN_scan_end;
    scan_start = IN_scan_start;
    charge_state = IN_charge_state;
    total_peak_area = IN_total_peak_area;
    apex_peak_intensity = IN_apex_peak_intensity;
    TR = IN_TR;
    PEAK_SCORE = IN_PEAK_SCORE;
    alignment_error_up = 0;
    alignment_error_down = 0;
    SCORE_HOLDER = 0;
    TR_START = TR;
    TR_END = TR;
    MONO_MZ_START = MONO_MZ;
    MONO_MZ_END = MONO_MZ;
    feature_match_status = false;
    PI = 0;

    LCprofile = NULL;
    MS2TraceFeature = NULL;

  }

////////////////////////////////////////////////
// constructor for the object feature:
  SHFeature::SHFeature(MS2Feature * ms2)
  {

    MONO_MZ = ms2->getPrecursorMZ();
    MONO_MZ_ORIGINAL = MONO_MZ;
    TR_APEX = ms2->getStartTR();
    feature_ID = -1;
    spectrum_ID = -1;
    MASTER_ID = -1;
    scan_apex = ms2->getApexScan();
    scan_end = ms2->getEndScan();
    scan_start = ms2->getStartScan();
    charge_state = ms2->getPrecursorChrg();
    total_peak_area = -1;
    apex_peak_intensity = -1;
    TR = ms2->getTR();
    PEAK_SCORE = -1;
    alignment_error_up = 0;
    alignment_error_down = 0;
    SCORE_HOLDER = 0;
    SignalToNoise = 100;
    TR_START = ms2->getStartTR();
    TR_END = ms2->getEndTR();
    MONO_MZ_START = MONO_MZ;
    MONO_MZ_END = MONO_MZ;
    feature_match_status = false;
    PI = 0;

    LCprofile = NULL;
    MS2TraceFeature = NULL;

    addMS2Feature(ms2);

  }

  SHFeature::SHFeature(float IN_pmass, int IN_scan, int IN_charge_state)
  {

    // get real charged peptide m/z
    MONO_MZ = IN_pmass;
    MONO_MZ_ORIGINAL = MONO_MZ;
    feature_ID = -1;
    spectrum_ID = -1;
    MASTER_ID = -1;
    scan_apex = IN_scan;
    scan_end = -1;
    scan_start = -1;
    charge_state = IN_charge_state;
    total_peak_area = 0;
    apex_peak_intensity = 0;
    TR = -1;
    PEAK_SCORE = 0;
    alignment_error_up = 0;
    alignment_error_down = 0;
    SCORE_HOLDER = 0;
    TR_START = TR;
    TR_END = TR;
    TR_APEX = TR;
    MONO_MZ_START = MONO_MZ;
    MONO_MZ_END = MONO_MZ;
    feature_match_status = false;
    PI = 0;

    LCprofile = NULL;
    MS2TraceFeature = NULL;
  }

////////////////////////////////////////////////
// copy constructor:
  SHFeature::SHFeature(const SHFeature & tmp)
  {

    MONO_MZ = tmp.MONO_MZ;
    MONO_MZ_ORIGINAL = tmp.MONO_MZ_ORIGINAL;
    TR_APEX = tmp.TR_APEX;
    feature_ID = tmp.feature_ID;
    spectrum_ID = tmp.spectrum_ID;
    MASTER_ID = tmp.MASTER_ID;
    MS2_SCANS = tmp.MS2_SCANS;
    scan_apex = tmp.scan_apex;
    scan_end = tmp.scan_end;
    scan_start = tmp.scan_start;
    charge_state = tmp.charge_state;
    total_peak_area = tmp.total_peak_area;
    apex_peak_intensity = tmp.apex_peak_intensity;
    TR = tmp.TR;
    PEAK_SCORE = tmp.PEAK_SCORE;
    alignment_error_up = tmp.alignment_error_up;
    alignment_error_down = tmp.alignment_error_down;
    SCORE_HOLDER = tmp.SCORE_HOLDER;
    matched_feature_list = tmp.matched_feature_list;
    TR_START = tmp.TR_START;
    TR_END = tmp.TR_END;
    MONO_MZ_START = tmp.MONO_MZ_START;
    MONO_MZ_END = tmp.MONO_MZ_END;
    feature_match_status = tmp.feature_match_status;
    SignalToNoise = tmp.SignalToNoise;
    BackgroundNoise = tmp.BackgroundNoise;
    PI = tmp.PI;
    featureExtraInformation = tmp.featureExtraInformation;

    MS2TraceFeature = NULL;
    if (tmp.MS2TraceFeature != NULL)
    {
      MS2TraceFeature = new MS2Feature(tmp.MS2TraceFeature);
    }

    LCprofile = NULL;
    if (tmp.LCprofile != NULL)
    {
      LCprofile = new FeatureLCProfile(tmp.LCprofile);
    }

  }

////////////////////////////////////////////////
// constructor for the object feature:
  SHFeature::SHFeature(const SHFeature * tmp)
  {
    MONO_MZ = tmp->MONO_MZ;
    MONO_MZ_ORIGINAL = tmp->MONO_MZ_ORIGINAL;
    TR_APEX = tmp->TR_APEX;
    feature_ID = tmp->feature_ID;
    spectrum_ID = tmp->spectrum_ID;
    MASTER_ID = tmp->MASTER_ID;
    MS2_SCANS = tmp->MS2_SCANS;
    scan_apex = tmp->scan_apex;
    scan_end = tmp->scan_end;
    scan_start = tmp->scan_start;
    charge_state = tmp->charge_state;
    total_peak_area = tmp->total_peak_area;
    apex_peak_intensity = tmp->apex_peak_intensity;
    TR = tmp->TR;
    PEAK_SCORE = tmp->PEAK_SCORE;
    alignment_error_up = tmp->alignment_error_up;
    alignment_error_down = tmp->alignment_error_down;
    SCORE_HOLDER = tmp->SCORE_HOLDER;
    matched_feature_list = tmp->matched_feature_list;
    TR_START = tmp->TR_START;
    TR_END = tmp->TR_END;
    MONO_MZ_START = tmp->MONO_MZ_START;
    MONO_MZ_END = tmp->MONO_MZ_END;
    feature_match_status = tmp->feature_match_status;
    SignalToNoise = tmp->SignalToNoise;
    BackgroundNoise = tmp->BackgroundNoise;
    PI = tmp->PI;
    featureExtraInformation = tmp->featureExtraInformation;

    MS2TraceFeature = NULL;
    if (tmp->MS2TraceFeature != NULL)
    {
      MS2TraceFeature = new MS2Feature(tmp->MS2TraceFeature);
    }

    /////////////////////////////////
    // copy the elution profile:
    LCprofile = NULL;
    if (tmp->LCprofile != NULL)
    {
      LCprofile = new FeatureLCProfile(tmp->LCprofile);
    }

  }

//////////////////////////////////////////////////
// class desctructor

  SHFeature::~SHFeature()
  {

    if (!matched_feature_list.empty())
      matched_feature_list.clear();

    if (!MS2_SCANS.empty())
    {
      MS2_SCANS.clear();
    }

    if (MS2TraceFeature != NULL)
    {
      delete MS2TraceFeature;
      MS2TraceFeature = NULL;
    }

    if (LCprofile != NULL)
    {
      delete LCprofile;
      LCprofile = NULL;
    }

  }

//////////////////////////////////////////////////
// copy operator:
  SHFeature & SHFeature::operator=(const SHFeature & tmp)
  {
    MONO_MZ = tmp.MONO_MZ;
    MONO_MZ_ORIGINAL = tmp.MONO_MZ_ORIGINAL;
    TR_APEX = tmp.TR_APEX;
    feature_ID = tmp.feature_ID;
    spectrum_ID = tmp.spectrum_ID;
    MASTER_ID = tmp.MASTER_ID;
    MS2_SCANS = tmp.MS2_SCANS;
    scan_apex = tmp.scan_apex;
    scan_end = tmp.scan_end;
    scan_start = tmp.scan_start;
    charge_state = tmp.charge_state;
    total_peak_area = tmp.total_peak_area;
    apex_peak_intensity = tmp.apex_peak_intensity;
    TR = tmp.TR;
    PEAK_SCORE = tmp.PEAK_SCORE;
    alignment_error_up = tmp.alignment_error_up;
    alignment_error_down = tmp.alignment_error_down;
    SCORE_HOLDER = tmp.SCORE_HOLDER;
    SignalToNoise = tmp.SignalToNoise;
    BackgroundNoise = tmp.BackgroundNoise;
    matched_feature_list = tmp.matched_feature_list;
    TR_START = tmp.TR_START;
    TR_END = tmp.TR_END;
    MONO_MZ_START = tmp.MONO_MZ_START;
    MONO_MZ_END = tmp.MONO_MZ_END;
    feature_match_status = tmp.feature_match_status;
    PI = tmp.PI;
    featureExtraInformation = tmp.featureExtraInformation;

    if (MS2TraceFeature != NULL)
    {
      delete MS2TraceFeature;
      MS2TraceFeature = NULL;
    }
    if (tmp.MS2TraceFeature != NULL)
    {
      MS2TraceFeature = new MS2Feature(tmp.MS2TraceFeature);
    }

    if (LCprofile != NULL)
    {
      delete LCprofile;
      LCprofile = NULL;
    }

    if (tmp.LCprofile != NULL)
    {
      LCprofile = new FeatureLCProfile(tmp.LCprofile);
    }

    return *this;
  }

/////////////////////////////////////////////////
// equal operator:
  bool SHFeature::operator==(const SHFeature & tmp)
  {

    if (tmp.feature_ID == this->feature_ID)
    {
      return true;
    }

    // compare the charge states:
    if (tmp.charge_state == charge_state)
    {

      // compare the parent masses:
      if (SuperHirnUtil::compareMassValuesAtPPMLevel(tmp.MONO_MZ, get_MZ(),
                                                     SuperHirnParameters::instance()->getMzTolPpm()))
      {

        // copmare the TR:
        if (fabs(tmp.TR - TR) <= SuperHirnParameters::instance()->getTrTol())
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }

//////////////////////////////////////////////////
// show the content of the spectra:
  void SHFeature::show_info()
  {

    string SEP = "";
    // print MS-peak info:
    printf("\tMS1 Feature#:%d,%s", get_feature_ID(), SEP.c_str());
    printf("m/z:%0.5f%s", get_MZ(), SEP.c_str());
    printf("[+%d],%s", get_charge_state(), SEP.c_str());
    printf("Area:%0.2f%s", get_peak_area(), SEP.c_str());
    printf(",apex:%0.2f[%0.2f:%0.2f][%d:%d:%d],s/n:%0.2f,%0.2f%s", get_retention_time(), get_retention_time_START(),
           get_retention_time_END(), get_scan_start(), get_scan_number(), get_scan_end(), getSignalToNoise(),
           get_peak_score(), SEP.c_str());
    printf(",matches:%d%s", get_replicate_match_nb(), SEP.c_str());
    printf(",LCMS-ID: %d", get_spectrum_ID());
    //printf(",M.ID:%d",get_MASTER_ID());
    printf("\n");
    MS2Info * tmp = get_best_MS2_SCAN(0.0);
    if (tmp != NULL)
    {
      tmp->show_info();
    }

    map<int, SHFeature>::iterator P = matched_feature_list.begin();
    while (P != matched_feature_list.end())
    {
      cout << "\t\t-";
      (*P).second.show_info();
      ++P;
    }

    //////////////////////////////////////////////////
    // show MS/MS spectra info:
    showMS2consensSpectraInfo();

  }

//////////////////////////////////////////////////
// show MS/MS spectra info:
  void SHFeature::showMS2consensSpectraInfo()
  {
    if (getMS2Feature() != NULL)
    {
      getMS2Feature()->show_info();
    }
  }

//////////////////////////////////////////////////
// add MS/MS info to the feature:
  void SHFeature::add_MS2_info(MS2Info * IN)
  {

    if (IN != NULL)
    {

      //////////////////////////////////////
      // check if alrady a high quality MS2 available:
      // if StoreAllLowProbMS2Scans is true, then store all
      // low quality scans unless a high quality is available
      // otherwise use only the one with the highest peptide probaility
      MS2Info * thisMS2 = NULL;
      if (SuperHirnParameters::instance()->storeAllLowProbabilityMS2Scans())
      {
        thisMS2 = get_best_MS2_SCAN();
      }
      else
      {
        thisMS2 = get_best_MS2_SCAN(-3.0);
      }

      /////////////////////////////////////////
      // compare with already available MS2 scans:
      if (thisMS2 != NULL)
      {

        // only save the new scan if its of better quality:
        // if so, clear all previous ids to save space:
        if (thisMS2->get_PEP_PROB() < IN->get_PEP_PROB())
        {
          MS2_SCANS.clear();
        }
        else
        {
          // is of lower quality, so through it away:
          IN = NULL;
        }
      }

      /////////////////////////////////////////
      // insert the MS2 scan if its not null:
      if (IN != NULL)
      {

        map<double, vector<MS2Info> >::iterator F = MS2_SCANS.find(IN->get_PEP_PROB());
        if (F == MS2_SCANS.end())
        {
          vector<MS2Info> TMP;
          TMP.push_back(*IN);
          MS2_SCANS.insert(make_pair(IN->get_PEP_PROB(), TMP));
        }
        else
        {
          (*F).second.push_back(*IN);
        }

      }

      IN = NULL;
    }
  }

//////////////////////////////////////////////////
// add MS/MS info to the feature:
  void SHFeature::add_MS2_info(map<double, vector<MS2Info> > * IN)
  {

    MS2_SCANS.clear();
    map<double, vector<MS2Info> >::iterator P = IN->begin();
    while (P != IN->end())
    {
      vector<MS2Info>::iterator M = (*P).second.begin();
      while (M != (*P).second.end())
      {
        add_MS2_info(&(*M));
        ++M;
      }
      ++P;
    }
    IN = NULL;
  }

//////////////////////////////////////////////////
// check how many matches
  int SHFeature::get_nb_common_match()
  {
    return (int) matched_feature_list.size() + 1;
  }

//////////////////////////////////////////////////
// functions to set/access machted features:
  void SHFeature::add_matched_feature(SHFeature * IN)
  {

    //////////////////////////////////////////////////////
    // check charge states, in cases where a feature was
    // created based on a MS2 trace, charge state is unknown ( = -1 )
    // -> derivce the charge state from the matched feature (if this is
    // also not -1
    deriveChargeStates(IN);

    // store the matches of this input features in
    // the new feature:
    map<int, SHFeature>::iterator P = IN->get_match_list_start();
    while (P != IN->get_match_list_end())
    {
      add_matched_feature(&((*P).second));
      ++P;
    }

    // add ms2 information:
    map<double, vector<MS2Info> >::iterator MS2 = IN->get_MS2_SCANS_START();
    while (MS2 != IN->get_MS2_SCANS_END())
    {
      vector<MS2Info>::iterator M = (*MS2).second.begin();
      while (M != (*MS2).second.end())
      {
        add_MS2_info(&(*M));
        ++M;
      }
      ++MS2;
    }

    // ok, now make a new feature and
    // remove its aligned features, since the are now
    // stored in this new feature:
    // feature* TMP = new feature( IN );
    IN->erase_match_list();
    // remove also the MS2 information:
    IN->removeAllMS2Information();

    // check now if this insert ID already exists
    // for example in meta alignments!!!
    // in this case add it to the size of the map
    // new ID = map_size + old_ID:
    int insert_ID = IN->get_spectrum_ID();
    map<int, SHFeature>::iterator F = matched_feature_list.find(insert_ID);
    if (F != matched_feature_list.end())
    {
      insert_ID += (int) matched_feature_list.size();
    }
    matched_feature_list.insert(pair<int, SHFeature>(insert_ID, *IN));
    IN = NULL;

  }

//////////////////////////////////////////////////
// return the sum of all intensities over replicates:
  double SHFeature::get_replicate_intensity_sum()
  {
    double SUM = get_peak_area();
    map<int, SHFeature>::iterator P = matched_feature_list.begin();
    while (P != matched_feature_list.end())
    {
      SUM += (*P).second.get_peak_area();
      ++P;
    }
    return SUM;
  }

//////////////////////////////////////////////////
// get the molecular mass of the corrsponding peptide!
  double SHFeature::get_Molecular_Mass()
  {

    double MR = get_MZ() * double(get_charge_state());

    // remove the H+'s
    MR -= (get_MONO_H() * double(get_charge_state()));

    return MR;
  }

//////////////////////////////////////////////////
// get the best ms2 scan with highest peptide probability:
  MS2Info * SHFeature::get_best_MS2_SCAN()
  {

    MS2Info * OUT = NULL;
    if (get_MS2_info())
    {

      map<double, vector<MS2Info> >::reverse_iterator R = MS2_SCANS.rbegin();
      OUT = &(*((*R).second.begin()));

      map<int, SHFeature>::iterator P = matched_feature_list.begin();
      while (P != matched_feature_list.end())
      {
        MS2Info * TMP = (*P).second.get_best_MS2_SCAN();
        if (TMP != NULL)
        {
          if (TMP->get_PEP_PROB() > OUT->get_PEP_PROB())
          {
            OUT = TMP;
          }
        }
        ++P;
      }
    }
    return OUT;
  }

//////////////////////////////////////////////////
// get the best ms2 scan with highest peptide probability:
  MS2Info * SHFeature::get_best_MS2_SCAN(double PP_T)
  {

    MS2Info * OUT = NULL;
    if (get_MS2_info(PP_T))
    {

      map<double, vector<MS2Info> >::reverse_iterator R = MS2_SCANS.rbegin();
      OUT = &(*((*R).second.begin()));

      map<int, SHFeature>::iterator P = matched_feature_list.begin();
      while (P != matched_feature_list.end())
      {
// MS2Info* TMP = (*P).second.get_best_MS2_SCAN(PP_T);    PKUNSZT THIS SHOULD BE THE CORRECT LINE?
        MS2Info * TMP = (*P).second.get_best_MS2_SCAN();
        if (TMP != NULL)
        {
          if (TMP->get_PEP_PROB() > OUT->get_PEP_PROB())
          {
            OUT = TMP;
          }
        }
        ++P;
      }
    }
    return OUT;
  }

//////////////////////////////////////////////////
// check if a feature is matched with this id:
  bool SHFeature::check_match_by_id(int ID)
  {
    // check if its teh own:
    if (spectrum_ID == ID)
    {
      return true;
    }
    else
    {
      // check the matched features:
      if (matched_feature_list.find(ID) != matched_feature_list.end())
      {
        return true;
      }
      else
      {
        return false;
      }
    }

    return false;
  }

//////////////////////////////////////////////////
// get the total peak are over all matched features:
  double SHFeature::get_MATCHED_peak_area()
  {
    double AREA = get_peak_area();
    map<int, SHFeature>::iterator P = matched_feature_list.begin();
    while (P != matched_feature_list.end())
    {
      AREA += (*P).second.get_peak_area();
      ++P;
    }
    return AREA;
  }

//////////////////////////////////////////////////
// get peak area at a certain LC/MS:
  double SHFeature::get_peak_area(int point)
  {

    // check if its teh own:
    if (spectrum_ID == point)
    {
      return get_peak_area();
    }

    map<int, SHFeature>::iterator P = matched_feature_list.find(point);
    if (P != matched_feature_list.end())
    {
      return (*P).second.get_peak_area();
    }

    return -1.0;

  }

//////////////////////////////////////////////////
// get feature at a certain LC-MS by LC_MS id
  SHFeature * SHFeature::get_feature(int id)
  {

    // check if its teh own:
    if (spectrum_ID == id)
    {
      return this;
    }

    map<int, SHFeature>::iterator P = matched_feature_list.find(id);
    if (P != matched_feature_list.end())
    {
      return &((*P).second);
    }

    return NULL;

  }

//////////////////////////////////////////////////
// get the profile over all matched features:
  map<int, double> SHFeature::get_feature_profile()
  {

    double TOT_AREA = get_MATCHED_peak_area();
    map<int, double> PROFILE;
    PROFILE.insert(make_pair(get_spectrum_ID(), get_peak_area() / TOT_AREA));

    map<int, SHFeature>::iterator P = matched_feature_list.begin();
    while (P != matched_feature_list.end())
    {
      PROFILE.insert(make_pair((*P).second.get_spectrum_ID(), (*P).second.get_peak_area() / TOT_AREA));
      ++P;
    }
    return PROFILE;
  }

//////////////////////////////////////////////////
// Molecular Mass computed over matched features:
  double SHFeature::get_profile_Molecular_Mass()
  {

    double mz = get_MZ();
    map<int, SHFeature>::iterator P = get_match_list_start();
    while (P != get_match_list_end())
    {
      mz += (*P).second.get_MZ();
      ++P;
    }
    mz /= double(get_replicate_match_nb());
    // compute MR:
    double MR = mz * double(get_charge_state());
    // remove the H+'s
    MR -= (get_MONO_H() * double(get_charge_state()));

    return MR;
  }

//////////////////////////////////////////////////
// retention time computed over matched features:
  double SHFeature::get_profile_retention_time()
  {
    double all_tr = TR;
    map<int, SHFeature>::iterator P = get_match_list_start();
    while (P != get_match_list_end())
    {
      all_tr += (*P).second.get_retention_time();
      ++P;
    }
    all_tr /= double(get_replicate_match_nb());
    return all_tr;
  }

//////////////////////////////////////////////////
// check if MS2 info is available:
  bool SHFeature::get_MS2_info()
  {

    // check if list is empty:
    if (MS2_SCANS.empty())
    {
      return false;
    }
    else
    {

      // check now if any ms2 info above PEP.PROP. THRESHOLD:
      map<double, vector<MS2Info> >::iterator F = MS2_SCANS.lower_bound(
        SuperHirnParameters::instance()->getPeptideProbabilityThreshold());
      if (F == MS2_SCANS.end())
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  }

//////////////////////////////////////////////////
// check if MS2 info is available:
  bool SHFeature::get_MS2_info(double PP_THRES)
  {

    // check if list is empty:
    if (MS2_SCANS.empty())
    {
      return false;
    }
    else
    {

      // check now if any ms2 info above PEP.PROP. THRESHOLD:
      map<double, vector<MS2Info> >::iterator F = MS2_SCANS.lower_bound(PP_THRES);
      if (F == MS2_SCANS.end())
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  }

//////////////////////////////////////////////////////
// check charge states, in cases where a feature was
// created based on a MS2 trace, charge state is unknown ( = -1 )
// -> derivce the charge state from the matched feature (if this is
// also not -1
  void SHFeature::deriveChargeStates(SHFeature * IN)
  {

    SHFeature * deriveFeature = NULL;
    SHFeature * adjustFeature = NULL;

    // find the feature to derive:
    if (IN->get_charge_state() == -1)
    {
      adjustFeature = IN;
    }
    else if (this->get_charge_state() == -1)
    {
      adjustFeature = this;
    }

    // find the feature to derive:
    if (IN->get_charge_state() > 0)
    {
      deriveFeature = IN;
    }
    else if (this->get_charge_state() > 0)
    {
      deriveFeature = this;
    }

    // make a derived charge state:
    if ((deriveFeature != NULL) && (adjustFeature != NULL))
    {

      adjustFeature->set_charge_state(deriveFeature->get_charge_state());

      // adjust the mapped ones:
      map<int, SHFeature>::iterator P = adjustFeature->get_match_list_start();
      while (P != adjustFeature->get_match_list_end())
      {
        P->second.set_charge_state(deriveFeature->get_charge_state());
        ++P;
      }
    }

    deriveFeature = NULL;
    adjustFeature = NULL;

  }

/*
 //////////////////////////////////////////////////////
 // compare to masses at the PPM value and decided
 // if they fall into the m/z tolerance window
 bool SHFeature::compareFeatureMassValuesAtPPMLevel(double mzA, double mzB)
 {
 return SimpleMath2::compareMassValuesAtPPMLevel(mzA, mzB, SuperHirnParameters::instance()->getMzTolPpm());
 }

 //////////////////////////////////////////////////////
 // get the masse error at the PPM value
 double SHFeature::getFeatureMassErrorAtPPMLevel(double mz)
 {
 return SimpleMath2::getMassErrorAtPPMLevel(mz, SuperHirnParameters::instance()->getMzTolPpm());
 }
 */

}
