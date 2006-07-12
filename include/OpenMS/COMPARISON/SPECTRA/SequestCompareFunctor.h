// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2006 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Andreas Bertsch $
// --------------------------------------------------------------------------
//
#ifndef OPENMS_COMPARISON_SPECTRA_SEQUESTCOMPAREFUNCTOR_H
#define OPENMS_COMPARISON_SPECTRA_SEQUESTCOMPAREFUNCTOR_H

#include <OpenMS/COMPARISON/SPECTRA/CompareFunctor.h>

namespace OpenMS
{

  /**
  	@brief SequestCompareFunctor compares the peptide annotations from sequest, based on sequence and deltaCn
  
  	@param isobaric
    	if isobaric is one, not the sequences, but the fragment sizes are compared<br>
    	if isobaric is zero, only the sequences are compared <br>
  	@param maxdeltaCN compare sequences with at most that deltaCn <br>
  	@param tolerance mass tolerance for fragment masses <br>

		@ingroup SpectraComparison
  */
  class SequestCompareFunctor : public CompareFunctor
  {
  public:

		// @name Constructors and Destructors
		// @{
    /// default constructor
    SequestCompareFunctor();

    /// copy constructor
    SequestCompareFunctor(const SequestCompareFunctor& source);

    /// destructor
    virtual ~SequestCompareFunctor();
		// @}

		// @name Operators
    /// assignment operator
    SequestCompareFunctor& operator = (const SequestCompareFunctor& source);

		///
		double operator () (const ClusterSpectrum& a, const ClusterSpectrum& b) const;
		// @}

		// @name Accessors
		// @{
		///
    static FactoryProduct* create() { return new SequestCompareFunctor(); }

		///
		static const String getName()
		{
			return "SequestCompareFunctor";
		}

    /// similarity based on fragment sizes
    double matchIsobaric(const String& seq1, const String& seq2) const;
		// @}
		
  };
}
#endif // OPENMS_COMPARISON_SPECTRA_SEQUESTCOMPAREFUNCTOR_H
