// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_KERNEL_PEAKINDEX_H
#define OPENMS_KERNEL_PEAKINDEX_H


#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/CONCEPT/Exception.h>
#include <OpenMS/CONCEPT/Macros.h>
#include <limits>

namespace OpenMS
{
	/**
	  @brief Index of a peak or feature

		This struct can be used to store both peak or feature indices.
	*/
	struct PeakIndex
	{
		/// Default constructor. Creates an invalid peak reference
		inline PeakIndex()
			: peak(std::numeric_limits<UInt>::max()),
			  spectrum(std::numeric_limits<UInt>::max())
		{
		}
		/// Constructor that sets the peak index (for feaure maps)
		inline PeakIndex(UInt peak)
			: peak(peak),
			  spectrum(std::numeric_limits<UInt>::max())
		{
		}
		///Constructor that sets the peak and spectrum index (for peak maps)
		inline PeakIndex(UInt spectrum, UInt peak)
			: peak(peak),
			  spectrum(spectrum)
		{
		}

		/// returns if the current peak ref is valid
		inline bool isValid() const
		{
		  return (peak != std::numeric_limits<UInt>::max());
	  }
		///Invalidates the current index
		inline void clear()
		{
		  peak = std::numeric_limits<UInt>::max();
      spectrum = std::numeric_limits<UInt>::max();
	  }
		
		/**
		 @brief Access to the feature corresponding to this index
    
		 @exception Exception::Precondition is thrown if this index is invalid for the @p map (only in debug mode) 
		*/
		template <typename FeatureMapType>
		const typename FeatureMapType::FeatureType& getFeature(const FeatureMapType& map) const
		{
		  OPENMS_PRECONDITION(peak<map.size(),"Feature index exceeds map size");
			return map[peak];
		}
		/**
		 @brief Access to a peak corresponding to this index
    
		 @exception Exception::Precondition is thrown if this index is invalid for the @p map (only in debug mode) 
		*/
		template <typename PeakMapType>
		const typename PeakMapType::PeakType& getPeak(const PeakMapType& map) const
		{
		  OPENMS_PRECONDITION(spectrum<map.size(),"Spectrum index exceeds map size");
		  OPENMS_PRECONDITION(peak<map[spectrum].size(),"Peak index exceeds spectrum size");
		  return map[spectrum][peak];
		}
		/**
		 @brief Access to a spectrum corresponding to this index
    
		 @exception Exception::Precondition is thrown if this index is invalid for the @p map (only in debug mode) 
		*/
    template <typename PeakMapType>
		const typename PeakMapType::SpectrumType& getSpectrum(const PeakMapType& map) const
		{
		  OPENMS_PRECONDITION(spectrum<map.size(),"Spectrum index exceeds map size");
			return map[spectrum];
		}
   	
		///Equality operator
		inline bool operator==(const PeakIndex& rhs) const
		{
		  return peak==rhs.peak && spectrum==rhs.spectrum;
		}
		///Inequality operator
		inline bool operator!=(const PeakIndex& rhs) const
		{
		  return peak!=rhs.peak || spectrum!=rhs.spectrum;
		}
		
		/// Peak or feature index
		UInt peak;
		/// Spectrum index
		UInt spectrum;
	};

} // namespace OpenMS

#endif // OPENMS_KERNEL_PEAKINDEX_H
