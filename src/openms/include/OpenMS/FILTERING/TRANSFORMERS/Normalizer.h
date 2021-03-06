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
// $Maintainer: Mathias Walzer $
// $Authors: $
// --------------------------------------------------------------------------
//
#ifndef OPENMS_FILTERING_TRANSFORMERS_NORMALIZER_H
#define OPENMS_FILTERING_TRANSFORMERS_NORMALIZER_H

#include <OpenMS/KERNEL/StandardTypes.h>

#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>

#include <vector>

namespace OpenMS
{
  /**
    @brief Normalizer normalizes the peak intensities

        @htmlinclude OpenMS_Normalizer.parameters

        @ingroup SpectraPreprocessers
  */
  class OPENMS_DLLAPI Normalizer :
    public DefaultParamHandler
  {
public:

    // @name Constructors and Destructors
    // @{
    /// default constructor
    Normalizer();
    /// desctructor
    virtual ~Normalizer();

    /// assignment operator
    Normalizer & operator=(const Normalizer & source);
    /// copy constructror
    Normalizer(const Normalizer & source);

    // @}

    // @name Accessors
    // @{

    ///
    template <typename SpectrumType>
    void filterSpectrum(SpectrumType & spectrum)
    {
      typedef typename SpectrumType::Iterator Iterator;
      typedef typename SpectrumType::ConstIterator ConstIterator;

      method_ = param_.getValue("method");

      // normalizes the max peak to 1 and the rest of the peaks to values relative to max
      if (method_ == "to_one")
      {
        double max(0);
        for (ConstIterator it = spectrum.begin(); it != spectrum.end(); ++it)
        {
          if (max < it->getIntensity())
          {
            max = it->getIntensity();
          }
        }
        for (Iterator it = spectrum.begin(); it != spectrum.end(); ++it)
        {
          it->setIntensity(it->getIntensity() / max);
        }
      }
      // normalizes the peak intensities to the TIC
      else if (method_ == "to_TIC")
      {
        double sum(0);
        for (ConstIterator it = spectrum.begin(); it != spectrum.end(); ++it)
        {
          sum += it->getIntensity();
        }

        for (Iterator it = spectrum.begin(); it != spectrum.end(); ++it)
        {
          it->setIntensity(it->getIntensity() / sum);
        }
      }
      // method unknown
      else
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Method not known", method_);
      }
      return;

    }

    ///
    void filterPeakSpectrum(PeakSpectrum & spectrum);
    ///
    void filterPeakMap(PeakMap & exp);

    //TODO reimplement DefaultParamHandler::updateMembers_()

    // @}

private:
    String method_;

  };


}
#endif //OPENMS_FILTERING_TRANSFORMERS_NORMALIZER_H
