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

#ifndef OPENMS_COMPARISON_SPECTRA_BINNEDREPMUTUALINFORMATION_H
#define OPENMS_COMPARISON_SPECTRA_BINNEDREPMUTUALINFORMATION_H

#include <OpenMS/COMPARISON/SPECTRA/CompareFunctor.h>

namespace OpenMS
{

  /**
		@brief Mutual Information content which is calculated from binned stick spectra

		Binned spectra mutual information content was defined in ????
  
  	@param intervals peak intensity is discretized into intervals

		@ingroup SpectraComparison
  */
  class BinnedRepMutualInformation
    : public CompareFunctor
  {
  public:

		// @name Constructors and Destructors
		// @{
    /// default constructor
    BinnedRepMutualInformation();

    /// copy constructor
    BinnedRepMutualInformation(const BinnedRepMutualInformation& source);

    /// destructor
    virtual ~BinnedRepMutualInformation();
		// @}
		
		// @name Operators
		// @{
    /// assignment operator
    BinnedRepMutualInformation& operator = (const BinnedRepMutualInformation& source);

		///
		double operator () (const ClusterSpectrum& csa, const ClusterSpectrum& csb) const;
		// @}

		// @name Accessors
		// @{
		///
    static FactoryProduct* create() {return new BinnedRepMutualInformation();}

		///
		static const String getName()
		{
			return "BinnedRepMutualInformation";
		}
		// @}

  };
}

#endif //OPENMS_COMPARISON_SPECTRA_BINNEDREPMUTUALINFORMATION_H

